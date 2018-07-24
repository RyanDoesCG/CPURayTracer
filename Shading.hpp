//
//  Shading.hpp
//  RayTracer
//
//  Created by macbook on 24/07/2018.
//  Copyright © 2018 hobby. All rights reserved.
//

#ifndef Shading_hpp
#define Shading_hpp

#include "Math.hpp"

struct Material
    {
    vec3 albedo;
    real diffuse;
    real metallic;
    real roughness;
    };

#endif /* Shading_hpp */
