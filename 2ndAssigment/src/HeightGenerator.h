#pragma once
#include <cstdlib>
class HeightGenerator
{

private:
    const float AMPTITUDE = 70.0f;
    int seed;

    HeightGenerator(int seed)
    {
        this->seed = seed;
    }

    float getNoise(int x, int z)
    {
        srand(x * 2421 + z * 3242 + this->seed);
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
};