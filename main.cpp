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

//  Results
//
//  Serial Run
//      784
//      3196
//      13039
//      48916
//      218173
//
//  Concurrent Attempt 1
//
//
//
//
//
//

int main(int argc, const char * argv[])
    { // main
    
    std::vector<uint32_t> resolutions = { 64, 128, 256, 512, 1080 };
    
    for (uint32_t i = 0; i < resolutions.size(); ++i)
        {
        auto start = std::chrono::system_clock::now();
        Rays tracer (resolutions[i], resolutions[i], 128);
        tracer.render();
        auto stop = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << diff.count() << '\n';
        }
  
    return 0;
    } // main
