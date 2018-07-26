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
#include <iomanip>
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

    uint32_t progress = 0;

    struct Distributions {
        std::uniform_real_distribution<real> antialiasing { -1.0f, 1.0f };
        std::uniform_real_distribution<real> uniform      {  0.0f, 1.0f };
        std::uniform_real_distribution<real> x            { -0.8f, 0.8f };
        std::uniform_real_distribution<real> r            {  0.032, 0.085};
        std::uniform_real_distribution<real> e            { 0.0f, 1.0f };
    } distributions;
    
    std::vector<Geometry*> scene = {
    
        // Room Geometry
        new Plane { { 0.0f, -0.5f, 0.0f }, { 0.0f, -1.0f, 0.0f }, 0},
        new Plane { { 1.2f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },  1 },
        new Plane { { -1.2f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, 2 },
        new Plane { { 0.0f, 0.0f, 1.5f }, { 0.0f, 0.0f, 1.0f }, 3 },
        new Plane { { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, 4 },
        new Plane { { 0.0f, 1.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 5 },
    
        // Spheres and Demo Geometry
        new Sphere { {  0.64f, -0.5 + 0.32f, 0.88 }, 0.32f, 6 },  // left light source
      //  new Sphere { { -0.64f, -0.5 + 0.32f, 0.88 }, 0.32f, 7 },  // right light source
        new Sphere { {  0.0f, -0.5 + 0.28f, 0.98 }, 0.28f, 7 },   // back middle ball
        
        new Sphere { {  0.35f, -0.5 + 0.16f, 0.58 }, 0.16f, 8 },  // left light source
        new Sphere { { -0.35f, -0.5 + 0.08f, 0.58 }, 0.08f, 9 }, // right light source
        
        new Sphere { { -0.05f, -0.5 + 0.08f, 0.48 }, 0.08f, 10 },
        new Sphere { { 0.125f, -0.5 + 0.06f, 0.38 }, 0.06f, 11 },
        
        
        new Sphere { { -0.2f, -0.5 + 0.06f, 0.24 }, 0.06f, 12 },
        new Sphere { { -0.24f, -0.5 + 0.02f, 0.14 }, 0.02f, 13 },
        new Sphere { { -0.16f, -0.5 + 0.02f, 0.14 }, 0.02f, 14 },
        new Sphere { { -0.2f, -0.5 + 0.018f, 0.11 }, 0.016f, 15 }
    
        };
        
    std::vector<Material> materials = {
    
        // Room Materials
        {{ 0.36f, 0.36f, 0.36f }, 0.4f, 3.0f, 0.6f, 0.5f},
        {{ 0.36f, 0.36f, 0.36f }, 0.4f, 3.0f, 0.6f, 0.5f},
        {{ 0.36f, 0.36f, 0.36f }, 0.4f, 3.0f, 0.6f, 0.5f},
        {{ 0.36f, 0.36f, 0.36f }, 0.4f, 3.0f, 0.6f, 0.5f},
        {{ 0.36f, 0.36f, 0.36f }, 0.4f, 3.0f, 0.6f, 0.5f},
        {{ 0.36f, 0.36f, 0.36f }, 0.4f, 3.0f, 0.6f, 0.5f},

        // Sphere and Demo Materials
        { Random::color(),  0.3f, 3.0f, 0.24f, 0.8f },
    //    { Random::color(),  0.3f, 3.0f, 0.24f, 0.8f },
        { { 0.42, 0.42, 0.42 }, 0.3f, 3.0f, 0.0f, 0.5f },
        { Random::color(),  4.0f, 0.0f, 0.0f, 0.5f },
        { Random::color(),  2.0f, 0.0f, 1.0f, 0.5f },
        { Random::color(),  1.0f, 0.0f, 0.0f, 0.5f },
        { Random::color(),  1.0f, 0.0f, 0.06f, 0.5f },
        { { 0.42, 0.42, 0.42 }, 0.3f, 3.0f, 0.01f, 0.5f },
        
        // 2 more
        { vec3{ 0.24, 0.24, 0.24} + Random::color(),  2.0f, 0.0f, 0.16f, 0.64f },
        { vec3{ 0.24, 0.24, 0.24} + Random::color(),  2.0f, 0.0f, 0.16f, 0.64f },
        { vec3{ 0.24, 0.24, 0.24} + Random::color(),  2.0f, 0.0f, 0.16f, 0.64f }
        };
        

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
            width       (_width)
        { // Rays :: Rays
        
        for (std::vector<Ray>& r : rays)
            r.resize(width);
            
            
        // a rejection algorithm is used to generate some additional
        // spheres so that no intersecting spheres can be found
        std::vector<Sphere*> tempGeometry;
        std::vector<Material>  tempMaterial;
        bool intersectionFree = false;
        uint32_t offset = scene.size() - 1;
        while (!intersectionFree)
            { // while scene contains collisions
            
            // start by generating a set of spheres with random
            // sizes and positions
            uint32_t nSpheres = 0;/// + Random::integer();
            for (uint32_t id = 0; id < nSpheres; ++id)
                {
                // Generate Geometry
                real radius = distributions.r (Random::rng);
                real x = distributions.x(Random::rng);
                real z = 0.2 + distributions.uniform(Random::rng);
                vec3 position = { x, -0.5f + radius, z };
                tempGeometry.push_back(new Sphere { position, radius, offset + id});
                // Generate Material
                tempMaterial.push_back({
                    { distributions.uniform(Random::rng), distributions.uniform(Random::rng), distributions.uniform(Random::rng) },
                    distributions.e(Random::rng),
                    distributions.e(Random::rng),
                    distributions.uniform(Random::rng),
                    distributions.e (Random::rng)});
                }
            
            intersectionFree = true;
            
            // then run an intersection test on the spheres in
            // the local temporary buffer
            for (uint32_t i = 0; i < tempGeometry.size(); ++i)
                for (uint32_t j = 0; j < tempGeometry.size(); ++j)
                    { // for all pairs of geometry
                    if (i == j) continue;
                    
                    real diff = length((tempGeometry[i]->p) - (tempGeometry[j]->p));
                    real min  = tempGeometry[i]->r + tempGeometry[j]->r;
                    
                    if (diff <= min)
                        intersectionFree = false;
                    
                    } // for all pairs of geometry
            
            // finally, run an intersection test with the temporary
            // buffer and the world at large
            for (uint32_t i = 0; i < offset; ++i)
                { // for all static spheres
                
                Sphere* sphere = static_cast<Sphere*>(scene[i]);
                
                for (uint32_t j = 0; j < tempGeometry.size(); ++j)
                    { // for all temporary geometry
                    
                    real diff = length((sphere->p) - (tempGeometry[j]->p));
                    real min  = sphere->r + tempGeometry[j]->r;
                    
                    if (diff <= min)
                        intersectionFree = false;
                    
                    } // for all temporary geometry
                
                } // for all static spheres
            
            } // while scene contains collisions
            
        // once the candidate scene has passed the intersection test
        // it's data can be appended to the scene for rendering
        for (uint32_t i = 0; i < tempGeometry.size(); ++i)
            { // for each candidate geometry
            
            scene.push_back(static_cast<Sphere*>(tempGeometry[i]));
            materials.push_back(tempMaterial[i]);
            
            } // for each candidate geometry

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
        for (Geometry* object : scene)
            { // for each sphere
            Intersection hit = object->intersect(rays[y][x]);
            if (hit.t < closest.t && hit.t != -1.0f)
                closest = hit;
            } // for each sphere

        // if a hit was detected, fire some additional rays
        // through the scene for global light sampling
        if (closest.t < 1000.0f)
            { // ray hit something
            
            vec3 incoming = -rays[y][x].d;
            
            // roughness normal distortion
            closest.n[0] += distributions.antialiasing(Random::rng) * materials[closest.id].roughness;
            closest.n[1] += distributions.antialiasing(Random::rng) * materials[closest.id].roughness;
            closest.n[2] += distributions.antialiasing(Random::rng) * materials[closest.id].roughness;
            
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

            return materials[closest.id].emissive * (diffuse + metallic);
            
            } // ray hit something

        // if no hit was detected, we just return the
        // scenes background color that acts as a luminaire
        return { 0.84f, 0.84f, 0.84f };

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
                    real uoffset = distributions.antialiasing (Random::rng);
                    real voffset = distributions.antialiasing (Random::rng);
                    real u = -1.0f + (((real(x + uoffset)) / real(width)) * 2.0);
                    real v =  1.0f - (((real(y + voffset)) / real(height)) * 2.0);
                    rays[y][x].p = vec3 { 0.0f, 0.0f, -1.0f };
                    rays[y][x].d = vec3 { 0.0f, 0.0f, 1.0f } - vec3 { u, v, 0.0f };
                    color = color + fire (x, y, bounces);
                    } // for each sample

                framebuffer.setPixel(x, y, color / (real)samples);

                progress = (((x + (height - y) * width) / float(width * height)) * 100);
                std::cout << width << " : " << progress << std::endl;
                } // for each pixel
        
        framebuffer.writeToPPM("ppm/" + std::to_string(width) + ".ppm");
        } // Rays :: render

    }; // Rays

#endif /* Rays_hpp */
