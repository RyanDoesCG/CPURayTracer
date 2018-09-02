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
    Rays tracer (size, size, 64, 2 );
    tracer.render();
    }

int main(int argc, const char * argv[])
    { // main

    Random::rng.seed(time(nullptr));

    mipchain = { 256 };
	
	for (uint32_t mip : mipchain)
		run (mip);

    return 0;
    } // main
