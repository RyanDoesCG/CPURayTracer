//
//  Math.hpp
//  RayTracer
//
//  Created by macbook on 22/07/2018.
//  Copyright © 2018 hobby. All rights reserved.
//

#ifndef Math_hpp
#define Math_hpp

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Linear Mathematics Implementations
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 #include <array>
 #include <cmath>

using real = float;
using vec2 = std::array<real, 2>;
using vec3 = std::array<real, 3>;
using vec4 = std::array<real, 4>;
using mat2 = std::array<std::array<real, 2>, 2>;
using mat3 = std::array<std::array<real, 3>, 3>;
using mat4 = std::array<std::array<real, 4>, 4>;

/** addition */
inline vec4 operator + (const vec4& lhs, const vec4& rhs) { return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] }; }
inline vec3 operator + (const vec3& lhs, const vec3& rhs) { return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2] }; }
inline vec2 operator + (const vec2& lhs, const vec2& rhs) { return { lhs[0] + rhs[0], lhs[1] + rhs[1] }; }

/** subtraction */
inline vec4 operator - (const vec4& lhs, const vec4& rhs) { return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] }; }
inline vec3 operator - (const vec3& lhs, const vec3& rhs) { return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2] }; }
inline vec2 operator - (const vec2& lhs, const vec2& rhs) { return { lhs[0] - rhs[0], lhs[1] - rhs[1] }; }

inline vec3 operator - (const vec3& v) { return { v[0] * -1.0f, v[1] * -1.0f, v[2] * -1.0f }; }

/** multiplication */
inline vec4 operator * (const vec4& lhs, const vec4& rhs) { return { lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2], lhs[3] * rhs[3] }; }
inline vec3 operator * (const vec3& lhs, const vec3& rhs) { return { lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2] }; }
inline vec3 operator * (const vec3& lhs, const real  rhs) { return { lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs }; }
inline vec3 operator * (const real  lhs, const vec3& rhs) { return { lhs * rhs[0], lhs * rhs[1], lhs * rhs[2] }; }
inline vec2 operator * (const vec2& lhs, const vec2& rhs) { return { lhs[0] * rhs[0], lhs[1] * rhs[1] }; }
inline vec2 operator * (const real  lhs, const vec2& rhs) { return { lhs * rhs[0], lhs * rhs[1] }; }
inline vec2 operator * (const vec2& lhs, const real  rhs) { return { lhs[0] * rhs, lhs[1] * rhs }; }

/** division */
inline vec4 operator / (const vec4& lhs, const vec4& rhs) { return { lhs[0] / rhs[0], lhs[1] / rhs[1], lhs[2] / rhs[2], lhs[3] / rhs[3] }; }
inline vec3 operator / (const vec3& lhs, const vec3& rhs) { return { lhs[0] / rhs[0], lhs[1] / rhs[1], lhs[2] / rhs[2] }; }
inline vec2 operator / (const vec2& lhs, const vec2& rhs) { return { lhs[0] / rhs[0], lhs[1] / rhs[1] }; }
inline vec3 operator / (const vec3& lhs, const real  rhs) { return { lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs }; }
inline vec3 operator / (const real  lhs, const vec3& rhs) { return { lhs / rhs[0], lhs / rhs[1], lhs / rhs[2] }; }

/** dot products */
inline real dot (const vec4& lhs, const vec4& rhs) { return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3] * rhs[3]; }
inline real dot (const vec3& lhs, const vec3& rhs) { return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2]; }
inline real dot (const vec2& lhs, const vec2& rhs) { return lhs[0] * rhs[0] + lhs[1] * rhs[1]; }

/** cross product */
inline vec3 cross (const vec3& lhs, const vec3& rhs)
    { return { lhs[1] * rhs[2] - lhs[2] * rhs[1],
               lhs[2] * rhs[0] - lhs[0] * rhs[2],
               lhs[0] * rhs[1] - lhs[1] * rhs[0]};}

/** magnitude */
inline real length (const vec3& v)
    { return sqrt (dot (v, v)); }

/** normalization */
inline vec3 normalise (const vec3& v)
    { return v / length (v); }

/** fresnel reflection */
inline vec3 reflect (const vec3& v, const vec3& n)
    { return v - 2.0f * dot (v, n) * n; }


#endif /* Math_hpp */
