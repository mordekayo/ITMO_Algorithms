#include "MemoryAllocator.h"

int main()
{
    MemoryAllocator Allocator;

    Allocator.Init();
    Allocator.Destroy();
    return 0;
}