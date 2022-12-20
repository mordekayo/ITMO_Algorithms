#include "FixedSizeAllocator.h"

#include <cassert>

FixedSizeAllocator::FixedSizeAllocator()
{
}

FixedSizeAllocator::~FixedSizeAllocator()
{
#ifdef _DEBUG
    assert(PageListHead == nullptr);
#endif
}

void FixedSizeAllocator::Init(size_t InBlockSize, size_t InBlocksPerPage)
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

void* FixedSizeAllocator::Alloc(size_t Size)
{
#ifdef _DEBUG
    assert(PageListHead == nullptr);
#endif

    auto Block = PageListHead->Buffer;
    
    return nullptr;
}

void FixedSizeAllocator::Free(void* p)
{
#ifdef _DEBUG
    assert(PageListHead == nullptr);
#endif
}

void FixedSizeAllocator::DumpStat() const
{
#ifdef _DEBUG
    assert(PageListHead == nullptr);
#endif
}

void FixedSizeAllocator::DumpBlocks() const
{
#ifdef _DEBUG
    assert(PageListHead == nullptr);
#endif
}

FixedSizeAllocator::FSAMemoryPage* FixedSizeAllocator::AllocPage() const
{
    const auto Page = static_cast<FSAMemoryPage*>(VirtualAlloc(nullptr, static_cast<long long int>(BlockSize * BlocksPerPage) + sizeof(FSAMemoryPage),
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

    Page->Buffer = reinterpret_cast<char*>(Page) + sizeof(FSAMemoryPage);
    Page->FreeListHead = 0;
    return Page;
}
