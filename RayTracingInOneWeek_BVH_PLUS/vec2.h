#pragma once
#ifndef VEC2_H
#define VEC2_H

#include "vec3.h"

using Vector3f = vec3;

class Vector2f
{
public:
    Vector2f() : x(0), y(0) {}
    Vector2f(float xx) : x(xx), y(xx) {}
    Vector2f(float xx, float yy) : x(xx), y(yy) {}
    Vector2f operator * (const float& r) const { return Vector2f(x * r, y * r); }
    Vector2f operator + (const Vector2f& v) const { return Vector2f(x + v.x, y + v.y); }
    float x, y;
};

using vec2 = Vector2f;

#endif // !VEC2_H
