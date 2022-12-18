#pragma once
#include <algorithm>
#include <cstdlib>
#include <cstdlib>
#include <utility>

template<typename T>
class DynamicArray final
{

public:
    
    DynamicArray();
    DynamicArray(int NewCapacity);
    ~DynamicArray();

    DynamicArray(const DynamicArray& Rhs);

    DynamicArray(DynamicArray&& Rhs) noexcept;
    
    DynamicArray& operator=(const DynamicArray& Rhs);
    DynamicArray& operator=(DynamicArray&& Rhs) noexcept;
    
    int Insert(const T& NewItem);
    int Insert(int Index, const T& NewItem);

    void Remove(int Index);
    
    bool ArrayPointerIsNullptr();
    
    const T& operator[](int Index) const;
    T& operator[](int Index);

    int GetSize() const;
    int GetCapacity() const;
    
    bool Reserve(int NewCapacity);
    void ShrinkToFit();

    class Iterator
    {
    public:
        Iterator(int NewIndex, DynamicArray* pNewArray, bool bIsReversed);
        const T& Get() const;
        void Set(const T& Value);
        void Next();
        bool HasNext() const;
        Iterator& operator++();
        Iterator & operator++(int Сrutch);

    private:
        
        int Index;
        DynamicArray* pArray;
        int Direction;
    };

    class ConstIterator
    {
    public:
        ConstIterator(int NewIndex, DynamicArray* pNewArray, bool bIsReversed);
        const T& Get() const;
        void Next();
        bool HasNext() const;
        ConstIterator& operator++();
        ConstIterator & operator++(int Сrutch);

    private:
        
        int Index;
        DynamicArray* pArray;
        int Direction;
    };
    
    Iterator GetIterator();
    ConstIterator GetConstIterator() const;
    Iterator GetReverseIterator();
    ConstIterator GetConstReverseIterator() const;
    Iterator Begin();
    ConstIterator cBegin() const;

    Iterator End();
    ConstIterator cEnd() const;

private:

    int Size;
    T* pData;
    int Capacity = 16;
    double ExpansionFactor = 2.0f;
};

//** ARRAY **//
template <typename T>
DynamicArray<T>::DynamicArray()
{
    Size = 0;
    pData = static_cast<T*>(malloc(Capacity * sizeof(T)));
}

template <typename T>
DynamicArray<T>::DynamicArray(int NewCapacity)
{
    Size = 0;
    Capacity = NewCapacity;
    pData = static_cast<T*>(malloc(Capacity * sizeof(T)));
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    for(int i = 0; i < Size; ++i)
    {
        pData[i].~T();
    }
    free(pData);
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& rhs)
{
    pData = static_cast<T*>(malloc(rhs.Capacity * sizeof(T)));
    for(int i = 0; i < rhs.Size; ++i)
    {
        new(pData + i)T(rhs[i]);
    }
    Size = rhs.Size;
    Capacity = rhs.Capacity;
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray&& rhs) noexcept
{
    pData = rhs.pData;
    Size = rhs.Size;
    Capacity = rhs.Capacity;
    rhs.pData = nullptr;
    rhs.Size = 0;
    rhs.Capacity = 0;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& Rhs)
{
    DynamicArray TempArray(Rhs);
    std::swap(pData, TempArray.pData);
    std::swap(Size, TempArray.Size);
    std::swap(Capacity, TempArray.Capacity);
    return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& Rhs) noexcept
{
    if (this == &Rhs)
    {
        return *this;
    }
    for (int i = 0; i < Size; i++)
    {
        pData[i].~T();
    }
    free(pData);
    Capacity = Rhs.Capacity;
    Size = Rhs.Size;
    pData = Rhs.pData;
    Rhs.pData = nullptr;
    Rhs.Capacity = 0;
    Rhs.Size = 0;
    return *this;
}

template <typename T>
int DynamicArray<T>::Insert(const T& NewItem)
{
    if(Size + 1 > Capacity)
    {
        if(Capacity == 0)
        {
            Capacity = 1;
        }
        Capacity = static_cast<int>(Capacity * ExpansionFactor);
        T* pTemp = static_cast<T*>(malloc(Capacity * sizeof(T)));
        
        for(int i = 0; i < Size; ++i)
        {
            new(pTemp + i) T(std::move(pData[i]));
            pData[i].~T();
        }
        
        free(pData);
        pData = pTemp;
    }

    new(pData + Size) T(NewItem);
    Size += 1;

    return Size - 1;
}

template <typename T>
int DynamicArray<T>::Insert(int Index, const T& NewItem)
{
    if(Index >= Size || Index < 0)
    {
        throw std::exception("Insertion index is out of array");
    }

    if(Size + 1 > Capacity)
    {
        if(Capacity == 0)
        {
            Capacity = 1;
        }
        Capacity *= static_cast<int>(static_cast<float>(Capacity) * ExpansionFactor);
        T* pTemp = static_cast<T*>(malloc(Capacity * sizeof(T)));
        
        for(int i = 0; i < Index; ++i)
        {
            new(pTemp + i) T(std::move(pData[i]));
            pData[i].~T();
        }

        new(pTemp + Index)T(NewItem);
        
        for(int i = Index + 1; i < Size + 1; ++i)
        {
            new(pTemp + i) T(std::move(pData[i - 1]));
            pData[i - 1].~T();
        }
        
        free(pData);
        pData = pTemp;
    }
    else
    {
        for(int i = Size; i > Index; --i)
        {
            new(pData + i) T(std::move(pData[i - 1]));
            pData[i-1].~T();
        }
        new(pData + Index) T(std::move(NewItem));
    }
   
    Size += 1;

    return Size - 1;
}

template <typename T>
void DynamicArray<T>::Remove(int Index)
{
    if(Index >= Size || Index < 0)
    {
        throw std::exception("Remove index is out of array");
    }
    
    for(int i = Index; i < Size -1; ++i)
    {
        pData[i].~T();
        new(pData + i) T(std::move(pData[i + 1]));
    }

    pData[Size - 1].~T();

    Size -= 1;
}

template <typename T>
bool DynamicArray<T>::ArrayPointerIsNullptr()
{
    return pData ? false : true;
}

template <typename T>
const T& DynamicArray<T>::operator[](int Index) const
{
    if(Index >= Size || Index < 0)
    {
        throw std::exception("Dereference index is out of array");
    }
    return pData[Index];
}

template <typename T>
T& DynamicArray<T>::operator[](int Index)
{
    if(Index >= Size || Index < 0)
    {
        throw std::exception("Dereference index is out of array");
    }
    return pData[Index];
}

template <typename T>
int DynamicArray<T>::GetSize() const
{
    return Size;
}

template <typename T>
int DynamicArray<T>::GetCapacity() const
{
    return Capacity;
}

template <typename T>
bool DynamicArray<T>::Reserve(int NewCapacity)
{
    if(NewCapacity <= Capacity)
    {
        return false;
    }
    Capacity = NewCapacity;
    return true;
}

template <typename T>
void DynamicArray<T>::ShrinkToFit()
{
    Capacity = Size;
    T* pTemp = malloc(Capacity * sizeof(T));
    free(pData);
    pData = pTemp;
}

//** ITERATOR**//
template <typename T>
DynamicArray<T>::Iterator::Iterator(int NewIndex, DynamicArray<T>* pNewArray, bool bIsReversed)
{
    Index = NewIndex;
    pArray = pNewArray;
    Direction = bIsReversed ? -1 : 1;
}

template <typename T>
const T& DynamicArray<T>::Iterator::Get() const
{
    return (*pArray)[Index];
}

template <typename T>
void DynamicArray<T>::Iterator::Set(const T& Value)
{
    (*pArray)[Index] = Value;
}

template <typename T>
void DynamicArray<T>::Iterator::Next()
{
    Index += Direction;
}

template <typename T>
bool DynamicArray<T>::Iterator::HasNext() const
{
    return (Index + Direction >= 0) && (Index + Direction < pArray->GetSize());
}

template <typename T>
typename DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator++()
{
    Next();
    return *this;
}

template <typename T>
typename DynamicArray<T>::Iterator& DynamicArray<T>::Iterator::operator++(int Сrutch)
{
    Iterator Temp(*this);
    Next();
    return Temp;
}

//** CONST ITERATOR**//
template <typename T>
DynamicArray<T>::ConstIterator::ConstIterator(int NewIndex, DynamicArray* pNewArray, bool bIsReversed)
{
    Index = NewIndex;
    pArray = pNewArray;
    Direction = bIsReversed ? -1 : 1;
}

template <typename T>
const T& DynamicArray<T>::ConstIterator::Get() const
{
    return (*pArray)[Index];
}

template <typename T>
void DynamicArray<T>::ConstIterator::Next()
{
    Index += Direction;
}

template <typename T>
bool DynamicArray<T>::ConstIterator::HasNext() const
{
    return (Index + Direction >= 0) && (Index + Direction < pArray->GetSize());
}

template <typename T>
typename DynamicArray<T>::ConstIterator& DynamicArray<T>::ConstIterator::operator++()
{
    Next();
    return *this;
}

template <typename T>
typename DynamicArray<T>::ConstIterator& DynamicArray<T>::ConstIterator::operator++(int Сrutch)
{
    ConstIterator Temp(*this);
    Next();
    return Temp;
}

template <typename T>
typename DynamicArray<T>::Iterator DynamicArray<T>::GetIterator()
{
    return Iterator(0, this, false);
}

template <typename T>
typename DynamicArray<T>::ConstIterator DynamicArray<T>::GetConstIterator() const
{
    return ConstIterator(0, this, false);
}

template <typename T>
typename DynamicArray<T>::Iterator DynamicArray<T>::GetReverseIterator()
{
    return Iterator(Size - 1, this, true);
}

template <typename T>
typename DynamicArray<T>::ConstIterator DynamicArray<T>::GetConstReverseIterator() const
{
    return ConstIterator(Size - 1, this, true);
}

template <typename T>
typename DynamicArray<T>::Iterator DynamicArray<T>::Begin()
{
    return GetIterator();
}

template <typename T>
typename DynamicArray<T>::ConstIterator DynamicArray<T>::cBegin() const
{
    return GetConstIterator();
}

template <typename T>
typename DynamicArray<T>::Iterator DynamicArray<T>::End()
{
    return Iterator(pData + Size - 1, Size - 1, this, false);
}

template <typename T>
typename DynamicArray<T>::ConstIterator DynamicArray<T>::cEnd() const
{
    return ConstIterator(pData + Size - 1, Size - 1, this, false);
}
