#pragma once
#include <cstdlib>

template<typename T>
class Array final
{
    class Iterator
    {
    public:
        const T& Get() const;
        void Set(const T& value);
        void Next();
        bool HasNext() const;
        Iterator& operator++();
        Iterator & operator++(int Сrutch);

    private:

        T* pData;
    };

    class ConstIterator
    {
    public:
        const T& Get() const;
        void Next();
        bool HasNext() const;
        ConstIterator& operator++();
        ConstIterator & operator++(int Сrutch);

    private:
        
        T* pData;
    };
    
public:
    
    Array();
    Array(int capacity);
    ~Array();

    Array(const Array& rhs);

    Array(Array&& rhs) noexcept;
    
    Array& operator=(const Array& rhs);
    Array& operator=(Array&& rhs) noexcept;
 

    int Insert(const T& value);
    int Insert(int index, const T& value);

    void Remove(int index);

    const T& operator[](int index) const;
    T& operator[](int index);

    int GetSize() const;
    int GetCapacity() const;
    
    void Reserve();

    Iterator getIterator();
    ConstIterator getConstIterator() const;
    Iterator Begin();
    ConstIterator cBegin() const;

    Iterator End();
    ConstIterator cEnd() const;

private:

    int Size;
    T* pData;
    int Capacity = 16;
    float ExpansionFactor = 2.0f;
};

//** ITERATOR**//
template <typename T>
const T& Array<T>::Iterator::Get() const
{
    return pData;
}

template <typename T>
void Array<T>::Iterator::Set(const T& value)
{
    pData = value;
}

template <typename T>
void Array<T>::Iterator::Next()
{
    pData += 1;
}

template <typename T>
bool Array<T>::Iterator::HasNext() const
{
    return true;
}

template <typename T>
typename Array<T>::Iterator& Array<T>::Iterator::operator++()
{
    Next();
    return *this;
}

template <typename T>
typename Array<T>::Iterator& Array<T>::Iterator::operator++(int Сrutch)
{
    Iterator Temp(*this);
    Next();
    return Temp;
}

//** CONST ITERATOR**//
template <typename T>
const T& Array<T>::ConstIterator::Get() const
{
    return pData;
}

template <typename T>
void Array<T>::ConstIterator::Next()
{
    pData += 1;
}

template <typename T>
bool Array<T>::ConstIterator::HasNext() const
{
    return true;
}

template <typename T>
typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator++()
{
    Next();
    return *this;
}

template <typename T>
typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator++(int Сrutch)
{
    ConstIterator Temp(*this);
    Next();
    return Temp;
}

template <typename T>
Array<T>::Array()
{
    Size = 0;
    Capacity = 0;
    pData = nullptr;
}

template <typename T>
Array<T>::Array(int NewCapacity)
{
    Size = 0;
    Capacity = NewCapacity;
    pData = nullptr;
}

template <typename T>
Array<T>::~Array()
{
    
}

template <typename T>
Array<T>::Array(const Array& rhs)
{
    
}

template <typename T>
Array<T>::Array(Array&& rhs) noexcept
{
    
}

template <typename T>
Array<T>& Array<T>::operator=(const Array& rhs)
{
    
}

template <typename T>
Array<T>& Array<T>::operator=(Array&& rhs) noexcept
{
    
}

template <typename T>
int Array<T>::Insert(const T& value)
{
    T* pTemp;

    if(Size != 0)
    {
        pTemp = malloc((Size + 1) * sizeof(T));
        for(int i = 0; i < Size; ++i)
        {
            new(pTemp + i) T(pData[i]);
        }
        for(int i = 0; i < Size; ++i)
        {
            pData[i].~T();
        }
        free(pData);
    }
    new(pData + Size) T(value);
    Size += 1;

    return Size - 1;
}

template <typename T>
int Array<T>::Insert(int index, const T& value)
{

}

template <typename T>
void Array<T>::Remove(int index)
{
    
}

template <typename T>
const T& Array<T>::operator[](int index) const
{
    
}

template <typename T>
T& Array<T>::operator[](int index)
{
    
}

template <typename T>
int Array<T>::GetSize() const
{
    return Size;
}

template <typename T>
int Array<T>::GetCapacity() const
{
    return Capacity;
}

template <typename T>
void Array<T>::Reserve()
{
    
}
