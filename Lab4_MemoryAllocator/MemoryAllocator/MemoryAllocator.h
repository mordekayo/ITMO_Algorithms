#pragma once
#include <map>

#include "FSA/FixedSizeAllocator.h"
#include "Coalesce/CoalesceAllocator.h"

class MemoryAllocator
{

public:
    
    MemoryAllocator() = default;

    virtual ~MemoryAllocator();

    //We cannot copy or move allocators
    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;
    MemoryAllocator(MemoryAllocator&&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&&) = delete;

    virtual void Init();

    virtual void Destroy();

    virtual void* Alloc(size_t Size);

    virtual void Free(void* Block);

#ifdef _DEBUG
    
    virtual void DumpStat() const;

    virtual  void DumpBlocks() const;


#endif

private:

#ifdef _DEBUG
    std::map<void*, size_t> OSBlocks;
    bool Initizalized = false;
#endif
    FixedSizeAllocator FSA16, FSA32, FSA64, FSA128, FSA256, FSA512;
    CoalesceAllocator CoalesceA;
    
};