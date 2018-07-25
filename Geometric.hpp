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

struct Intersection
    {  // Intersection
    real t;      // distance along ray
    vec3 p;      // point in 3D space
    vec3 n;      // normal of collision
    uint32_t id; // the intersected object
    }; // Intersection

struct Geometry
    {  // Abstract Geometry
    uint32_t id;
    Geometry (uint32_t _id): id (_id) {}
    virtual Intersection intersect (const Ray& ray)
        = 0;
    }; // Abstract Geometry

struct Sphere : public Geometry
    {  // Sphere
    vec3 p;      // center
    real r;      // radius
    
    Sphere (vec3 _p, real _r, uint32_t _id) :
        p (_p), r (_r), Geometry (_id) {}
    
    virtual Intersection intersect (const Ray& ray) override
        { // Sphere :: intersect
        
        vec3 o = ray.p - p;
        real a = dot (ray.d, ray.d);
        real b = 2.0f * dot (o, ray.d);
        real c = dot (o, o) - r * r;
        real d = b * b - 4 * a * c;
        real t = 0.0f;
        
        if (d > 0.0f)
            {
            t = (-b - sqrt(d)) / (2.0f * a);
            if (t > 0.0f && t < 1000.0f)
                {
                vec3 position = ray.trace (t);
                vec3 normal = normalise(p - position);
                return { t, position, normal, id};
                }
            
            t = (-b + sqrt(b * b - a * c)) / a;
            if (t > 0.0f && t < 1000.0f)
                {
                vec3 position = ray.trace (t);
                vec3 normal = normalise(p - position);
                return { t, position, normal, id };
                }
            }
        
        return { -1.0, {}, {}};
        
        } // Sphere :: intersect
    
    }; // Sphere

struct Plane : public Geometry
    {  // Plane
    vec3 p; // position
    vec3 n; // normal
    
    Plane (vec3 _p, vec3 _n, uint32_t _id) :
        p (_p), n (normalise(_n)), Geometry (_id) {}
    
    virtual Intersection intersect (const Ray& ray) override
        { // Sphere :: intersect
        
        real d = dot (n, ray.d);
        
        if (d > 0.0f)
            {
            real t = dot (p - ray.p, n) / d;
            vec3 position = ray.trace (t);
            vec3 normal = n;
            return { t, position, normal, id };
            
            }
        
        return { -1, {}, {} };
        
        } // Sphee :: intersect
    
    }; // Plane

#endif /* Geometric_hpp */
