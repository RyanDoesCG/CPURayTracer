//
//  main.cpp
//  RayTracer
//
//  Created by macbook on 17/07/2018.
//  Copyright © 2018 hobby. All rights reserved.
//
#include "Rays.hpp"
#include <vector>
#include <chrono>
#include <thread>

std::vector<uint32_t> mipchain;
std::vector<std::thread> threads;

long seed = 0;

void run (uint32_t size)
    { 
    Rays tracer (size, size, 64, 2);
    tracer.render();
    }

int main(int argc, const char * argv[])
    { // main

    srand(8);
    seed = rand();
    Random::rng.seed(seed);

    mipchain = { 128, 256, 512 };

    for (uint32_t id = 0; id < mipchain.size(); ++id)
        {
        threads.push_back(std::thread(run, mipchain[id]));
        }

    for (std::thread& t : threads)
        t.join();

    return 0;
    } // main
