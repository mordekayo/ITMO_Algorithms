//
// pch.h
//

#pragma once

#include <chrono>

#include "gtest/gtest.h"
#include <random>
#include <string>
#include "../Lab3_QuickSort/sort.h"

std::string random_string();


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

    /*
    ComplexClass(const ComplexClass& rhs) = delete;
    ComplexClass& operator=(const ComplexClass& other) = delete;

    ComplexClass(const ComplexClass&& rhs) 
    {
        String = rhs.String;
        Integer = rhs.Integer;
    };
    ComplexClass& operator=(ComplexClass&& rhs) = default;

    */
    std::string String;
    int Integer;
};
