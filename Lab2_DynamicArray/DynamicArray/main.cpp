#include <iostream>

#include "DynamicArray.h"

int main(int argc, char* argv[])
{
    DynamicArray<int> Array( 0);
    for (int i = 0; i < 9; ++i)
    {
        Array.Insert(i);
    }

    auto it =  Array.GetIterator();
    auto rit = Array.GetReverseIterator();

    for(int i = 0 ; i < Array.GetSize(); ++i, it.Next(), rit.Next())
    {
        std::cout << Array[i] << " ";
        std::cout << it.Get() << " ";
        std::cout << rit.Get() << " ";
        std::cout << std::endl;
    }


    std::cout << std::endl;
    std::cout << Array.GetCapacity() << std::endl;
    std::cout << Array.GetSize() << std::endl;

    return 0;
}
