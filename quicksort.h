template<typename T, typename Compare>
void quicksort(T *first, T *last, Compare comp)
{
    int pivotIndex = 0;
    T* pFirst = first;
    T* pLast = last;
    T* pPivot = first + pivotIndex;
    while(true)
    {
        while(comp(*pFirst, *pPivot))
        {
            pFirst = pFirst + 1;
        }
        if (pFirst >= pLast)
        {
            break;
        }
        if(pFirst == first + pivotIndex)
        {
            pFirst = pFirst + 1;
        } 
        if(pLast == first + pivotIndex)
        {
            pLast = pLast - 1;
        } 
        std::swap(pFirst, pLast);
    }
    quicksort(first, first + i, comp);
    quicksort(first + i + 1, last, comp);
}


9 2 5 3 7 8 2