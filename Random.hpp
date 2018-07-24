//
//  Random.hpp
//  RayTracer
//
//  Created by macbook on 23/07/2018.
//  Copyright © 2018 hobby. All rights reserved.
//

#ifndef Random_hpp
#define Random_hpp

#include "Math.hpp"
#include <random>

static std::default_random_engine rng;

struct Random
    {  // Random
    
    static vec3 unitSphere ()
        { // Rays :: randomUnitSphere
        std::uniform_real_distribution<real> dist (-1.0f, 1.0f);
        vec3 candidate { };
        do  { // while candidate is not in unit sphere
            candidate = 2.0f * vec3 { dist(rng), dist(rng), dist(rng) };
            } // while candidate is not in unit sphere
        while (dot(candidate, candidate) >= 1.0f);
        return candidate;
        } // Rays :: randomUnitSphere
    
    }; // Random

#endif /* Random_hpp */
