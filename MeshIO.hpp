//
//  MeshIO.hpp
//  ModelValidator
//
//  Created by macbook on 20/06/2018.
//  Copyright © 2018 Hobby. All rights reserved.
//

#ifndef MeshIO_hpp
#define MeshIO_hpp

#include <algorithm>
#include <vector>

#include <iomanip>
#include <fstream>
#include <sstream>

#include <iostream>

struct Vertex
    {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uvs;
    uint32_t  id;
    
    
    bool operator==(const Vertex& other) const {
        return position == other.position && color == other.color && normal == other.normal && uvs == other.uvs;
    }
    };

struct MeshIO
    {
    
    //
    //  readMeshFile
    //
    //  populates the referenced arrays with the contents of the .mesh file
    //  found at the given path on disk
    //
    static void readMeshFile (const char* path, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
        { // MeshIO :: readMeshFile
        std::ifstream input (path, std::ios::binary);
        uint32_t v; input.read((char*)&v, sizeof(uint32_t)); vertices.resize(v);
        uint32_t f; input.read((char*)&f, sizeof(uint32_t)); indices.resize(f);
        input.read((char*)vertices.data(), sizeof(Vertex) * v);
        input.read((char*)indices.data(), sizeof(uint32_t) * f);
        input.close();
        } // MeshIO :: readMeshFile
        
    //
    //  writeMeshFile
    //
    //  creates a .mesh file of the given model at the given path
    //
    static void writeMeshFile (const char* path, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
        { // MeshIO :: writeMeshFile

        std::ofstream output (path, std::ios::binary);
        uint32_t v = static_cast<uint32_t>(vertices.size());
        uint32_t f = static_cast<uint32_t>(indices.size());
        output.write((char*)&v, sizeof(uint32_t));
        output.write((char*)&f, sizeof(uint32_t));
        output.write((char*)vertices.data(), sizeof(Vertex) * v);
        output.write((char*)indices.data(), sizeof(uint32_t) * f);
        output.close();
        
        } // MeshIO :: writeMeshFile
        
    //
    //  uses the method found in graphics gems to estimate a bounding
    //  sphere radius for the given mesh
    //
    static float estimateBounds (const std::vector<Vertex>& vertices)
        { // MeshIO :: estimateBounds
        
        float minX = std::numeric_limits<float>::max(); uint32_t minXIndex = 0;
        float maxX = std::numeric_limits<float>::min(); uint32_t maxXIndex = 0;
        float minY = std::numeric_limits<float>::max(); uint32_t minYIndex = 0;
        float maxY = std::numeric_limits<float>::min(); uint32_t maxYIndex = 0;
        float minZ = std::numeric_limits<float>::max(); uint32_t minZIndex = 0;
        float maxZ = std::numeric_limits<float>::min(); uint32_t maxZIndex = 0;
        
        vec3 centroid = { 0.0f, 0.0f, 0.0f };
        
        for (uint32_t v = 0; v < vertices.size(); ++v)
            { // for each vertex position
            
            if (vertices[v].position[0] < minX) { minX = vertices[v].position[0]; minXIndex = v; }
            if (vertices[v].position[0] > maxX) { maxX = vertices[v].position[0]; maxXIndex = v; }
            if (vertices[v].position[1] < minY) { minY = vertices[v].position[1]; minYIndex = v; }
            if (vertices[v].position[1] > maxY) { maxY = vertices[v].position[1]; maxYIndex = v; }
            if (vertices[v].position[2] < minZ) { minZ = vertices[v].position[2]; minZIndex = v; }
            if (vertices[v].position[2] > maxZ) { maxZ = vertices[v].position[2]; maxZIndex = v; }
            
            centroid = centroid + vertices[v].position;
            
            } // for each vertex position
            
        centroid = centroid / (float)vertices.size();
            
        /*
        assert(minX == vertices[minXIndex].position[0]);
        assert(minY == vertices[minYIndex].position[1]);
        assert(minZ == vertices[minZIndex].position[2]);
        assert(maxX == vertices[maxXIndex].position[0]);
        assert(maxY == vertices[maxYIndex].position[1]);
        assert(maxZ == vertices[maxZIndex].position[2]);
        */
        
        // we compute 3 pairs of vertices with the minimum and maximum
        // value of each dimension, giving us a vector across the extents
        // of the x, y and z axes
        std::vector<float> spans = {
                length(vertices[maxXIndex].position - vertices[minXIndex].position),
                length(vertices[maxYIndex].position - vertices[minYIndex].position),
                length(vertices[maxZIndex].position - vertices[minZIndex].position)};
            
        float r = *std::max_element(std::begin(spans), std::end(spans));
        
        // we then make a single pass through the vertices and expand the
        // sphere whenever we encounter a point outside of it
        for (uint32_t v = 0; v < vertices.size(); ++v)
            { // for each position
            
            float distance = length(vertices[v].position - centroid);
            if (distance > r)
                r += distance - r;
            
            } // for each position
        
        return r;
        
        } // MeshIO :: estimateBounds
        
    //
    //  returns the average of the vertex positions
    //
    static vec3 centroid (const std::vector<Vertex>& vertices)
        { // MeshIO :: centroid
        vec3 result = { 0.0f, 0.0f, 0.0f };
        for (const Vertex& v : vertices)
            result = result + v.position;
        return result / (float)vertices.size();
        } // MeshIO :: centroid
        
    //
    //  colours the vertices with the given r g b colour
    //
    inline static void paint (std::vector<Vertex>& vertices, float r, float g, float b)
        { // MeshIO :: paint
        
        for (Vertex& v : vertices) v.color = { r, g, b };

        } // MeshIO :: paint
        
    //
    //  colours the vertices with the given r g b colour
    //
    inline static void assign (std::vector<Vertex>& vertices, uint32_t id)
        { // MeshIO :: paint
        
        for (Vertex& v : vertices) v.id = id;

        } // MeshIO :: paint
        
    //
    //  moves the centroid of the mesh to (0, 0, 0)
    //
    inline static void center (std::vector<Vertex>& vertices)
        { // MeshIO :: center
        
        vec3 c = centroid (vertices);
        
        for (Vertex& v : vertices)
            v.position = v.position - c;
        
        } // MeshIO :: center
    
    };

#endif /* MeshIO_hpp */
