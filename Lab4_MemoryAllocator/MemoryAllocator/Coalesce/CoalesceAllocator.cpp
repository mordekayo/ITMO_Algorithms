#include "CoalesceAllocator.h"

#include <cassert>
#include <windows.h>

CoalesceAllocator::~CoalesceAllocator()
{
#ifdef _DEBUG
    assert(PageListHead == nullptr);
#endif
}

void CoalesceAllocator::Init(size_t InPageSize = 10 * MB)
{
#ifdef _DEBUG
    assert(PageSize > MinimumBlockSize);
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
        if(Page->FreeListHead != -1)
        {
            /*
            //Determine block size
            auto Block = static_cast<CoalesceMetaData*>(Page->Buffer + Page->FreeListHead);
            while(true)
            {
                const char* EndOfBlock = reinterpret_cast<char*>(Block->NextBlock);
                if(EndOfBlock == nullptr)
                {
                    EndOfBlock = static_cast<char*>(Page->Buffer + PageSize);
                }

                //Looking for appropriate block
                if(EndOfBlock - reinterpret_cast<char*>(Block) <= Size)
                {
                    return Block;
                }
                Block = (reinterpret_cast<char*>(Block) + Block->NextFreeBlock * sizeof(char*));
            }
            */
        }
        Page = Page->NextPage;
        if(Page == nullptr)
        {
            const auto NewPage = AllocPage();
            NewPage->NextPage = PageListHead;
            PageListHead = NewPage;
        }
    }
    
    return nullptr;
}

void CoalesceAllocator::Free(void* p)
{
    
}

#ifdef _DEBUG
void CoalesceAllocator::DumpStat() const
{
    
}

void CoalesceAllocator::DumpBlocks() const
{
    
}
#endif

CoalesceAllocator::CoalesceMemoryPage* CoalesceAllocator::AllocPage() const
{
#ifdef _DEBUG
    assert(PageListHead != nullptr);
#endif

    auto Page = static_cast<CoalesceMemoryPage*>(VirtualAlloc(nullptr,
        sizeof(CoalesceMemoryPage) + sizeof(int) + PageSize,
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    
    Page->Buffer = reinterpret_cast<char*>(Page) + sizeof(CoalesceMemoryPage) + sizeof(int);
    Page->FreeListHead = 0;

#ifdef _DEBUG
    
    // *static_cast<long long*>(Page->Buffer) = LeftDebugFlag;
    // static_cast<CoalesceMetaData*>(Page->Buffer + sizeof(long long))->PrevBlock = nullptr;
    // static_cast<CoalesceMetaData*>(Page->Buffer + sizeof(long long))->NextBlock = nullptr;
    // static_cast<CoalesceMetaData*>(Page->Buffer + sizeof(long long))->NextFreeBlock = -1;
    // *static_cast<long long*>(Page->Buffer + PageSize - sizeof(long long)) = RightDebugFlag;
    
#else
    static_cast<CoalesceMetaData*>(Page->Buffer)->PrevBlock = nullptr;
    static_cast<CoalesceMetaData*>(Page->Buffer)->NextBlock = nullptr;
    //static_cast<CoalesceMetaData*>(Page->Buffer)->NextFreeBlock = -1;
#endif
    
    return Page;
}
