#include <iostream>

#include "FSA/FixedSizeAllocator.h"

int main()
{
    FixedSizeAllocator Allocator;

    Allocator.Init(16, 10);
    int* P1 = new int;
    int* P2 = static_cast<int*>(Allocator.Alloc());
    *P1 = 10;
    std::cout << *P1 << std::endl;
    *P2 = 10;
    std::cout << *P2 << std::endl;
    Allocator.Destroy();
    return 0;
}