#include "MemoryAllocator.h"
#include <Windows.h>

MemoryAllocator::MemoryAllocator()
{
    
}

MemoryAllocator::~MemoryAllocator()
{
    
}

void MemoryAllocator::Init()
{
    
}

void MemoryAllocator::Destroy()
{
    
}

void* MemoryAllocator::Alloc(size_t Size)
{
    if(Size <= 16)
    {
        return FSA16.Alloc();
    }
    if(Size <= 32)
    {
        return FSA32.Alloc();
    }
    if(Size <= 64)
    {
        return FSA64.Alloc();
    }
    if(Size <= 128)
    {
        return FSA128.Alloc();
    }
    if(Size <= 256)
    {
        return FSA256.Alloc();
    }
    if(Size <= 512)
    {
        return FSA512.Alloc();
    }   
    if(Size <= 10 * MB )
    {
        return FSA16.Alloc();
    }

    void* p = VirtualAlloc(nullptr, Size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    return p;
}

void MemoryAllocator::Free(void* p)
{
    /*
    if(16)
    {
        return FSA16.Free(p);
    }
    if(32)
    {
        return FSA32.Free(p);
    }
    if(64)
    {
        return FSA64.Free(p);
    }
    if(128)
    {
        return FSA128.Free(p);
    }
    if(256)
    {
        return FSA256.Free(p);
    }
    if(512)
    {
        return FSA512.Free(p);
    }   
    if(10 * MB )
    {
        return FSA16.Free(p);
    }
    */
    
    bool res = VirtualFree(p, 0, MEM_RELEASE);
    /*
    assert(res && "Poiner out of bounds");
    if (res) {
        for (auto it = OSBlocks.begin(); it < OSBlocks.end(); it++) {
            if (static_cast<Block>(*it).data == p) {
                OSBlocks.erase(it);
                break;
            }
        }
    }
    */
}
