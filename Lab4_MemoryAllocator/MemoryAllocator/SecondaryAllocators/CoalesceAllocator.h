#pragma once
#include "ProtoAllocator.h"

class CoalesceAllocator : ProtoAllocator
{

public:
    
    CoalesceAllocator();

    ~CoalesceAllocator() override;

    //We cannot copy allocators
    CoalesceAllocator(const CoalesceAllocator&) = delete;
    CoalesceAllocator& operator=(const CoalesceAllocator&) = delete;
    CoalesceAllocator(CoalesceAllocator&&) = delete;
    CoalesceAllocator& operator=(const CoalesceAllocator&&) = delete;
    
    void Init() override;

    void Destroy() override;

    void* Alloc(size_t Size) override;

    void Free(void* p) override;

#ifdef _DEBUG
    
    void DumpStat() const override; 

    void DumpBlocks() const override;

#endif

private:

    
};

