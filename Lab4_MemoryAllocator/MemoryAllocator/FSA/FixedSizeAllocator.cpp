#include "FixedSizeAllocator.h"

#include <cassert>
#include <iostream>

FixedSizeAllocator::FixedSizeAllocator()
{
}

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
#endif

    auto Page = PageListHead;
    while(Page != nullptr)
    {
        if(Page->FreeListHead != -1)
        {
            const auto Block = static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(Page->FreeListHead * BlockSize);
            Page->FreeListHead = *reinterpret_cast<int*>(static_cast<char*>(Page->Buffer) + static_cast<ptrdiff_t>(Page->FreeListHead * BlockSize));
            return Block;
        }
        Page = Page->NextPage;
    }

    const auto NewPage = AllocPage();
    NewPage->NextPage = PageListHead;
    PageListHead = NewPage;
    
    return PageListHead->Buffer;
}

void FixedSizeAllocator::Free(void* Block) const
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
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

void FixedSizeAllocator::DumpStat() const
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
#endif
}

void FixedSizeAllocator::DumpBlocks() const
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
#endif
}

FixedSizeAllocator::FSAMemoryPage* FixedSizeAllocator::AllocPage() const
{
    const auto Page = static_cast<FSAMemoryPage*>(VirtualAlloc(nullptr, static_cast<long long int>(BlockSize * BlocksPerPage) + sizeof(FSAMemoryPage),
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

    Page->Buffer = reinterpret_cast<char*>(Page) + sizeof(FSAMemoryPage);
    Page->FreeListHead = 0;

    for(int i = 0; i < BlocksPerPage - 1; ++i)
    {
        *static_cast<int*>(Page->Buffer) = i + 1;
    }

    *static_cast<int*>(Page->Buffer) = -1;
    
    return Page;
}
