#include <chrono>
#include <random>

#include "gtest/gtest.h"
#include "../DynamicArray/DynamicArray.h"

#define ARRAY_SIZE 100

std::string random_string()
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(str.begin(), str.end(), generator);

    return str.substr(0, 32);    // assumes 32 < number of characters in str         
}

class ComplexClass
{
public:
    ComplexClass()
    {
        std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
        const std::uniform_int_distribution<int> RandomIntGenerator;
        String = random_string();
        Integer = RandomIntGenerator(RandomEngine);
    }
    ~ComplexClass()
    {
        
    }
    
    ComplexClass(const ComplexClass& rhs) = delete;
    ComplexClass& operator=(const ComplexClass& other) = delete;

    ComplexClass(const ComplexClass&& rhs) 
    {
        String = rhs.String;
        Integer = rhs.Integer;
    };
    ComplexClass& operator=(ComplexClass&& rhs) = default;
    
    std::string String;
    int Integer;
};

class ArrayTests : public testing::Test
{
protected:

    void SetUp() override
    {
        
    }
    
    template<typename T>
    void TestConstructor()
    {
        DynamicArray<T> Array;
        EXPECT_EQ(0, Array.GetSize());
        EXPECT_EQ(16, Array.GetCapacity());
    }

    template<typename T>
    void TestConstructorWithParams(int Capacity)
    {
        DynamicArray<T> Array(Capacity);
        EXPECT_EQ(0, Array.GetSize());
        EXPECT_EQ(Capacity, Array.GetCapacity());
    }
    
    template<typename T>
    void TestCopyConstructor(T* Items, int Capacity, int Size)
    {
        DynamicArray<T> ArrayA(Capacity);
        for (int i = 0; i < Size; ++i)
        {
            ArrayA.Insert(Items[i]);
        }
        DynamicArray<T> ArrayB(ArrayA);

        EXPECT_EQ(Size, ArrayA.GetSize());
        EXPECT_EQ(Size, ArrayB.GetSize());
        EXPECT_EQ(Capacity, ArrayA.GetCapacity());
        EXPECT_EQ(Capacity, ArrayB.GetCapacity());

        for (int i = 0; i < ArrayA.GetSize(); ++i)
        {
            EXPECT_EQ(ArrayA[i], ArrayB[i]);
            EXPECT_EQ(ArrayA[i], Items[i]);
        }

        for (int i = 0; i < Size; ++i)
        {
            ArrayA.Remove(0);
        }

        EXPECT_EQ(0, ArrayA.GetSize());
        EXPECT_EQ(Size, ArrayB.GetSize());

        for (int i = 0; i < ArrayA.GetSize(); ++i)
        {
            EXPECT_EQ(ArrayB[i], Items[i]);
        }
    }

    template<typename T>
    void TestMoveConstructor(T* Items, int Capacity, int Size)
    {
        DynamicArray<T> ArrayA(Capacity);
        for( int i = 0; i < Size; ++i)
        {
            ArrayA.Insert(Items[i]);
        }
        
        DynamicArray<T> ArrayB(std::move(ArrayA));
        
        EXPECT_EQ(Size, ArrayB.GetSize());
        EXPECT_EQ(Capacity, ArrayB.GetCapacity());

        for (int i = 0; i < ArrayB.GetSize(); ++i)
        {
            EXPECT_EQ(ArrayB[i], Items[i]);
        }
    }

    template<typename T>
    void TestCopyAssigment(T* AItems, int ACapacity, int ASize, T* BItems, int BCapacity, int BSize)
    {
        DynamicArray<T> ArrayA(ACapacity);
        for (int i = 0; i < ASize; ++i)
        {
            ArrayA.Insert(AItems[i]);
        }

        DynamicArray<T> ArrayB(BCapacity);
        for (int i = 0; i < BSize; ++i)
        {
            ArrayB.Insert(BItems[i]);
        }
        
        ArrayB = ArrayA;
        
        EXPECT_EQ(ASize, ArrayA.GetSize());
        EXPECT_EQ(ASize, ArrayB.GetSize());
        EXPECT_EQ(ACapacity, ArrayA.GetCapacity());
        EXPECT_EQ(ACapacity, ArrayB.GetCapacity());

        for (int i = 0; i < ArrayA.GetSize(); ++i)
        {
            EXPECT_EQ(ArrayA[i], ArrayB[i]);
            EXPECT_EQ(ArrayA[i], AItems[i]);
        }
        
        for (int i = 0; i < ASize; ++i)
        {
            ArrayA.Remove(0);
        }

        EXPECT_EQ(0, ArrayA.GetSize());
        EXPECT_EQ(ASize, ArrayB.GetSize());

        for (int i = 0; i < ArrayA.GetSize(); ++i)
        {
            EXPECT_EQ(ArrayB[i], AItems[i]);
        }
    }

    template<typename T>
    void TestMoveAssigment(T* AItems, int ACapacity, int ASize, T* BItems, int BCapacity, int BSize)
    {
        DynamicArray<T> ArrayA(ACapacity);
        for (int i = 0; i < ASize; ++i)
        {
            ArrayA.Insert(AItems[i]);
        }

        DynamicArray<T> ArrayB(BCapacity);
        for (int i = 0; i < BSize; ++i)
        {
            ArrayB.Insert(BItems[i]);
        }
        
        ArrayB = std::move(ArrayA);
        
        EXPECT_EQ(0, ArrayA.GetSize());
        EXPECT_EQ(ASize, ArrayB.GetSize());
        EXPECT_EQ(0, ArrayA.GetCapacity());
        EXPECT_EQ(ACapacity, ArrayB.GetCapacity());

        for (int i = 0; i < ASize; ++i)
        {
            EXPECT_EQ(AItems[i], ArrayB[i]);
        }
        
        EXPECT_EQ(true, ArrayA.ArrayPointerIsNullptr());
    }

    template<typename T>
    void TestInsert(T* Items, int Size, int Index, T Value)
    {
        DynamicArray<T> Array(Size);
        for (int i = 0; i < Size; ++i)
        {
            Array.Insert(Items[i]);
        }

        EXPECT_EQ(Size, Array.GetSize());
        EXPECT_EQ(Size, Array.GetCapacity());
        
        for (int i = 0; i < Size; ++i)
        {
            EXPECT_EQ(Items[i], Array[i]);
        }

        Array.Insert(Index, Value);

        for(int i = 0; i < Index; ++i)
        {
            EXPECT_EQ(Items[i], Array[i]);
        }

        EXPECT_EQ(Array[Index], Value);

        for(int i = Index + 1; i < Array.GetSize(); ++i)
        {
            EXPECT_EQ(Items[i - 1], Array[i]);
        }
    }

    template<typename T>
    void TestPushBack(T* Items, int Size, T Value)
    {
        DynamicArray<T> Array(Size + 1);
        for (int i = 0; i < Size; ++i)
        {
            Array.Insert(Items[i]);
        }

        EXPECT_EQ(Size, Array.GetSize());
        EXPECT_EQ(Size + 1, Array.GetCapacity());
        
        for (int i = 0; i < Size; ++i)
        {
            EXPECT_EQ(Items[i], Array[i]);
        }

        Array.Insert(Value);
        
        for(int i = 0; i < Size; ++i)
        {
            EXPECT_EQ(Items[i], Array[i]);
        }

        EXPECT_EQ(Array[Array.GetSize() - 1], Value);
    }

    template<typename T>
    void TestRemove(T* Items, int Size, int Index)
    {
        DynamicArray<T> Array(Size);
        for (int i = 0; i < Size; ++i)
        {
            Array.Insert(Items[i]);
        }

        EXPECT_EQ(Size, Array.GetSize());
        EXPECT_EQ(Size, Array.GetCapacity());
        
        for (int i = 0; i < Size; ++i)
        {
            EXPECT_EQ(Items[i], Array[i]);
        }

        Array.Remove(Index);

        EXPECT_EQ(Size - 1, Array.GetSize());

        for(int i = 0; i < Index; ++i)
        {
            EXPECT_EQ(Items[i], Array[i]);
        }

        for(int i = Index + 1; i < Size; ++i)
        {
            EXPECT_EQ(Items[i], Array[i - 1]);
        }
    }

    template<typename T>
    void TestInsertWithCapacityIncrease(T* Items, int Size, int Index, T Value)
    {
        DynamicArray<T> Array(0);
        for (int i = 0; i < Size; ++i)
        {
            Array.Insert(Items[i]);
        }

        EXPECT_EQ(Size, Array.GetSize());
        
        for (int i = 0; i < Size; ++i)
        {
            EXPECT_EQ(Items[i], Array[i]);
        }

        Array.Insert(Index, Value);

        for(int i = 0; i < Index; ++i)
        {
            EXPECT_EQ(Items[i], Array[i]);
        }

        EXPECT_EQ(Array[Index], Value);

        for(int i = Index + 1; i < Array.GetSize(); ++i)
        {
            EXPECT_EQ(Items[i - 1], Array[i]);
        }
    }

    template<typename T>
    void TestIterator(T* Items, int Size)
    {
        DynamicArray<T> Array(0);
        for (int i = 0; i < Size; ++i)
        {
            Array.Insert(Items[i]);
        }

        auto NewIterator = Array.GetIterator();

        for(int i = 0; i < Array.GetSize(); ++i, NewIterator.Next())
        {
            EXPECT_EQ(Array[i], NewIterator.Get());
        }
    }

    template<typename T>
    void TestReverseIterator(T* Items, int Size)
    {
        DynamicArray<T> Array(0);
        for (int i = 0; i < Size; ++i)
        {
            Array.Insert(Items[i]);
        }

        auto NewIterator = Array.GetReverseIterator();

        for(int i = Array.GetSize(); i > 0; --i, NewIterator.Next())
        {
            EXPECT_EQ(Array[i], NewIterator.Get());
        }
    }
};

TEST_F(ArrayTests, DefaultConstructor)
{
    TestConstructor<int>();
    TestConstructor<std::string>();
    TestConstructor<ComplexClass>();
}

TEST_F(ArrayTests, ConstructorWithParams)
{
    TestConstructorWithParams<int>(ARRAY_SIZE);
    TestConstructorWithParams<std::string>(ARRAY_SIZE);
    TestConstructorWithParams<std::string>(5);
    TestConstructorWithParams<ComplexClass>(ARRAY_SIZE);
}

TEST_F(ArrayTests, CopyConstructor)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArray = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArray[k] = RandomIntGenerator(RandomEngine);
    }
    TestCopyConstructor<int>(IntArray, ARRAY_SIZE, ARRAY_SIZE);

    const auto StringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = random_string();
    }
    TestCopyConstructor<std::string>(StringArray, ARRAY_SIZE, ARRAY_SIZE);

    const auto ShortStringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = "123";
    }
    TestCopyConstructor<std::string>(ShortStringArray, ARRAY_SIZE, ARRAY_SIZE);
}

TEST_F(ArrayTests, MoveConstructor)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArray = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArray[k] = RandomIntGenerator(RandomEngine);
    }
    TestMoveConstructor<int>(IntArray, ARRAY_SIZE, ARRAY_SIZE);

    const auto StringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = random_string();
    }
    TestMoveConstructor<std::string>(StringArray, ARRAY_SIZE, ARRAY_SIZE);

    const auto ShortStringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = "123";
    }
    TestMoveConstructor<std::string>(ShortStringArray, ARRAY_SIZE, ARRAY_SIZE);
}

TEST_F(ArrayTests, CopyAssigment)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArrayA = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArrayA[k] = RandomIntGenerator(RandomEngine);
    }
    const auto IntArrayB = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArrayB[k] = RandomIntGenerator(RandomEngine);
    }
    TestCopyAssigment<int>(IntArrayA, ARRAY_SIZE, ARRAY_SIZE , IntArrayB, ARRAY_SIZE + 1, ARRAY_SIZE + 1);

    
    const auto StringArrayA = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArrayA[k] = random_string();
    }
    const auto StringArrayB = new std::string[ARRAY_SIZE + 1];
    for (int k = 0; k < ARRAY_SIZE + 1; ++k)
    {
        StringArrayB[k] = random_string();
    }
    TestCopyAssigment<std::string>(StringArrayB, ARRAY_SIZE, ARRAY_SIZE, StringArrayB, ARRAY_SIZE + 1, ARRAY_SIZE + 1);

    const auto ShortStringArrayA = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        ShortStringArrayA[k] = "123";
    }
    const auto ShortStringArrayB = new std::string[ARRAY_SIZE + 1];
    for (int k = 0; k < ARRAY_SIZE + 1; ++k)
    {
        ShortStringArrayB[k] = "123";
    }
    TestCopyAssigment<std::string>(StringArrayB, ARRAY_SIZE, ARRAY_SIZE, StringArrayB, ARRAY_SIZE + 1, ARRAY_SIZE + 1);
}

TEST_F(ArrayTests, MoveAssigment)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArrayA = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArrayA[k] = RandomIntGenerator(RandomEngine);
    }
    const auto IntArrayB = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArrayB[k] = RandomIntGenerator(RandomEngine);
    }
    TestMoveAssigment<int>(IntArrayA, ARRAY_SIZE, ARRAY_SIZE , IntArrayB, ARRAY_SIZE + 1, ARRAY_SIZE + 1);

    
    const auto StringArrayA = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArrayA[k] = random_string();
    }
    const auto StringArrayB = new std::string[ARRAY_SIZE + 1];
    for (int k = 0; k < ARRAY_SIZE + 1; ++k)
    {
        StringArrayB[k] = random_string();
    }
    TestMoveAssigment<std::string>(StringArrayB, ARRAY_SIZE, ARRAY_SIZE, StringArrayB, ARRAY_SIZE + 1, ARRAY_SIZE + 1);

    const auto ShortStringArrayA = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        ShortStringArrayA[k] = "123";
    }
    const auto ShortStringArrayB = new std::string[ARRAY_SIZE + 1];
    for (int k = 0; k < ARRAY_SIZE + 1; ++k)
    {
        ShortStringArrayB[k] = "123";
    }
    TestMoveAssigment<std::string>(StringArrayB, ARRAY_SIZE, ARRAY_SIZE, StringArrayB, ARRAY_SIZE + 1, ARRAY_SIZE + 1);
}

TEST_F(ArrayTests, Insert)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArray = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArray[k] = RandomIntGenerator(RandomEngine);
    }
    TestInsert<int>(IntArray, ARRAY_SIZE, ARRAY_SIZE/2, RandomIntGenerator(RandomEngine));
    TestInsert<int>(IntArray, ARRAY_SIZE, 0, RandomIntGenerator(RandomEngine));
    TestPushBack<int>(IntArray, ARRAY_SIZE, RandomIntGenerator(RandomEngine));
    
    const auto StringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = random_string();
    }
    TestInsert<std::string>(StringArray, ARRAY_SIZE, ARRAY_SIZE/2, random_string());
    TestInsert<std::string>(StringArray, ARRAY_SIZE, 0, random_string());
    TestPushBack<std::string>(StringArray, ARRAY_SIZE, random_string());
    
    const auto ShortStringArrayA = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        ShortStringArrayA[k] = "123";
    }
    TestInsert<std::string>(ShortStringArrayA, ARRAY_SIZE, ARRAY_SIZE/2, "321");
    TestInsert<std::string>(ShortStringArrayA, ARRAY_SIZE, 0, "321");
    TestPushBack<std::string>(ShortStringArrayA, ARRAY_SIZE, "321");
}

TEST_F(ArrayTests, Remove)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArray = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArray[k] = RandomIntGenerator(RandomEngine);
    }
    TestRemove<int>(IntArray, ARRAY_SIZE, ARRAY_SIZE/2);
    TestRemove<int>(IntArray, ARRAY_SIZE, 0);
    TestRemove<int>(IntArray, ARRAY_SIZE, ARRAY_SIZE - 1);
    
    const auto StringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = random_string();
    }
    
    TestRemove<std::string>(StringArray, ARRAY_SIZE, ARRAY_SIZE / 2);
    TestRemove<std::string>(StringArray, ARRAY_SIZE, 0);
    TestRemove<std::string>(StringArray, ARRAY_SIZE, ARRAY_SIZE - 1);
    
    const auto ShortStringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        ShortStringArray[k] = "123";
    }
    TestRemove<std::string>(ShortStringArray, ARRAY_SIZE, ARRAY_SIZE/2);
    TestRemove<std::string>(ShortStringArray, ARRAY_SIZE, 0);
    TestRemove<std::string>(ShortStringArray, ARRAY_SIZE, ARRAY_SIZE - 1);
}

TEST_F(ArrayTests, InsertWithCapacityExchange)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArray = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArray[k] = RandomIntGenerator(RandomEngine);
    }
    TestInsertWithCapacityIncrease<int>(IntArray, ARRAY_SIZE, ARRAY_SIZE / 2,RandomIntGenerator(RandomEngine));
    TestInsertWithCapacityIncrease<int>(IntArray, ARRAY_SIZE, 0, RandomIntGenerator(RandomEngine));
    
    const auto StringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = random_string();
    }
    TestInsertWithCapacityIncrease<std::string>(StringArray, ARRAY_SIZE, ARRAY_SIZE/2, random_string());
    TestInsertWithCapacityIncrease<std::string>(StringArray, ARRAY_SIZE, 0, random_string());
    
    const auto ShortStringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        ShortStringArray[k] = "123";
    }
    TestInsertWithCapacityIncrease<std::string>(ShortStringArray, ARRAY_SIZE, ARRAY_SIZE/2, "321");
    TestInsertWithCapacityIncrease<std::string>(ShortStringArray, ARRAY_SIZE, 0, "321");
}

TEST_F(ArrayTests, Iterator)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArray = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArray[k] = RandomIntGenerator(RandomEngine);
    }
    TestIterator(IntArray, ARRAY_SIZE);
    
    const auto StringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = random_string();
    }
    TestIterator(StringArray, ARRAY_SIZE);
    
    const auto ShortStringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        ShortStringArray[k] = "123";
    }
    TestIterator(ShortStringArray, ARRAY_SIZE);
}

TEST_F(ArrayTests, ReverseIterator)
{
    std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> RandomIntGenerator;
    const auto IntArray = new int[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        IntArray[k] = RandomIntGenerator(RandomEngine);
    }
    TestReverseIterator(IntArray, ARRAY_SIZE);
    
    const auto StringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        StringArray[k] = random_string();
    }
    //TestReverseIterator(StringArray, ARRAY_SIZE);
    
    const auto ShortStringArray = new std::string[ARRAY_SIZE];
    for (int k = 0; k < ARRAY_SIZE; ++k)
    {
        ShortStringArray[k] = "123";
    }
    //TestReverseIterator(ShortStringArray, ARRAY_SIZE);
}
