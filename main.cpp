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
     
    return 0;
    } // main
