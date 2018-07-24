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
    
    
    std::vector<std::thread> pool;
    std::vector<uint32_t> resolutions = { 32, 64, 128, 256, 512, 1080 };
    
    for (uint32_t size : resolutions)
        pool.push_back(std::thread(run, size));

    for (std::thread& thread : pool)
        thread.join();
     
    
    /*
    auto start = std::chrono::system_clock::now();
    Rays tracer (512, 512, 128);
    tracer.render();
    auto stop = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << diff.count() << '\n';
     */
     
    return 0;
    } // main
