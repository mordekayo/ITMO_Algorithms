#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include "sort.h"

void Benchmark()
{
    std::ofstream FileOutput;
    FileOutput.open("output.txt");
    FileOutput << "Size QuickSort InsertSort CombinedSort";
    FileOutput << std::endl;
    const int TestCount = 100000000;
    std::default_random_engine RandomEngine(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> RandomIntGenerator;
    for(int i = 2; i < 100; ++i)
    {
        FileOutput << i << " ";
        int* Array = new int[i];
        int* QuickSortArray = new int[i];
        int* InsertionSortArray = new int[i];
        int* CombinedSortArray = new int[i];
        std::chrono::duration<float> QuicksortRuntime = std::chrono::duration<float>(0);
        std::chrono::duration<float> InsertSortRuntime = std::chrono::duration<float>(0);
        std::chrono::duration<float> CombinedSortRuntime = std::chrono::duration<float>(0);

        for(int j = 0; j < TestCount; ++j)
        {
            for(int k = 0; k < i; ++k)
            {
                Array[k] = RandomIntGenerator(RandomEngine);
                QuickSortArray[k] = Array[k];
                InsertionSortArray[k] = Array[k];
                CombinedSortArray[k] = Array[k];
            }
            
            std::chrono::steady_clock::time_point QuickSortStart = std::chrono::steady_clock::now();
            QuickSort(QuickSortArray, QuickSortArray + i - 1, [] (int a, int b) {return a < b ;});
            std::chrono::steady_clock::time_point QuickSortEnd = std::chrono::steady_clock::now();
            QuicksortRuntime += QuickSortEnd - QuickSortStart;
            
            std::chrono::steady_clock::time_point InsertionSortStart = std::chrono::steady_clock::now();
            InsertSort(InsertionSortArray, InsertionSortArray + i - 1, [] (int a, int b) {return a < b ;});
            std::chrono::steady_clock::time_point InsertionSortEnd = std::chrono::steady_clock::now();
            InsertSortRuntime += InsertionSortEnd - InsertionSortStart;

            std::chrono::steady_clock::time_point CombinedSortStart = std::chrono::steady_clock::now();
            CombinedSort(CombinedSortArray, CombinedSortArray + i - 1, [] (int a, int b) {return a < b ;});
            std::chrono::steady_clock::time_point CombinedSortEnd = std::chrono::steady_clock::now();
            CombinedSortRuntime += CombinedSortEnd - CombinedSortStart;
        }

        const float QuickSortMedianTime = QuicksortRuntime.count();
        const float InsertSortMedianTime = InsertSortRuntime.count();
        const float CombinedSortMedianTime = CombinedSortRuntime.count();
        
        FileOutput << QuickSortMedianTime;
        FileOutput << " ";
        FileOutput << InsertSortMedianTime;
        FileOutput << " ";
        FileOutput << CombinedSortMedianTime;
        FileOutput << std::endl;
        std::cout << "Tests on array size " << i << " done." << std::endl;
    }
    FileOutput.close();
}

int main()
{
    Benchmark();
    
    return 0;
}