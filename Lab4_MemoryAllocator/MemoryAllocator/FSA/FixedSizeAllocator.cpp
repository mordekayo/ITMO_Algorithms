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
            const auto Block = static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(Page->InitizalizedBlocks * BlockSize);
            Page->InitizalizedBlocks += 1;
            *reinterpret_cast<int*>(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(Page->InitizalizedBlocks * BlockSize)) = -1;
            Page->FreeListHead = Page->InitizalizedBlocks;
            return Block;
        }
        Page = Page->NextPage;
        if(Page == nullptr)
        {
            const auto NewPage = AllocPage();
            NewPage->NextPage = PageListHead;
            PageListHead = NewPage;
        }
    }
}

void FixedSizeAllocator::Free(void* Block)
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
    FreeCalls += 1;
#endif

    auto Page = PageListHead;
    while(Page != nullptr)
    {
        if(Block >= Page->Buffer && Block <= static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(BlocksPerPage - 1) * BlockSize)
        {
            *static_cast<int*>(Block) = Page->FreeListHead;
            Page->FreeListHead = static_cast<int>(static_cast<char*>(Block) - static_cast<char*>(Page->Buffer)) / BlockSize;
            return;
        }
        Page = Page->NextPage;
    }
}

#ifdef _DEBUG
void FixedSizeAllocator::DumpStat() const
{
    assert(PageListHead != nullptr);
    
}

void FixedSizeAllocator::DumpBlocks() const
{

    assert(PageListHead != nullptr);

}
#endif

FixedSizeAllocator::FSAMemoryPage* FixedSizeAllocator::AllocPage() const
{
    const auto Page = static_cast<FSAMemoryPage*>(VirtualAlloc(nullptr, static_cast<long long int>(BlockSize * BlocksPerPage) + sizeof(FSAMemoryPage),
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

    Page->Buffer = reinterpret_cast<char*>(Page) + sizeof(FSAMemoryPage);
    Page->FreeListHead = -1;
    Page->InitizalizedBlocks = 0;
    
    return Page;
}
