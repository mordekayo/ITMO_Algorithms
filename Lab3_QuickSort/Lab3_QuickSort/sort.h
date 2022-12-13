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
            return;
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
template<typename T, typename Compare>
T* pPivotSelection(T* First, T* Last, Compare Comp)
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
    return Medium;
}

template<typename T, typename Compare>
T* pPartition(T* First, T* Last, Compare Comp)
{
    T* FirstCopy = First;
    T* LastCopy = Last;
    T Pivot = pPivotSelection(FirstCopy, LastCopy, Comp);
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
void pQuickSort(T* First, T* Last, Compare Comp)
{
    T* p;
    while (Last - First > 0)
    {
        p = Partition(First, Last, Comp);

        if((p - First) < (Last - p + 1))
        {
            pQuickSort(First, p, Comp);
            First = p + 1;
        }
        else
        {
            pQuickSort(p + 1, Last, Comp);
            Last = p;
        }
    }
}