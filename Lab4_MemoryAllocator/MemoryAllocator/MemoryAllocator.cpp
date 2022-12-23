#include "MemoryAllocator.h"

#include <cassert>
#include <iostream>
#include <Windows.h>
constexpr size_t MB = 1048576;

MemoryAllocator::~MemoryAllocator()
{
#ifdef _DEBUG
    assert(Initizalized != true);
#endif
}

void MemoryAllocator::Init()
{
    FSA16.Init(16, 10);
    FSA32.Init(42, 10);
    FSA64.Init(64, 10);
    FSA128.Init(128, 10);
    FSA256.Init(256, 10);
    FSA512.Init(512, 10);
    CoalesceA.Init();
    Initizalized = true;
}

void MemoryAllocator::Destroy()
{
#ifdef _DEBUG
    assert(Initizalized == true);
#endif
    FSA16.Destroy();
    FSA32.Destroy();
    FSA64.Destroy();
    FSA128.Destroy();
    FSA256.Destroy();
    FSA512.Destroy();
    CoalesceA.Destroy();
    Initizalized = false;
}

void* MemoryAllocator::Alloc(size_t Size)
{
#ifdef _DEBUG
    assert(Initizalized == true);
#endif
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

    void* Block = VirtualAlloc(nullptr, Size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

#ifdef _DEBUG
    OSBlocks.insert(std::pair<void*, size_t>(Block, Size));
#endif
    
    return Block;
}

void MemoryAllocator::Free(void* Block)
{
#ifdef _DEBUG
    assert(Initizalized == true);
#endif
    if(FSA16.Free(Block))
    {
        return;
    }
    if(FSA32.Free(Block))
    {
        return;
    }
    if( FSA64.Free(Block))
    {
        return;
    }
    if(FSA128.Free(Block))
    {
        return;
    }
    if(FSA256.Free(Block))
    {
        return;
    }
    if(FSA512.Free(Block))
    {
        return;
    }   
    if(CoalesceA.Free(Block))
    {
        return;
    }
    
    bool Result = VirtualFree(Block, 0, MEM_RELEASE);
    
    assert(Result && "Poiner out of bounds");

#ifdef _DEBUG
    OSBlocks.try_emplace(Block);
#endif
}

#ifdef _DEBUG

void MemoryAllocator::DumpStat() const
{
#ifdef _DEBUG
    assert(Initizalized == true);
#endif
    std::cout << "Fixed size allocator (16 bytes) stat:" <<std::endl;
    FSA16.DumpStat();
    std::cout << "Fixed size allocator (32 bytes) stat:" <<std::endl;
    FSA32.DumpStat();
    std::cout << "Fixed size allocator (64 bytes) stat:" <<std::endl;
    FSA64.DumpStat();
    std::cout << "Fixed size allocator (128 bytes) stat:" <<std::endl;
    FSA128.DumpStat();
     std::cout << "Fixed size allocator (256 bytes) stat:" <<std::endl;
    FSA256.DumpStat();
    std::cout << "Fixed size allocator (512 bytes) stat:" <<std::endl;
    FSA512.DumpStat();
    std::cout << "Coalesce allocator stat:" <<std::endl;
    CoalesceA.DumpStat();
    std::cout << "Direct OS allocations stat:" <<std::endl;

    std::cout << "Allocated " <<  OSBlocks.size() << " blocks of virtual memory directly from OS" << std::endl << std::endl;;
}

void MemoryAllocator::DumpBlocks() const
{
#ifdef _DEBUG
    assert(Initizalized == true);
#endif
    std::cout << "Fixed size allocator (16 bytes) occupied blocks:" <<std::endl;
    FSA16.DumpBlocks();
    std::cout << "Fixed size allocator (32 bytes) occupied blocks:" <<std::endl;
    FSA32.DumpBlocks();
    std::cout << "Fixed size allocator (64 bytes) occupied blocks:" <<std::endl;
    FSA64.DumpBlocks();
    std::cout << "Fixed size allocator (128 bytes) occupied blocks:" <<std::endl;
    FSA128.DumpBlocks();
    std::cout << "Fixed size allocator (256 bytes) occupied blocks:" <<std::endl;
    FSA256.DumpBlocks();
    std::cout << "Fixed size allocator (512 bytes) occupied blocks:" <<std::endl;
    FSA512.DumpBlocks();
    std::cout << "Coalesce allocator occupied blocks:" <<std::endl;
    CoalesceA.DumpBlocks();
    std::cout << "Direct OS allocations occupied blocks:" <<std::endl;

    std::cout << "Allocated " <<  OSBlocks.size() << " blocks of virtual memory directly from OS" << std::endl;
    for(auto i : OSBlocks)
    {
        std::cout << "Address: " << i.first << " First: " << i.second << std::endl << std::endl;;
    }
}

#endif