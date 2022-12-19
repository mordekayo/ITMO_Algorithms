#include "ProtoAllocator.h"

#include <cassert>

ProtoAllocator::ProtoAllocator()
{
    bIsInitialized = false;
}

ProtoAllocator::~ProtoAllocator()
{
}

void ProtoAllocator::Init()
{
    
}

void ProtoAllocator::Destroy()
{
    
}

void* ProtoAllocator::Alloc(size_t Size)
{
    assert(bIsInitialized);
}

void ProtoAllocator::Free(void* p)
{
    assert(bIsInitialized);
}

void ProtoAllocator::DumpStat() const
{
    assert(bIsInitialized);
}

void ProtoAllocator::DumpBlocks() const
{
    assert(bIsInitialized);
}
