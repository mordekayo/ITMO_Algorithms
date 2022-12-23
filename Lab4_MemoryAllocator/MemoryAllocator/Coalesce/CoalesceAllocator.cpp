#include "CoalesceAllocator.h"

#include <cassert>
#include <iostream>
#include <windows.h>

CoalesceAllocator::~CoalesceAllocator()
{
#ifdef _DEBUG
    assert(PageListHead == nullptr);
#endif
}

void CoalesceAllocator::Init(size_t InPageSize)
{
#ifdef _DEBUG
    assert(InPageSize > MinimumBlockSize);
#endif
    PageSize = InPageSize;
    PageListHead = AllocPage();
}

void CoalesceAllocator::Destroy()
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
    //assert(AllocCalls == FreeCalls && "Memory leak");
#endif

    CoalesceMemoryPage* Page = PageListHead;

    while(Page != nullptr)
    {
        CoalesceMemoryPage* PageToDestroy = Page;
        Page = Page->NextPage;
        VirtualFree(static_cast<void*>(PageToDestroy), 0, MEM_RELEASE);
    }
    PageListHead = nullptr;
}

void* CoalesceAllocator::Alloc(size_t Size)
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
#endif

    auto Page = PageListHead;
    while(true)
    {
        if(Page->FreeListHead != nullptr)
        {
            auto Block = static_cast<CoalesceMetaData*>(Page->FreeListHead);
            while(true)
            {
                //Find appropriate block by size
#ifdef _DEBUG
                if(Block->BlockSize >= (Size  + sizeof(CoalesceMetaData) + sizeof(FreeListNode) + sizeof(long long) * 2))
                {
                    const auto PrevNext = Block->NextBlock;
                    const auto PrevSize = Block->BlockSize;
                    const auto NewBlock= reinterpret_cast<CoalesceMetaData*>(reinterpret_cast<char*>(Block) + sizeof(CoalesceMetaData) + Size + sizeof(long long) * 2);
                    Block->BlockSize = Size;
                    Block->NextBlock = NewBlock;
                    *reinterpret_cast<long long*>(reinterpret_cast<char*>(NewBlock) - sizeof(long long) * 2) = RightDebugFlag;
                    *reinterpret_cast<long long*>(reinterpret_cast<char*>(NewBlock) - sizeof(long long)) = LeftDebugFlag;
                    NewBlock->PrevBlock = Block;
                    NewBlock->NextBlock = PrevNext;
                    NewBlock->BlockSize = PrevSize - Size - sizeof(CoalesceMetaData) - sizeof(long long) * 2;
                    reinterpret_cast<FreeListNode*>(reinterpret_cast<char*>(NewBlock + sizeof(CoalesceMetaData)))->PrevFreeListBlock = nullptr;
                    reinterpret_cast<FreeListNode*>(reinterpret_cast<char*>(NewBlock + sizeof(CoalesceMetaData)))->NextFreeListBlock = reinterpret_cast<FreeListNode*>(reinterpret_cast<char*>(Block + sizeof(CoalesceMetaData)))->NextFreeListBlock;
                    Page->FreeListHead = NewBlock;
                    Block->bIsFree = false;
                    NewBlock->bIsFree = true;
                    
                    return reinterpret_cast<char*>(Block) + sizeof(CoalesceMetaData);
                }
#else
                if(Block->BlockSize >= Size  + sizeof(CoalesceMetaData) + sizeof(FreeListNode))
                {
                    const auto PrevNext = Block->NextBlock;
                    const auto PrevSize = Block->BlockSize;
                    Block->NextBlock = reinterpret_cast<CoalesceMetaData*>(reinterpret_cast<char*>(Block) + sizeof(CoalesceMetaData) + Size);
                    Block->BlockSize = Size;
                    const auto NewBlock = Block->NextBlock;
                    NewBlock->PrevBlock = Block;
                    NewBlock->NextBlock = PrevNext;
                    NewBlock->BlockSize = PrevSize - Size - sizeof(CoalesceMetaData);
                    reinterpret_cast<FreeListNode*>(reinterpret_cast<char*>(NewBlock + sizeof(CoalesceMetaData)))->PrevFreeListBlock = nullptr;
                    reinterpret_cast<FreeListNode*>(reinterpret_cast<char*>(NewBlock + sizeof(CoalesceMetaData)))->NextFreeListBlock = reinterpret_cast<FreeListNode*>(reinterpret_cast<char*>(Block + sizeof(CoalesceMetaData)))->NextFreeListBlock;
                    Page->FreeListHead = NewBlock;
                    Block->bIsFree = false;
                    NewBlock->bIsFree = true;
                    
                    return reinterpret_cast<char*>(Block) + sizeof(CoalesceMetaData);
                }
#endif
                Block = reinterpret_cast<CoalesceMetaData*>(reinterpret_cast<FreeListNode*>(Block + sizeof(CoalesceMetaData))->NextFreeListBlock);
                if(Block == nullptr)
                {
                    break;
                }
            }
        }
        Page = Page->NextPage;
        if(Page == nullptr)
        {
            const auto NewPage = AllocPage();
            NewPage->NextPage = PageListHead;
            PageListHead = NewPage;
            Page = PageListHead;
        }
    }
}

bool CoalesceAllocator::Free(void* Block)
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
#endif

    auto BlockMeta = reinterpret_cast<CoalesceMetaData*>(static_cast<char*>(Block) - sizeof(CoalesceMetaData));
    auto Page = PageListHead;
    while(Page != nullptr)
    {
#ifdef _DEBUG
        if(Block >= static_cast<char*>(Page->Buffer) + sizeof(long long) + sizeof(CoalesceMetaData) && Block <= static_cast<char*>(Page->Buffer) + PageSize - sizeof(long long))
        {
            const auto LeftDebugFlagValue = *reinterpret_cast<long long*>(static_cast<char*>(Block) - sizeof(CoalesceMetaData) - sizeof(long long));
            assert(LeftDebugFlagValue == LeftDebugFlag && "Incorrect pointer to block");
            const auto RightDebugFlagValue = *reinterpret_cast<long long*>(static_cast<char*>(Block) + BlockMeta->BlockSize);
            assert(RightDebugFlagValue == RightDebugFlag && "Incorrect pointer to block");
#else
            if(Block >= Page->Buffer + sizeof(CoalesceMetaData) && Block <= static_cast<char*>(Page->Buffer) + PageSize - sizeof(CoalesceMetaData))
            {
#endif
                //Coalision
                
                if(BlockMeta->PrevBlock != nullptr && BlockMeta->PrevBlock->bIsFree)
                {
                    const auto PrevBlockMeta = BlockMeta->PrevBlock;
                    PrevBlockMeta->NextBlock = BlockMeta->NextBlock;
                    BlockMeta->NextBlock->PrevBlock = PrevBlockMeta;
#ifdef _DEBUG
                    PrevBlockMeta->BlockSize = PrevBlockMeta->BlockSize + sizeof(long long) * 2 + sizeof(CoalesceMetaData) + BlockMeta->BlockSize;
#else
                    PrevBlockMeta->BlockSize = PrevBlockMeta->BlockSize + sizeof(CoalesceMetaData) + BlockMeta->BlockSize;
#endif
                    BlockMeta = PrevBlockMeta;
                }
                if(BlockMeta->NextBlock != nullptr && BlockMeta->NextBlock->bIsFree)
                {
                    const auto NextBlockMeta = BlockMeta->NextBlock;
                    const auto NextBlockFreeListNode = reinterpret_cast<FreeListNode*>(reinterpret_cast<char*>(NextBlockMeta + sizeof(CoalesceMetaData)));
                    if(NextBlockFreeListNode->PrevFreeListBlock != nullptr)
                    {
                        NextBlockFreeListNode->PrevFreeListBlock->NextFreeListBlock = NextBlockFreeListNode->NextFreeListBlock;
                    }
                    if(NextBlockFreeListNode->NextFreeListBlock != nullptr)
                    {
                        NextBlockFreeListNode->NextFreeListBlock->PrevFreeListBlock = NextBlockFreeListNode->PrevFreeListBlock;
                    }
                    BlockMeta->NextBlock = NextBlockMeta->NextBlock;
                    if(NextBlockMeta->NextBlock != nullptr)
                    {
                        NextBlockMeta->NextBlock->PrevBlock = BlockMeta;
                    }
#ifdef _DEBUG
                    BlockMeta->BlockSize = BlockMeta->BlockSize + sizeof(long long) * 2 + sizeof(CoalesceMetaData) + NextBlockMeta->BlockSize;
#else
                    BlockMeta->BlockSize = BlockMeta->BlockSize + sizeof(CoalesceMetaData) + NextBlockMeta->BlockSize;
#endif
                }
                BlockMeta->bIsFree = true;
                if(Page->FreeListHead == nullptr)
                {
                    Page->FreeListHead = BlockMeta;
                }
                
                return true;
            }
            Page = Page->NextPage;
        }
        return false;
}

void CoalesceAllocator::CheckValid() const
{
#ifdef _DEBUG
        
        assert(PageListHead != nullptr);
        
        auto Page = PageListHead;
        while(Page != nullptr)
        {
            auto Block = reinterpret_cast<CoalesceMetaData*>(static_cast<char*>(Page->Buffer) + sizeof(long long));
            while(Block != nullptr)
            {
                const auto LeftFlagValue = *reinterpret_cast<long long*>(reinterpret_cast<char*>(Block) - sizeof(long long));
                assert(LeftFlagValue == LeftDebugFlag && "Memory corruption");
                const auto RightFlagValue = *reinterpret_cast<long long*>(reinterpret_cast<char*>(Block) + sizeof(CoalesceMetaData) + Block->BlockSize);
                assert(RightFlagValue == RightDebugFlag && "Memory corruption");
                Block = Block->NextBlock;
            }
            Page = Page->NextPage;
        }
#endif
}



#ifdef _DEBUG
void CoalesceAllocator::DumpStat() const
{
        assert(PageListHead != nullptr);
        
        auto Page = PageListHead;
        int PageNumber = 0;
        int FreeBlocks = 0;
        int BusyBlocks = 0;
        while(Page != nullptr)
        {
            PageNumber += 1;
            auto Block = reinterpret_cast<CoalesceMetaData*>(static_cast<char*>(Page->Buffer) + sizeof(long long));
            while(Block != nullptr)
            {
                if(Block->bIsFree)
                {
                    FreeBlocks += 1;
                }
                else
                {
                    BusyBlocks += 1;
                }
                Block = Block->NextBlock;
            }
            Page = Page->NextPage;
            std::cout << std::endl;
        }

        std::cout << "Allocated " << PageNumber << " pages of virtual memory" << std::endl;
        std::cout << "Each page size is " << sizeof(CoalesceMemoryPage) + sizeof(int) + PageSize << std::endl;
        std::cout << "Free blocks: " << FreeBlocks << std::endl;
        std::cout << "Busy blocks: " << BusyBlocks << std::endl << std::endl;;
}

void CoalesceAllocator::DumpBlocks() const
{
        assert(PageListHead != nullptr);
        
        auto Page = PageListHead;
        while(Page != nullptr)
        {
            std::cout << "-----------Page 0----------" << std::endl << std::endl;
            auto Block = reinterpret_cast<CoalesceMetaData*>(static_cast<char*>(Page->Buffer) + sizeof(long long));
            while(Block != nullptr)
            {
                if(!Block->bIsFree)
                {
                    std::cout << "Address: " << Block << " Size:" << Block->BlockSize << "." <<  std::endl;
                }
                Block = Block->NextBlock;
            }
            Page = Page->NextPage;
            std::cout << std::endl;
            std::cout << "---------------------------" << std::endl;
        }
}

#endif

CoalesceAllocator::CoalesceMemoryPage* CoalesceAllocator::AllocPage() const
{
    auto Page = static_cast<CoalesceMemoryPage*>(VirtualAlloc(nullptr, sizeof(CoalesceMemoryPage) + sizeof(int) + PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    
    Page->Buffer = reinterpret_cast<char*>(Page) + sizeof(CoalesceMemoryPage) + sizeof(int);

#ifdef _DEBUG
    
    *static_cast<long long*>(Page->Buffer) = LeftDebugFlag;
    *reinterpret_cast<long long*>(static_cast<char*>(Page->Buffer) + PageSize - sizeof(long long)) = RightDebugFlag;
    const auto BlockMetaData = reinterpret_cast<CoalesceMetaData*>(static_cast<char*>(Page->Buffer) + sizeof(long long));
    BlockMetaData->PrevBlock = nullptr;
    BlockMetaData->NextBlock = nullptr;
    BlockMetaData->BlockSize = PageSize - sizeof(long long) * 2 - sizeof(CoalesceMetaData);
        BlockMetaData->bIsFree = true;
    const auto FreeListNodeInBlock = reinterpret_cast<FreeListNode*>(static_cast<char*>(Page->Buffer) + sizeof(long long) + sizeof(CoalesceMetaData));
    FreeListNodeInBlock->PrevFreeListBlock = nullptr;
    FreeListNodeInBlock->NextFreeListBlock = nullptr;

#else
    
    const auto BlockMetaData = static_cast<CoalesceMetaData*>(Page->Buffer);
    BlockMetaData->PrevBlock = nullptr;
    BlockMetaData->NextBlock = nullptr;
    BlockMetaData->BlockSize = PageSize - sizeof(CoalesceMetaData);
        BlockMetaData->bIsFree = true;
    const auto FreeListNodeInBlock = static_cast<FreeListNode*>(Page->Buffer + sizeof(CoalesceMetaData));
    FreeListNodeInBlock->PrevFreeListBlock = nullptr;
    FreeListNodeInBlock->NextFreeListBlock = nullptr;

#endif

    Page->FreeListHead = BlockMetaData;
    
    return Page;
}
