#pragma once
#include <climits>

class CoalesceAllocator
{
private:
    
    struct CoalesceMemoryPage
    {
        CoalesceMemoryPage* NextPage = nullptr;
        void* FreeListHead = nullptr;
        void* Buffer = nullptr;
    };

    struct CoalesceMetaData
    {
        CoalesceMetaData* PrevBlock;
        CoalesceMetaData* NextBlock;
        size_t BlockSize;
        bool bIsFree;
    };

    struct FreeListNode
    {
        FreeListNode* PrevFreeListBlock;
        FreeListNode* NextFreeListBlock;
    };

public:
    
    CoalesceAllocator() = default;

    ~CoalesceAllocator();

    //We cannot copy allocators
    CoalesceAllocator(const CoalesceAllocator&) = delete;
    CoalesceAllocator& operator=(const CoalesceAllocator&) = delete;
    CoalesceAllocator(CoalesceAllocator&&) = delete;
    CoalesceAllocator& operator=(const CoalesceAllocator&&) = delete;
    
    void Init(size_t PageSize = 10 * MB);

    void Destroy();

    void* Alloc(size_t Size);

    bool Free(void* Block);

    void CheckValid() const;

#ifdef _DEBUG
    
    void DumpStat() const; 

    void DumpBlocks() const;

#endif

private:
    
    CoalesceMemoryPage* PageListHead = nullptr;
    
    [[nodiscard]] CoalesceMemoryPage* AllocPage() const;

    size_t PageSize;
#ifdef _DEBUG
    size_t MinimumBlockSize = sizeof(CoalesceMetaData) + sizeof(FreeListNode) + sizeof(long long) * 2;

    static constexpr size_t MB = 1048576;
    static constexpr long long LeftDebugFlag = LLONG_MIN;
    static constexpr long long RightDebugFlag = LLONG_MAX;
#endif
};

