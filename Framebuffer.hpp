//
//  Framebuffer.hpp
//  RayTracer
//
//  Created by macbook on 23/07/2018.
//  Copyright © 2018 hobby. All rights reserved.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include "Math.hpp"
#include <fstream>
#include <vector>
#include <limits>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Image Output
 *
 *  This will effectively represent our framebuffer, allowing
 *  contigious floating point pixel data to be read by any
 *  rendering front end (ppm / OpenGL / etc.)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct Framebuffer
    {  // Framebuffer Struct
    std::vector<vec3> pixel;
    std::vector<real> depth;
    
    const uint32_t width  = 0;
    const uint32_t height = 0;

    Framebuffer (uint32_t _width, uint32_t _height) :
            width  (_width),
            height (_height)
        { // Framebuffer :: Framebuffer
        
        pixel.resize(width * height);
        depth.resize(width * height);

        for (uint32_t y = 0; y < height; ++y)
            for (uint32_t x = 0; x < width; ++x)
                { // for each pixel

                setPixel (x, y, vec3 { 0.12f, 0.12f, 0.12f });
                setDepth (x, y, std::numeric_limits<real>::max());

                } // for each pixel
            
        } // Framebuffer :: Framebuffer
        
    /**
     *  setPixel
     *
     *  assigns the given rgb value to the pixel at (x, y)
     */
    void setPixel (uint32_t x, uint32_t y, vec3 color)
        { // Buffers :: setPixel
        pixel[x + y * width] = color;
        } // Buffers :: setPixel
    
    /**
     *  setDepth
     *
     *  assigns the given scalar depth value to the pixel at (x, y)
     */
    void setDepth (uint32_t x, uint32_t y, real z)
        { // Buffers :: setDepth
        depth[x + y * width] = z;
        } // Buffers :: setDepth

    /**
     *  getPixel
     *
     *  assigns the given rgb value to the pixel at (x, y)
     */
    vec3 getPixel (uint32_t x, uint32_t y)
        { // Buffers :: setPixel
        return pixel[x + y * width];
        } // Buffers :: setPixel
    
    /**
     *  getDepth
     *
     *  assigns the given scalar depth value to the pixel at (x, y)
     */
    real getDepth (uint32_t x, uint32_t y)
        { // Buffers :: setDepth
        return depth[x + y * width];
        } // Buffers :: setDepth

    void writeToPPM (const std::string& path)
        { // Rays :: writeToPPM
        
        std::ofstream output (path.c_str());
        
        output << "P3\n";
        output << "# Rays 2018\n";
        output << width << " " << height << "\n";
        output << "255\n";

        for (int32_t y = height - 1; y >= 0; y--)
            for (int32_t x = 0; x < width; x++)
                { // cap pixel values
                vec3 color = getPixel (x, y);
                color[0] = fmin (color[0], 1.0f);
                color[1] = fmin (color[1], 1.0f);
                color[2] = fmin (color[2], 1.0f);
                setPixel (x, y, color);
                } // cap pixel values
            
        for (int32_t y = height - 1; y >= 0; y--)
            for (int32_t x = 0; x < width; x++)
                { // write data to disk
                // pixel values mapped from 0.0 ... 1.0 to 0 ... 255
                vec3 color = getPixel (x, y);
                output << int(255.99 * ((color[0]))) << " "
                       << int(255.99 * ((color[1]))) << " "
                       << int(255.99 * ((color[2]))) << "\n";
                } // write data to disk
            
        output.close();
        
        } // Rays :: writeToPPM
    
    }; // Framebuffer Struct

#endif /* Framebuffer_hpp */
