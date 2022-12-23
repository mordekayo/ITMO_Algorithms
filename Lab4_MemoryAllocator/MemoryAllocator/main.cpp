#include <iostream>
#include <Windows.h>

#include "MemoryAllocator.h"

int main()
{
    MemoryAllocator Allocator;
    Allocator.Init();
    int *pi = (int *)Allocator.Alloc(sizeof(int));
    double *pd = (double *)Allocator.Alloc(sizeof(double));
    
    Allocator.DumpStat();
    Allocator.DumpBlocks();
    
    Allocator.Free(pi);
    Allocator.Free(pd);
    Allocator.Destroy();
    
    return 0;
}