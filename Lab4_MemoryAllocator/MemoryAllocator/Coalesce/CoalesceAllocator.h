#pragma once
#include "../ProtoAllocator.h"

class CoalesceAllocator
{

public:
    
    CoalesceAllocator();

    ~CoalesceAllocator();

    //We cannot copy allocators
    CoalesceAllocator(const CoalesceAllocator&) = delete;
    CoalesceAllocator& operator=(const CoalesceAllocator&) = delete;
    CoalesceAllocator(CoalesceAllocator&&) = delete;
    CoalesceAllocator& operator=(const CoalesceAllocator&&) = delete;
    
    void Init();

    void Destroy();

    void* Alloc(size_t Size);

    void Free(void* p);

#ifdef _DEBUG
    
    void DumpStat() const; 

    void DumpBlocks() const;

#endif

private:

    
};

