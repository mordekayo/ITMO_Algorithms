#pragma once
#include <iostream>

class ProtoAllocator
{
    
public:
    
    ProtoAllocator();

    virtual ~ProtoAllocator();

    //We cannot copy allocators
    ProtoAllocator(const ProtoAllocator&) = delete;
    ProtoAllocator& operator=(const ProtoAllocator&) = delete;
    ProtoAllocator(ProtoAllocator&&) = delete;
    ProtoAllocator& operator=(const ProtoAllocator&&) = delete;
    
    virtual void Init();

    virtual void Destroy();

    virtual void* Alloc(size_t Size);

    virtual void Free(void* p);

#ifdef _DEBUG
    
    virtual void DumpStat() const;

    virtual  void DumpBlocks() const;

#endif

private:

    bool bIsInitialized;
};
