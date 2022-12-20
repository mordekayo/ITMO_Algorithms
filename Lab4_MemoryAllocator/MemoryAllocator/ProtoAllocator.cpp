#include "ProtoAllocator.h"

#include <cassert>

ProtoAllocator::ProtoAllocator()
{
    bIsInitialized = false;
}

ProtoAllocator::~ProtoAllocator()
{
#ifdef _DEBUG
    assert(!bIsInitialized);
#endif
}

void ProtoAllocator::Init()
{
    bIsInitialized = true;
}

void ProtoAllocator::Destroy()
{
    bIsInitialized = false;
}

void* ProtoAllocator::Alloc(size_t Size)
{
    std::cout << "Proto Alloc" << std::endl;

    return nullptr;
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
