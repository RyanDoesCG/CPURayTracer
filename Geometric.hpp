//
//  Geometric.hpp
//  RayTracer
//
//  Created by macbook on 22/07/2018.
//  Copyright © 2018 hobby. All rights reserved.
//

#ifndef Geometric_hpp
#define Geometric_hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Geometric Defintions / Intersection
 *
 *  Here are the geometric intersection test methods for rays
 *  against pieces of the scene
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 #include "Math.hpp"

struct Ray
    {  // Ray
    vec3 p; // origin
    vec3 d; // direction
    inline vec3 trace (real t) const
        { // Ray :: trace
        return p + (d * t);
        } // Ray :: trace
    }; // Ray

struct Sphere
    {  // Sphere
    vec3 p; // center
    real r; // radius
    }; // Sphere

struct Triangle
    {  // Triangle
    vec3 a; // corner 1
    vec3 b; // corner 2
    vec3 c; // corner 3
    }; // Triangle

struct Intersection
    {  // Intersection
    real t; // distance along ray
    vec3 p; // point in 3D space
    vec3 n; // normal of collision
    }; // Intersection

Intersection intersect (const Ray& ray, const Sphere& sphere)
    { // Ray -> Sphere intersection
    
    vec3 o = ray.p - sphere.p;
    real a = dot (ray.d, ray.d);
    real b = 2.0f * dot (o, ray.d);
    real c = dot (o, o) - sphere.r * sphere.r;
    real d = b * b - 4 * a * c;
    real t = 0.0f;
    
    if (d > 0.0f)
        {
        t = (-b - sqrt(d)) / (2.0f * a);
        if (t > 0.0f && t < 1000.0f)
            {
            vec3 p = ray.trace (t);
            vec3 n = normalise(sphere.p - p);
            return { t, p, n };
            }
        
        t = (-b + sqrt(b * b - a * c)) / a;
        if (t > 0.0f && t < 1000.0f)
            {
            vec3 p = ray.trace (t);
            vec3 n = normalise(sphere.p - p);
            return { t, p, n };
            }

        }
    
    return { -1.0, {}, {}};

    } // Ray -> Sphere intersection

Intersection intersect (const Ray& ray, const Triangle& triangle)
    { // Ray -> Triangle intersection
    
    return {};
    } // Ray -> Triangle intersection

#endif /* Geometric_hpp */
