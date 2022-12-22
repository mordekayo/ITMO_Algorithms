#include <iostream>

#include "FSA/FixedSizeAllocator.h"

int main()
{
    FixedSizeAllocator Allocator;
    Allocator.Init(16, 10);
    Allocator.CheckValid();
    int* P = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P1 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P2 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P3 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P4 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P5 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P6 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P7 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P8 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P9 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P10 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P11 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P12 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P13 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P14 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P15 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P16 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P17 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P18 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P19 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P20 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P21 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();
    int* P22 = static_cast<int*>(Allocator.Alloc());
    Allocator.CheckValid();

    Allocator.Free(P);
    Allocator.CheckValid();
    Allocator.Free(P1);
    Allocator.CheckValid();
    Allocator.Free(P2);
    Allocator.CheckValid();
    Allocator.Free(P3);
    Allocator.CheckValid();
    Allocator.Free(P4);
    Allocator.CheckValid();
    Allocator.Free(P5);
    Allocator.CheckValid();
    Allocator.Free(P6);
    Allocator.CheckValid();
    Allocator.Free(P7);
    Allocator.CheckValid();
    Allocator.Free(P8);
    Allocator.CheckValid();
    Allocator.Free(P9);
    Allocator.CheckValid();
    Allocator.Free(P10);
    Allocator.CheckValid();
    Allocator.Free(P11);
    Allocator.CheckValid();
    Allocator.Free(P12);
    Allocator.CheckValid();
    Allocator.Free(P13);
    Allocator.CheckValid();
    Allocator.Free(P14);
    Allocator.CheckValid();
    Allocator.Free(P15);
    Allocator.CheckValid();
    Allocator.Free(P16);
    Allocator.CheckValid();
    Allocator.Free(P17);
    Allocator.CheckValid();
    Allocator.Free(P18);
    Allocator.CheckValid();
    Allocator.Free(P19);
    Allocator.CheckValid();
    Allocator.Free(P20);
    Allocator.CheckValid();
    Allocator.Free(P21);
    Allocator.CheckValid();
    Allocator.Free(P22);
    Allocator.CheckValid();
    
    Allocator.Destroy();
    
    return 0;
}