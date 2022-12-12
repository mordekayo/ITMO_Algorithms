#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

#define Breakpoint 42

template<typename T, typename Compare>
T PivotSelection(T* First, T* Last, Compare Comp)
{
    T* Medium = First + (Last - First ) / 2;
    if (Comp(*Last, *First))
    {
        std::swap(*First, *Last);
    }
    if (Comp(*Medium, *First))
    {
        std::swap(*First, *Medium); 
    }
    if (Comp(*Last, *Medium))
    {
        std::swap(*Medium, *Last);
    }
    return *Medium;
}

template<typename T, typename Compare>
T* Partition(T* First, T* Last, Compare Comp)
{
    T* FirstCopy = First;
    T* LastCopy = Last;
    T Pivot = PivotSelection(FirstCopy, LastCopy, Comp);
    while(true)
    {
        while(Comp(*FirstCopy, Pivot))
        {
            ++FirstCopy;
        }
        while(Comp(Pivot, *LastCopy))
        {
            --LastCopy;
        }
        if (FirstCopy >= LastCopy)
        {
            return LastCopy;
        }
        std::swap(*FirstCopy, *LastCopy);
        ++FirstCopy;
        --LastCopy;
    }
}

template<typename T, typename Compare>
void QuickSort(T* First, T* Last, Compare Comp)
{
    T* p;
    while (Last - First > 0)
    {
        p = Partition(First, Last, Comp);

        if((p - First) < (Last - p + 1))
        {
            QuickSort(First, p, Comp);
            First = p + 1;
        }
        else
        {
            QuickSort(p + 1, Last, Comp);
            Last = p;
        }
    }
}

template<typename T, typename Compare>
void InsertSort(T* First, T* Last, Compare Comp)
{
    for (int i = 1; i < Last - First + 1; ++i)
    {
        int k = i;
        while (k > 0 && Comp(*(First + k), *(First + k - 1)))
        {
            std::swap(*(First + k - 1), *(First + k));
            k -= 1;
        }
    }
}

template<typename  T, typename  Compare>
void CombinedSort(T* First, T* Last, Compare Comp)
{
    T* p;
    while (Last - First > 0)
    {
        if(Last - First < Breakpoint)
        {
            InsertSort(First, Last, Comp);
        }
        
        p = Partition(First, Last, Comp);

        if((p - First) < (Last - p + 1))
        {
            CombinedSort(First, p, Comp);
            First = p + 1;
        }
        else
        {
            CombinedSort(p + 1, Last, Comp);
            Last = p;
        }
    }
}

int main()
{
    std::ofstream FileOutput;
    FileOutput.open("output.txt");
    FileOutput << "Size QuickSort InsertSort CombinedSort";
    FileOutput << std::endl;
    const int TestCount = 10000000;
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
            InsertSort(CombinedSortArray, CombinedSortArray + i - 1, [] (int a, int b) {return a < b ;});
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
    
    return 0;
}
