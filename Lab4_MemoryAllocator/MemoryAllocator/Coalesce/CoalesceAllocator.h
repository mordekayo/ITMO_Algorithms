#pragma once
#include <climits>
#include <cstdint>

constexpr size_t MB = 1048576;
constexpr long long LeftDebugFlag = LLONG_MIN;
constexpr long long RightDebugFlag = LLONG_MAX;

class CoalesceAllocator
{
private:
    
    struct CoalesceMemoryPage
    {
        CoalesceMemoryPage* NextPage = nullptr;
        int FreeListHead = 0;
        void* Buffer = nullptr;
    };

    struct CoalesceMetaData
    {
        CoalesceMetaData* PrevBlock;
        CoalesceMetaData* NextBlock;
        CoalesceMetaData* NextFreeBlock;
        CoalesceMetaData* PrevFreeBlock;
    };
    
public:
    
    CoalesceAllocator() = default;

    ~CoalesceAllocator();

    //We cannot copy allocators
    CoalesceAllocator(const CoalesceAllocator&) = delete;
    CoalesceAllocator& operator=(const CoalesceAllocator&) = delete;
    CoalesceAllocator(CoalesceAllocator&&) = delete;
    CoalesceAllocator& operator=(const CoalesceAllocator&&) = delete;
    
    void Init(size_t PageSize);

    void Destroy();

    void* Alloc(size_t Size);

    void Free(void* p);

#ifdef _DEBUG
    
    void DumpStat() const; 

    void DumpBlocks() const;

#endif

private:
    
    CoalesceMemoryPage* PageListHead = nullptr;
    
    [[nodiscard]] CoalesceMemoryPage* AllocPage() const;

    size_t PageSize;
    size_t MinimumBlockSize;
};

