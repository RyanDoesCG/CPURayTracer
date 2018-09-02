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
#include <thread>
#include <atomic>
#include <bitset>

#include <numeric>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <vector>
#include <cmath>

struct Rays;

static void worker (uint32_t id, int32_t xStart, int32_t xStop, int32_t yStart, int32_t yStop, Rays& tracer);

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

    std::atomic<uint32_t> pixelsDone;
    uint32_t pixelsToDo;
    
    std::vector<bool> status = { 0, 0, 0, 0 };

    struct Distributions {
        std::uniform_real_distribution<real> antialiasing { -1.000f, 1.000f };
        std::uniform_real_distribution<real> uniform      {  0.000f, 1.000f };
        std::uniform_real_distribution<real> x            { -0.800f, 0.800f };
        std::uniform_real_distribution<real> r            {  0.032f, 0.085f };
        std::uniform_real_distribution<real> e            {  0.000f, 1.000f };
    } distributions;
    
    struct Camera {
        vec3 position = {{ 0.0f, -0.25f, -0.5f }};
        vec3 look     = {{ 0.0f,  0.0f,  1.0f }};
        vec3 ray (real u, real v)
            { return look - vec3 { u, v, 0.0f }; }
    } camera;
    
    struct Scene {
        std::vector<Geometry*> geometry;
        std::vector<Material*> material;
    } scene;

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

        scene.geometry = {
            // Room Geometry
            new Plane { { 0.0f, -0.5f, 0.0f }, { 0.0f, -1.0f, 0.0f }, 0 },
            new Plane { { 1.2f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },   0 },
            new Plane { { -1.2f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, 0 },
            new Plane { { 0.0f, 0.0f, 1.5f }, { 0.0f, 0.0f, 1.0f },   0 },
            new Plane { { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, 0 },
            new Plane { { 0.0f, 1.5f, 0.0f }, { 0.0f, 1.0f, 0.0f },   0 },
            
            new Mesh  { "models/tracing_bust.mesh", 0 },
            
             new Sphere { {   0.2f, -0.5f + 0.08f, 0.1f }, 0.08f,  0 },
             new Sphere { {  -0.2f, -0.5f + 0.08f, 0.1f }, 0.08f,  0 },
             

			 new Sphere{ { 0.0f, 0.75, -0.6f }, 0.16f,  0 }
            
            /*
            // Spheres and Demo Geometry
            new Sphere { {  0.64f, -0.5 + 0.32f, 0.88 }, 0.32f,  0 },  // left light source
            new Sphere { {  0.35f, -0.5 + 0.16f, 0.58 }, 0.16f,  0 },  // left light source
            new Sphere { { -0.35f, -0.5 + 0.08f, 0.58 }, 0.08f,  0 },  // right light source
            new Sphere { { -0.05f, -0.5 + 0.08f, 0.48 }, 0.08f,  0 },
            new Sphere { { 0.125f, -0.5 + 0.06f, 0.38 }, 0.06f,  0 },
            new Sphere { { -0.2f, -0.5 + 0.06f, 0.24 }, 0.06f,   0 },
            new Sphere { { -0.24f, -0.5 + 0.02f, 0.14 }, 0.02f,  0 },
            new Sphere { { -0.16f, -0.5 + 0.02f, 0.14 }, 0.02f,  0 },
            new Sphere { { -0.2f, -0.5 + 0.018f, 0.11 }, 0.016f, 0 },
             */
            
            //new Mesh   { "NA", 0 }
            };
            
            uint32_t id = 0;
            for (Geometry* geometry : scene.geometry)
                geometry->id = id++;

        scene.material = {
            // Room Materials
            new Material {{ 0.64f, 0.64f, 0.64f }, 0.6f, 3.0f, 0.6f, 0.25f},
            new Material {{ 0.64f, 0.64f, 0.64f }, 0.6f, 3.0f, 0.6f, 0.25f},
            new Material {{ 0.64f, 0.64f, 0.64f }, 0.6f, 3.0f, 0.6f, 0.25f},
            new Material {{ 0.64f, 0.64f, 0.64f }, 0.6f, 3.0f, 0.6f, 0.25f},
            new Material {{ 0.64f, 0.64f, 0.64f }, 0.6f, 3.0f, 0.6f, 0.25f},
            new Material {{ 0.64f, 0.64f, 0.64f }, 0.6f, 3.0f, 0.6f, 0.25f},
            
            new Material { { 0.84f, 0.84f, 0.84f }, 0.8f, 2.5f, 0.4f, 0.25f },
            
			new Material { { 1.0f, 0.48823f, 0.0f },  0.4f, 3.0f, 0.24f, 1.0f },
			new Material { { 0.0f, 1.0f, 0.68823f },  0.4f, 3.0f, 0.24f, 1.0f },

			new Material{ { 1.0f, 1.0f, 1.0f },  0.4f, 3.0f, 0.24f, 0.8f },
            
            /*
            // Sphere and Demo Materials
            new Material { Random::color(),  0.3f, 3.0f, 0.24f, 0.8f },
            new Material { Random::color(),  4.0f, 0.0f, 0.24f, 0.5f },
            new Material { Random::color(),  2.0f, 0.0f, 1.0f, 0.5f },
            new Material { Random::color(),  1.0f, 0.0f, 0.0f, 0.5f },
            new Material { Random::color(),  1.0f, 0.0f, 0.06f, 0.5f },
            new Material { { 0.42, 0.42, 0.42 }, 0.3f, 3.0f, 0.01f, 0.5f },
            new Material { vec3{ 0.24, 0.24, 0.24} + Random::color(),  2.0f, 0.0f, 0.16f, 0.64f },
            new Material { vec3{ 0.24, 0.24, 0.24} + Random::color(),  2.0f, 0.0f, 0.16f, 0.64f },
            new Material { vec3{ 0.24, 0.24, 0.24} + Random::color(),  2.0f, 0.0f, 0.16f, 0.64f },
             */
        
            //new Material { vec3{ 0.24, 0.24, 0.24} + Random::color(),  2.0f, 0.0f, 0.16f, 0.64f }
            };
        

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
        for (Geometry* object : scene.geometry)
            { // for each sphere
            Intersection hit = object->intersect(rays[y][x]);
            if (hit.t < closest.t && hit.t >= 0.01f)
                closest = hit;
            } // for each sphere

        // if a hit was detected, fire some additional rays
        // through the scene for global light sampling
        if (closest.t < 1000.0f)
            { // ray hit something
            
            vec3 incoming = -rays[y][x].d;
            
            // roughness normal distortion
            closest.n[0] += distributions.antialiasing(Random::rng) * scene.material[closest.id]->roughness;
            closest.n[1] += distributions.antialiasing(Random::rng) * scene.material[closest.id]->roughness;
            closest.n[2] += distributions.antialiasing(Random::rng) * scene.material[closest.id]->roughness;
            
            // random diffuse probe
            rays[y][x].p = closest.p;
            rays[y][x].d = closest.p - (closest.p + closest.n + Random::unitSphere());
            vec3 diffuse =
                scene.material[closest.id]->diffuse *
                scene.material[closest.id]->albedo *
                fire (x, y, depth - 1);
                
            // reflected matallic probe
            rays[y][x].p = closest.p;
            rays[y][x].d = closest.p - (closest.p + closest.n + reflect (incoming, closest.n));
            vec3 metallic =
                scene.material[closest.id]->metallic *
                scene.material[closest.id]->albedo *
                fire (x, y, depth - 1);

            return scene.material[closest.id]->emissive * (diffuse + metallic);
            
            } // ray hit something

        // if no hit was detected, we just return the
        // scenes background color that acts as a luminaire
        return { 0.84f, 0.84f, 0.84f };

        } // Rays :: cast
        
    void report ()
        { // Rays :: report
        float done = pixelsDone.load() / (float)pixelsToDo;
        std::cout << done << std::endl;
        } // Rays :: report
        
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *  render
     *
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    void render ()
        { // Rays :: render
        
        pixelsDone.store(0);
        pixelsToDo = width * height;
        
        std::vector<std::thread> pool (8);
        
		int slice = height / 8;
		int base = height - 1;

        pool[0] = std::thread(worker, 0, 0, width - 1, base - (slice * 0), base - (slice * 1) + 1, std::ref(*this));
        pool[1] = std::thread(worker, 1, 0, width - 1, base - (slice * 1), base - (slice * 2) + 1, std::ref(*this));
        
        pool[2] = std::thread(worker, 2, 0, width - 1, base - (slice * 2), base - (slice * 3) + 1, std::ref(*this));
        pool[3] = std::thread(worker, 3, 0, width - 1, base - (slice * 3), base - (slice * 4) + 1, std::ref(*this));

		pool[4] = std::thread(worker, 0, 0, width - 1, base - (slice * 4), base - (slice * 5) + 1, std::ref(*this));
		pool[5] = std::thread(worker, 1, 0, width - 1, base - (slice * 5), base - (slice * 6) + 1, std::ref(*this));

		pool[6] = std::thread(worker, 2, 0, width - 1, base - (slice * 6), base - (slice * 7) + 1, std::ref(*this));
		pool[7] = std::thread(worker, 3, 0, width - 1, base - (slice * 7), 0, std::ref(*this));

        pool[0].join();
        pool[1].join();
        pool[2].join();
        pool[3].join();

		pool[4].join();
		pool[5].join();
		pool[6].join();
		pool[7].join();

        framebuffer.writeToPPM(std::to_string(rand()) + ".ppm");
        
        } // Rays :: render

    }; // Rays

void worker (uint32_t id, int32_t xStart, int32_t xStop, int32_t yStart, int32_t yStop, Rays& tracer)
    { // worker
    
    tracer.status[id] = true;
    
    for (int32_t y = yStart; y >= yStop; --y)
        for (int32_t x = xStart; x <= xStop; ++x)
            { // for each pixel in my workload
                
            vec3 color = { 0.0f, 0.0f, 0.0f };
            
            for (uint32_t s = 0; s < tracer.samples; ++s)
                { // for each sample
                real uoffset = tracer.distributions.antialiasing (Random::rng);
                real voffset = tracer.distributions.antialiasing (Random::rng);
                real u = -1.0f + (((real(x + uoffset)) / real(tracer.width))  * 2.0);
                real v =  1.0f - (((real(y + voffset)) / real(tracer.height)) * 2.0);
                tracer.rays[y][x].p = tracer.camera.position;
                tracer.rays[y][x].d = tracer.camera.ray(u, v);
                color = color + tracer.fire (x, y, tracer.bounces);
                } // for each sample

            tracer.framebuffer.setPixel(x, y, color / (real)tracer.samples);
            tracer.pixelsDone++;
            tracer.report();

            } // for each pixel in my workload
    
    tracer.status[id] = false;
    
    } // worker

#endif /* Rays_hpp */
