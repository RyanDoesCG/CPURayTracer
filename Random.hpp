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

struct Random
    {  // Random
    
    static std::default_random_engine rng;
    
    static vec3 unitSphere ()
        { // Random :: randomUnitSphere
        std::uniform_real_distribution<real> dist (-1.0f, 1.0f);
        vec3 candidate { };
        do  { // while candidate is not in unit sphere
            candidate = 2.0f * vec3 { dist(rng), dist(rng), dist(rng) };
            } // while candidate is not in unit sphere
        while (dot(candidate, candidate) >= 1.0f);
        return candidate;
        } // Random :: randomUnitSphere

	static vec3 unitHemisphere (vec3 n)
		{ // Random :: unitHemisphere
		std::uniform_real_distribution<real> dist(-1.0f, 1.0f);
		vec3 candidate{};
		do { // while candidate is not in unit sphere
			candidate = 2.0f * vec3{ dist(rng), dist(rng), dist(rng) };
		} // while candidate is not in unit sphere
		while ((dot(candidate, candidate) >= 1.0f) && (dot(candidate, n) < 0.0f));
		return candidate;
		} // Random :: unitHemisphere
        
    static vec3 color ()
        { // Random :: color
        
        std::uniform_real_distribution<real> distribution (0.0f, 1.0f);
        return { distribution (rng), distribution (rng), distribution (rng) };
        
        } // Random :: color
        
    static uint32_t integer ()
        { // Random :: integer
        
        std::uniform_int_distribution<uint32_t> distribution (0, 10);
        return distribution (rng);
        
        } // Random :: integer
    
    }; // Random

std::default_random_engine Random::rng = std::default_random_engine (time(0));

#endif /* Random_hpp */
