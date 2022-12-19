#pragma once
#include "ProtoAllocator.h"

class FixedSizeAllocator : ProtoAllocator
{

public:
    
    FixedSizeAllocator();

    ~FixedSizeAllocator() override;

    //We cannot copy allocators
    FixedSizeAllocator(const FixedSizeAllocator&) = delete;
    FixedSizeAllocator& operator=(const FixedSizeAllocator&) = delete;
    FixedSizeAllocator(FixedSizeAllocator&&) = delete;
    FixedSizeAllocator& operator=(const FixedSizeAllocator&&) = delete;
    
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
