#include "pch.h"
#include <random>
#include <chrono>

#define ARRAY_SIZE 1000

TEST(StdSortTest, IntSorting) 
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> RandomIntGenerator(1,10);

	const auto Array = new int[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomIntGenerator(RandomEngine);
	}
	
	std::sort(Array, Array + ARRAY_SIZE - 1, [](int a, int b) {return a < b; });
	
	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(StdSortTest, FloatSorting) 
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> RandomFloatGenerator;

	const auto Array = new float[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomFloatGenerator(RandomEngine);
	}

	std::sort(Array, Array + ARRAY_SIZE - 1, [](float a, float b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(StdSortTest, StringSorting) 
{
	const auto Array = new std::string[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = random_string();
	}

	std::sort(Array, Array + ARRAY_SIZE - 1, [](const std::string& a, const std::string& b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(StdSortTest, ComplexClassSorting) 
{
	const auto Array = new ComplexClass[ARRAY_SIZE];
	
	std::sort(Array, Array + ARRAY_SIZE - 1, [](const ComplexClass& a, const ComplexClass& b) {return a.Integer < b.Integer; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k].Integer, Array[k + 1].Integer);
	}
}

TEST(pQuickSortTest, IntSorting) 
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> RandomIntGenerator(1,10);

	const auto Array = new int[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomIntGenerator(RandomEngine);
	}
	
	pQuickSort(Array, Array + ARRAY_SIZE - 1, [](int a, int b) {return a < b; });
	
	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(pQuickSortTest, FloatSorting) 
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> RandomFloatGenerator;

	const auto Array = new float[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomFloatGenerator(RandomEngine);
	}

	pQuickSort(Array, Array + ARRAY_SIZE - 1, [](float a, float b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(pQuickSortTest, StringSorting) 
{
	const auto Array = new std::string[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = random_string();
	}

	pQuickSort(Array, Array + ARRAY_SIZE - 1, [](const std::string& a, const std::string& b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(pQuickSortTest, ComplexClassSorting) 
{
	const auto Array = new ComplexClass[ARRAY_SIZE];
	
	pQuickSort(Array, Array + ARRAY_SIZE - 1, [](const ComplexClass& a, const ComplexClass& b) {return a.Integer < b.Integer; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k].Integer, Array[k + 1].Integer);
	}
}

TEST(QuickSortTest, IntSorting) 
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> RandomIntGenerator(1,10);

	const auto Array = new int[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomIntGenerator(RandomEngine);
	}
	
	QuickSort(Array, Array + ARRAY_SIZE - 1, [](int a, int b) {return a < b; });
	
	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(QuickSortTest, FloatSorting) 
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> RandomFloatGenerator;

	const auto Array = new float[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomFloatGenerator(RandomEngine);
	}

	QuickSort(Array, Array + ARRAY_SIZE - 1, [](float a, float b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(QuickSortTest, StringSorting) 
{
	const auto Array = new std::string[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = random_string();
	}

	QuickSort(Array, Array + ARRAY_SIZE - 1, [](const std::string& a, const std::string& b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(InsertSortTest, IntSorting) 
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> RandomIntGenerator(1,10);

	const auto Array = new int[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomIntGenerator(RandomEngine);
	}
	
	InsertSort(Array, Array + ARRAY_SIZE - 1, [](int a, int b) {return a < b; });
	
	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(InsertSortTest, FloatSorting)
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> RandomFloatGenerator;

	const auto Array = new float[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] =  RandomFloatGenerator(RandomEngine); 
	}

	InsertSort(Array, Array + ARRAY_SIZE - 1, [](float a, float b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(InsertSortTest, StringSorting) 
{
	const auto Array = new std::string[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = random_string();
	}

	InsertSort(Array, Array + ARRAY_SIZE - 1, [](const std::string& a, const std::string& b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}


TEST(CombinedSortTest, FloatSorting)
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_real_distribution<float> RandomFloatGenerator;

	const auto Array = new float[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomFloatGenerator(RandomEngine);
	}

	CombinedSort(Array, Array + ARRAY_SIZE - 1, [](float a, float b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(CombinedSortTest, StringSorting) 
{
	const auto Array = new std::string[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = random_string();
	}

	CombinedSort(Array, Array + ARRAY_SIZE - 1, [](const std::string& a, const std::string& b) {return a < b; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}

TEST(CombinedSortTest, IntSorting) 
{
	std::default_random_engine RandomEngine(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> RandomIntGenerator(1,10);

	const auto Array = new int[ARRAY_SIZE];
	for (int k = 0; k < ARRAY_SIZE; ++k)
	{
		Array[k] = RandomIntGenerator(RandomEngine);
	}
	
	CombinedSort(Array, Array + ARRAY_SIZE - 1, [](int a, int b) {return a < b; });
	
	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k], Array[k + 1]);
	}
}


TEST(QuickSortTest, ComplexClassSorting) 
{
	const auto Array = new ComplexClass[ARRAY_SIZE];
	
	QuickSort(Array, Array + ARRAY_SIZE - 1, [](const ComplexClass& a, const ComplexClass& b) {return a.Integer < b.Integer; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k].Integer, Array[k + 1].Integer);
	}
}

TEST(InsertSortTest, ComplexClassSorting) 
{
	const auto Array = new ComplexClass[ARRAY_SIZE];
	
	InsertSort(Array, Array + ARRAY_SIZE - 1, [](const ComplexClass& a, const ComplexClass& b) {return a.Integer < b.Integer; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k].Integer, Array[k + 1].Integer);
	}
}

TEST(CombinedSortTest, ComplexClassSorting)
{
	const auto Array = new ComplexClass[ARRAY_SIZE];
	
	CombinedSort(Array, Array + ARRAY_SIZE - 1, [](const ComplexClass& a, const ComplexClass& b) {return a.Integer < b.Integer; });

	for (int k = 0; k < ARRAY_SIZE - 2; ++k)
	{
		EXPECT_LE(Array[k].Integer, Array[k + 1].Integer);
	}
}