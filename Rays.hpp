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
#include "Shading.hpp"
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
    
    uint32_t bounces = 0;
    uint32_t samples = 0;
    uint32_t height  = 0;
    uint32_t width   = 0;
    
    std::default_random_engine rng;
    struct Distributions {
        std::uniform_real_distribution<real> antialiasing { -1.0f, 1.0f };
        std::uniform_real_distribution<real> uniform      {  0.0f, 1.0f };
    } distributions;
    
    std::vector<Sphere> spheres = {
        { {  1.15f, 000.25f, 1.5f  }, 000.75f, 0 },
        { { -0.2f,  000.0f,  1.25f }, 000.50f, 1 },
        { { -0.9f, -0.25f,   1.0f  }, 000.25f, 2 },
        
        { { -0.55f, -0.375f,  0.75f }, 00.125f, 3 },
        { { -0.2f, -0.431875,  0.45f }, 00.0625f, 4 },
        { {  0.15f, -0.375f,  0.75f }, 00.125f, 5 },
        
        { {  0.0f, -100.5f,  1.5f  }, 100.00f, 6 }};
        
    std::vector<Material> materials = {
        {{ distributions.uniform (rng), distributions.uniform (rng), distributions.uniform (rng) },   1.0f, 0.0f, 0.0f },
        {{ 0.44f, 0.44f, 0.44f }, 0.2f, 3.0f, 0.12f },
        {{ distributions.uniform (rng), distributions.uniform (rng), distributions.uniform (rng) }, 1.0f, 0.0f, 0.0f },
        
        {{ distributions.uniform (rng), distributions.uniform (rng), distributions.uniform (rng) }, 1.0f, 0.0f, 0.0f },
        {{ distributions.uniform (rng), distributions.uniform (rng), distributions.uniform (rng) }, 1.0f, 0.0f, 0.0f },
        {{ distributions.uniform (rng), distributions.uniform (rng), distributions.uniform (rng)}, 1.0f, 0.0f, 0.0f },
        
        {{ 0.22f, 0.22f, 0.22f }, 0.4f, 3.0f, 0.6f }};
        

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *  Rays
     *
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    Rays (uint32_t _width, uint32_t _height, uint32_t _samples, uint32_t _bounces):
            framebuffer (_width, _height),
            rays        (_height),
            bounces     (_bounces),
            samples     (_samples),
            height      (_height),
            width       (_width),
            rng         (time(NULL))
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
    vec3 fire (uint32_t x, uint32_t y, int32_t depth)
        { // Rays :: fire

        if (depth < 0) return { 1.0f, 1.0f, 1.0f };

        Intersection closest;
        closest.t = 1000.0f;
        
        // fire the ray through the scene and record the
        // nearest geometric intersection as the hit
        for (uint32_t s = 0; s < spheres.size(); ++s)
            { // for each sphere
            Intersection hit = intersect(rays[y][x], spheres[s]);
            if (hit.t < closest.t && hit.t != -1.0f)
                closest = hit;
            } // for each sphere

        // if a hit was detected, fire some additional rays
        // through the scene for global light sampling
        if (closest.t < 1000.0f)
            { // ray hit something
            
            vec3 incoming = -rays[y][x].d;
            
            // roughness normal distortion
            closest.n[0] += distributions.antialiasing(rng) * materials[closest.id].roughness;
            closest.n[1] += distributions.antialiasing(rng) * materials[closest.id].roughness;
            closest.n[2] += distributions.antialiasing(rng) * materials[closest.id].roughness;
            
            // random diffuse probe
            rays[y][x].p = closest.p;
            rays[y][x].d = closest.p - (closest.p + closest.n + Random::unitSphere());
            vec3 diffuse =
                materials[closest.id].diffuse *
                materials[closest.id].albedo *
                fire (x, y, depth - 1);
                
            // reflected matallic probe
            rays[y][x].p = closest.p;
            rays[y][x].d = closest.p - (closest.p + closest.n + reflect (incoming, closest.n));
            vec3 metallic =
                materials[closest.id].metallic *
                materials[closest.id].albedo *
                fire (x, y, depth - 1);
            
            return 0.5 * (diffuse + metallic);
            
            } // ray hit something

        // if no hit was detected, we just return the
        // scenes background color that acts as a luminaire
        return { 1.0f, 1.0f, 1.0f };

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
                    rays[y][x].d = vec3 { 0.0f, 0.0f, 1.0f } - vec3 { u, v, 0.0f };
                    color = color + fire (x, y, bounces);
                    } // for each sample

                framebuffer.setPixel(x, y, color / (real)samples);
                
                } // for each pixel
        
        framebuffer.writeToPPM("ppm/" + std::to_string(width) + "x" + std::to_string(height) + ".ppm");
        } // Rays :: render

    }; // Rays

#endif /* Rays_hpp */
