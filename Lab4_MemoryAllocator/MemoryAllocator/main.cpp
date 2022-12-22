#include <iostream>

#include "FSA/FixedSizeAllocator.h"

int main()
{
    FixedSizeAllocator Allocator;
    Allocator.Init(16, 10);
    Allocator.CheckValid();
    int* P = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    *P = 10;
    std::cout << *P << std::endl;
    Allocator.Free(P);
    Allocator.CheckValid();
    Allocator.Destroy();
    
    return 0;
}