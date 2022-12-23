#include "FixedSizeAllocator.h"

#include <cassert>
#include <iostream>
#include <windows.h>

FixedSizeAllocator::~FixedSizeAllocator()
{
#ifdef _DEBUG
    assert(PageListHead == nullptr);
#endif
}

void FixedSizeAllocator::Init(int InBlockSize, int InBlocksPerPage)
{
#ifdef _DEBUG
    assert(InBlockSize > 0 && InBlocksPerPage > 0);
#endif
    BlockSize = InBlockSize;
    BlocksPerPage = InBlocksPerPage;
    PageListHead = AllocPage();
}

void FixedSizeAllocator::Destroy()
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
    assert(AllocCalls == FreeCalls && "Memory leak");
#endif

    FSAMemoryPage* Page = PageListHead;

    while(Page != nullptr)
    {
        FSAMemoryPage* PageToDestroy = Page;
        Page = Page->NextPage;
        VirtualFree(static_cast<void*>(PageToDestroy), 0, MEM_RELEASE);
    }
    PageListHead = nullptr;
}

void* FixedSizeAllocator::Alloc()
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
    AllocCalls += 1;
#endif

    auto Page = PageListHead;
    while(true)
    {
        if(Page->FreeListHead != -1)
        {
            const auto Block = static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(Page->FreeListHead * BlockSize);
            Page->FreeListHead = *reinterpret_cast<int*>(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(Page->FreeListHead * BlockSize));
            return Block;
        }
        if(Page->InitizalizedBlocks < BlocksPerPage)
        {
#ifdef _DEBUG
            const auto Block = static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>((Page->InitizalizedBlocks * 3 + 1) * BlockSize);
            const auto LeftDebugBlock = Block - BlockSize;
            for(int i = 0; i < BlockSize; ++i)
            {
                *(LeftDebugBlock + i) = LeftDebugFlag;
            }
            const auto RightDebugBlock = Block + BlockSize;
            for(int i = 0; i < BlockSize; ++i)
            {
                *(RightDebugBlock + i) = RightDebugFlag;
            }
#else
            const auto Block = static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(Page->InitizalizedBlocks * BlockSize);
#endif 
            Page->InitizalizedBlocks += 1;
#ifdef _DEBUG
            *reinterpret_cast<int*>(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>((Page->InitizalizedBlocks * 3 + 1) * BlockSize)) = -1;
#else
            *reinterpret_cast<int*>(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(Page->InitizalizedBlocks * BlockSize)) = -1;
#endif
            Page->FreeListHead = -1;
            return Block;
        }
        Page = Page->NextPage;
        if(Page == nullptr)
        {
            const auto NewPage = AllocPage();
            NewPage->NextPage = PageListHead;
            PageListHead = NewPage;
            Page = NewPage;
        }
    }
}

bool FixedSizeAllocator::Free(void* Block)
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
    FreeCalls += 1;
#endif

    auto Page = PageListHead;
    while(Page != nullptr)
    {
#ifdef _DEBUG
        if(Block >= static_cast<char*>(Page->Buffer) + BlockSize && Block <= static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(BlocksPerPage - 2) * BlockSize)
        {
            assert(*(static_cast<char*>(Block) - BlockSize) == LeftDebugFlag && "Incorrect pointer to block");
            assert(*(static_cast<char*>(Block) + BlockSize) == RightDebugFlag && "Incorrect pointer to block");
#else
            if(Block >= Page->Buffer && Block <= static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(BlocksPerPage - 1) * BlockSize)
            {
#endif
            *static_cast<int*>(Block) = Page->FreeListHead;
            Page->FreeListHead = static_cast<int>(static_cast<char*>(Block) - static_cast<char*>(Page->Buffer)) / BlockSize;
            return true;
        }
        Page = Page->NextPage;
    }
        return false;
}

void FixedSizeAllocator::CheckValid() const
{
#ifdef _DEBUG
    auto Page = PageListHead;
    while(Page != nullptr)
    {
        for(int i = 0; i < Page->InitizalizedBlocks; ++i)
        {
            for(int j = 0; j < BlockSize; ++j)
            {
                assert(LeftDebugFlag == *(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>((i * 3 + 1) * BlockSize - BlockSize + j)) && "Memory corruption");
            }
            for(int j = 0; j < BlockSize; ++j)
            {
                assert(RightDebugFlag == *(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>((i * 3 + 1) * BlockSize + BlockSize + j)) && "Memory corruption");
            }
        }
        Page = Page->NextPage;
    }
#endif
}

#ifdef _DEBUG


void FixedSizeAllocator::DumpStat() const
{
    assert(PageListHead != nullptr);

        auto Page = PageListHead;
        int PageNumber = 0;
        int FreeBlocks = 0;
        while(Page != nullptr)
        {
            PageNumber += 1;
            int BlockIndex = Page->FreeListHead;
            while(BlockIndex != -1)
            {
                FreeBlocks += 1;
                BlockIndex = *reinterpret_cast<int*>(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(BlockIndex * BlockSize));
            }
            FreeBlocks += BlocksPerPage - Page->InitizalizedBlocks;
            Page = Page->NextPage;
            std::cout << std::endl;
        }

        std::cout << "Allocated " << PageNumber << " pages of virtual memory" << std::endl;
        std::cout << "Each page size is " << sizeof(FSAMemoryPage) + sizeof(int) + sizeof(int) + static_cast<size_t>(BlockSize * (BlocksPerPage * 3)) << std::endl;
        std::cout << "Free blocks: " << FreeBlocks << std::endl;
        std::cout << "Busy blocks: " << BlocksPerPage - FreeBlocks << std::endl << std::endl;
    
}

void FixedSizeAllocator::DumpBlocks() const
{
    assert(PageListHead != nullptr);

        auto Page = PageListHead;
        while(Page != nullptr)
        {
            std::cout << "-----------Page 0----------" << std::endl << std::endl;
            for(int i = 0; i < BlocksPerPage; ++i)
            {
                int BlockToFindIndex = i * 3 + 1;
                int BlockIndex = Page->FreeListHead;
                bool bIsFree = false;
                while(BlockIndex != -1)
                {
                    if(BlockToFindIndex == BlockIndex)
                    {
                        bIsFree = true;
                    }
                    BlockIndex = *reinterpret_cast<int*>(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(BlockIndex * BlockSize));
                }
                if(bIsFree != true && BlockToFindIndex < Page->InitizalizedBlocks * 3 + 1)
                {
                    std::cout << "Address: " << static_cast<void*>(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(BlockToFindIndex * BlockSize)) << " Size: "<< BlockSize << std::endl;
                }
            }
            Page = Page->NextPage;
            std::cout << std::endl;
        }
}
#endif

FixedSizeAllocator::FSAMemoryPage* FixedSizeAllocator::AllocPage() const
{
#ifdef _DEBUG
    const auto Page = static_cast<FSAMemoryPage*>(VirtualAlloc(nullptr, sizeof(FSAMemoryPage) +sizeof(int) + sizeof(int) + static_cast<size_t>(BlockSize * (BlocksPerPage * 3)),
            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
#else
    const auto Page = static_cast<FSAMemoryPage*>(VirtualAlloc(nullptr, sizeof(FSAMemoryPage) + sizeof(int) + sizeof(int) + static_cast<size_t>(BlockSize * BlocksPerPage),
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
#endif
    
    Page->FreeListHead = -1;
    Page->InitizalizedBlocks = 0;
    Page->Buffer = reinterpret_cast<char*>(Page) + sizeof(FSAMemoryPage) + sizeof(int) + sizeof(int);
    
    return Page;
}
