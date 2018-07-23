//
//  Rays.hpp
//  RayTracer
//
//  Created by macbook on 18/07/2018.
//  Copyright © 2018 hobby. All rights reserved.
//

#ifndef Rays_hpp
#define Rays_hpp

#include "Framebuffer.hpp"
#include "Geometric.hpp"
#include "Random.hpp"
#include "Math.hpp"

#include <numeric>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <cmath>


/* * * * * * * * * * * * * * * * * * * * * * *
 *  Rays
 *
 *  single header based cpu ray
 *  tracing implementation
 *
 * * * * * * * * * * * * * * * * * * * * * * */
struct Rays
    { // Rays
    
    std::vector<std::vector<Ray>> rays;
    
    Framebuffer framebuffer;
    
    uint32_t samples = 0;
    uint32_t height  = 0;
    uint32_t width   = 0;
    
    std::vector<Sphere> spheres = {
        { { 0.0f,  000.0f, 1.0f }, 000.5f },
        { { 0.0f, -100.5f, 1.0f }, 100.0f }};
        
    std::default_random_engine rng;
    struct Distributions {
        std::uniform_real_distribution<real> antialiasing { -1.0f, 1.0f };
        std::uniform_real_distribution<real> uniform      {  0.0f, 1.0f };
    } distributions;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *  Rays
     *
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    Rays (uint32_t _width, uint32_t _height, uint32_t _samples):
            framebuffer (_width, _height),
            rays        (_height),
            samples     (_samples),
            height      (_height),
            width       (_width)
        { // Rays :: Rays
        
        for (std::vector<Ray>& r : rays)
            r.resize(width);
        
        } // Rays :: Rays

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *  fire
     *
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    vec3 fire (uint32_t x, uint32_t y)
        { // Rays :: fire

        Intersection closest;
        closest.t = 1000.0f;

        for (uint32_t s = 0; s < spheres.size(); ++s)
            { // for each sphere
            Intersection hit = intersect(rays[y][x], spheres[s]);
            if (hit.t < closest.t && hit.t != -1.0f)
                closest = hit;
            } // for each sphere

        if (closest.t < 1000.0f)
            { // ray hit something
            
            vec3 dir = closest.p + closest.n + Random::unitSphere();
    
            rays[y][x].p = closest.p;
            rays[y][x].d = closest.p - dir;

            return 0.5 * fire (x, y);
            } // ray hit something

        else
            { // ray missed everything
            return { 1.0f, 1.0f, 1.0f };
            } // ray missed everything

        } // Rays :: cast
        
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *  render
     *
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    void render ()
        { // Rays :: render
        
        for (int32_t y = height - 1; y >= 0; --y)
            for (int32_t x = 0; x < width; ++x)
                { // for each pixel
                
                vec3 color = { 0.0f, 0.0f, 0.0f };
                
                for (uint32_t s = 0; s < samples; ++s)
                    { // for each sample
                    real uoffset = distributions.antialiasing (rng);
                    real voffset = distributions.antialiasing (rng);
                    
                    real u = -1.0f + (((real(x + uoffset)) / real(width)) * 2.0);
                    real v =  1.0f - (((real(y + voffset)) / real(height)) * 2.0);
                    
                    rays[y][x].p = vec3 { 0.0f, 0.0f, -1.0f };
                    rays[y][x].d = (vec3 { 0.0f, 0.0f, 1.0f } - vec3 { u, v, 0.0f });
                    
                    color = color + fire (x, y);
                    
                    } // for each sample

                framebuffer.setPixel(x, y, color / (real)samples);
                
                } // for each pixel
        
        framebuffer.writeToPPM("ppm/serial/" + std::to_string(width) + "x" + std::to_string(height) + ".ppm");
        } // Rays :: render

    }; // Rays

#endif /* Rays_hpp */
