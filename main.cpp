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

//  Results
//
//  Serial Run
//      1177
//      4571
//      17964
//      71082
//      322239
//
//  Concurrent Attempt 1
//
//
//
//
//
//

void run (uint32_t size)
    {
    Rays tracer (size, size, 128, 2);
    tracer.render();
    }

int main(int argc, const char * argv[])
    { // main
    
    /*
    std::vector<std::thread> threads;
    std::vector<uint32_t> resolutions = { 32, 64, 128, 256, 512, 1080 };
    srand(time(0));
    long seed = rand();
    for (uint32_t size : resolutions)
        {
        Random::rng.seed(seed);
        run (size);
        }
    for (std::thread& thread : threads)
        thread.join();
     */
     
    //run (64);
    
    uint32_t nThreads = 8;
    std::vector<std::thread> threads;

    srand(time(0));
    long seed = rand();
    for (uint32_t id = 0; id < nThreads; ++id)
        {
        Random::rng.seed(seed);
        threads.push_back(std::thread(run, 1500));
        }
        
    for (uint32_t id = 0; id < nThreads; ++id)
        threads[id].join();

    return 0;
    } // main
