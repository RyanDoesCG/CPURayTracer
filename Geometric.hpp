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
#include "MeshIO.hpp"
#include "Math.hpp"
#include <fstream>
#include <sstream>

/* * * * * * * * * * * * * * * * * * * * *
 *  Ray
 *
 *  parametric form p + dt
 *
 * * * * * * * * * * * * * * * * * * * * */
struct Ray
    {  // Ray
    vec3 p; // origin
    vec3 d; // direction
    inline vec3 trace (real t) const
        { // Ray :: trace
        return p + (d * t);
        } // Ray :: trace
    }; // Ray

/* * * * * * * * * * * * * * * * * * * * *
 *  Intersection
 *
 * * * * * * * * * * * * * * * * * * * * */
struct Intersection
    {  // Intersection
    real t;      // distance along ray
    vec3 p;      // point in 3D space
    vec3 n;      // normal of collision
    uint32_t id; // the intersected object
    }; // Intersection

/* * * * * * * * * * * * * * * * * * * * *
 *  Geometry
 *
 * * * * * * * * * * * * * * * * * * * * */
struct Geometry
    {  // Abstract Geometry
    uint32_t id;
    Geometry (uint32_t _id): id (_id) {}
    virtual Intersection intersect (const Ray& ray)
        = 0;
    }; // Abstract Geometry

/* * * * * * * * * * * * * * * * * * * * *
 *  Material
 *
 * * * * * * * * * * * * * * * * * * * * */
struct Material
    {
    vec3 albedo;
    real diffuse;
    real metallic;
    real roughness;
    real emissive;
    };

/* * * * * * * * * * * * * * * * * * * * *
 *  Sphere
 *
 * * * * * * * * * * * * * * * * * * * * */
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

/* * * * * * * * * * * * * * * * * * * * *
 *  Plane
 *
 * * * * * * * * * * * * * * * * * * * * */
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

/* * * * * * * * * * * * * * * * * * * * *
 *  Mesh
 *
 * * * * * * * * * * * * * * * * * * * * */
struct Mesh : public Geometry
    {  // Mesh
    std::vector<Vertex>    vertices;
    std::vector<uint32_t> indices;

	Sphere approximation { vec3{0.0f, 0.0f, 0.0f}, 0.0f, 0 };

    Mesh (const char* path, uint32_t _id) :
        Geometry (_id)
        { // Mesh :: Mesh
        
        MeshIO::readMeshFile(path, vertices, indices);
        
        for (Vertex& vertex : vertices)
            {
            vertex.position[2] += 0.4f;
		//	vertex.position[1] += 0.1f;
            }

		float bounds = MeshIO::estimateBounds(vertices);
		vec3  center = MeshIO::centroid(vertices);

		approximation.p = center;
		approximation.r = bounds;
		
		/*
		
		

        vertices = {
            { 0.2f, 0.0f, 0.75f },
            { -0.2f, 0.0f, 0.75f },
            { 0.0f, 0.2f, 0.75f } };
        indices  = { 0,1,2 };
*/
        } // Mesh :: Mesh
        
    virtual Intersection intersect (const Ray& ray) override
        { // Mesh :: intersect

		if (approximation.intersect(ray).t == -1.0f)
			return { -1,{},{} };

        for (uint32_t f = 0; f < indices.size(); f += 3)
            { // for each face
            
            const vec3& a = vertices[indices[f + 0]].position;
            const vec3& b = vertices[indices[f + 1]].position;
            const vec3& c = vertices[indices[f + 2]].position;
            
            const vec3& u = b - a;
            const vec3& v = c - a;
            const vec3& n = cross (ray.d, v);
            
            const real area = dot (u, n);
            
            const vec3& s = ray.p - a;
            const vec3& r = cross (s, u);
            
            const real beta  = dot (s, n) / area;
            const real gamma = dot (ray.d, r) / area;
            const real alpha = 1.0f - (beta + gamma);
            
            const real d = dot (v, r) / area;

			const float eps1 = 1e-7f;
			const float eps2 = 1e-10;

			if (!((area  <=  eps1) || (alpha <  -eps2) || (beta  <  -eps2) || (gamma <  -eps2) || (d     <= 0.0f)))
                { // hit
                
                vec3 normal = { 0.0f, 0.0f, 0.0f };
        
				normal = normal + vertices[indices[f + 0]].normal * alpha;
				normal = normal + vertices[indices[f + 1]].normal * beta;
				normal = normal + vertices[indices[f + 2]].normal * gamma;

                return { d, ray.trace(d), normal, id };
               
				} // hit
                
            } // for each face
        
        return { -1, {}, {} };
        
        } // Mesh :: intersect

    }; // Mesh

#endif /* Geometric_hpp */
