#include "vec.h"
#include "smath.h"

#include <float.h>
#include <math.h>

// Create a Vec3 from x, y and z values
Vec3 vec3(float x, float y, float z)
{
    return (Vec3) {x, y, z};
}

// Create a Vec3 from a single value
Vec3 vec3s(float s)
{
    return (Vec3) {s, s, s};
}

// Calculate the dot-product of two Vec3s
float vec3_dot(Vec3 a, Vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Calculate the magnitude of a Vec3
float vec3_mag(Vec3 v)
{
    return sqrtf(vec3_mag_squared(v));
}

// Calculate the squared magnitude of a Vec3
float vec3_mag_squared(Vec3 v)
{
    return vec3_dot(v, v);
}

// Calculate the absolute distance between two Vec3s
float vec3_distance(Vec3 a, Vec3 b)
{
    return sqrtf(vec3_distance_squared(a, b));
}

// Calculate the square of the absolute distance between two Vec3s
float vec3_distance_squared(Vec3 a, Vec3 b)
{
    return pow2(a.x - b.x) + pow2(a.y - b.y) + pow2(a.z - b.z);
}

// Add two Vec3s together
Vec3 vec3_add(Vec3 a, Vec3 b)
{
    return vec3(
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    );
}

// Add a scalar to a Vec3
Vec3 vec3_adds(Vec3 v, float s)
{
    return vec3(
        v.x + s,
        v.y + s,
        v.z + s
    );
}

// Subtract two Vec3s, a - b
Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    return vec3(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    );
}

// Subtract a scalar from a Vec3
Vec3 vec3_subs(Vec3 v, float s)
{
    return vec3(
        v.x - s,
        v.y - s,
        v.z - s
    );
}

// Component-wise multiplication of two Vec3s
Vec3 vec3_mul(Vec3 a, Vec3 b)
{
    return vec3(
        a.x * b.x,
        a.y * b.y,
        a.z * b.z
    );
}

// Multiply a Vec3 by a scalar
Vec3 vec3_scale(Vec3 v, float s)
{
    return vec3(
        v.x * s,
        v.y * s,
        v.z * s
    );
}

// Component-wise division of two Vec3s
Vec3 vec3_div(Vec3 a, Vec3 b)
{
    return vec3(
        a.x / b.x,
        a.y / b.y,
        a.z / b.z
    );
}

// Divide a Vec3 by a scalar
Vec3 vec3_divs(Vec3 v, float s)
{
    return vec3(
        v.x / s,
        v.y / s,
        v.z / s
    );
}

// Negate a Vec3, res = -v
Vec3 vec3_negate(Vec3 v)
{
    return vec3(-v.x, -v.y, -v.z);
}

// Normalize a Vec3
void vec3_normalize(Vec3 *v)
{
    *v = vec3_normalize_to(*v);
}

// Normalize a Vec3 and return the result
Vec3 vec3_normalize_to(Vec3 v)
{
    float mag;
    
    mag = vec3_mag_squared(v);

    // vec is already normalized
    if (fabsf(mag - 1.0f) < EPS) return v;

    mag = sqrtf(mag);
    // vec has magnitude 0
    if (mag < EPS) return vec3s(0.0f);

    return vec3_scale(v, 1.0f / mag);
}

// Calculate the cross product of two Vec3s
Vec3 vec3_cross(Vec3 a, Vec3 b)
{
    return vec3(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

// Calculate the cross product of two Vec3s and normalize the result
Vec3 vec3_crossn(Vec3 a, Vec3 b)
{
    Vec3 v;

    v = vec3_cross(a, b);
    return vec3_normalize_to(v);
}

// Create a Vec3 from Spherical Coordinates
// radius = radius of sphere
// theta  = angle in radians from z-Axis
// phi    = angle in radians from x-Axis
// see https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
Vec3 vec3_from_spherical(float radius, float theta, float phi)
{
    return vec3(
        radius * sinf(theta) * cosf(phi),
        radius * sinf(theta) * sinf(phi),
        radius * cosf(theta)
    );
}

// Convert a Vec3 to Spherical Coordinates
// res.x = radius
// res.y = theta (angle in radians from z-Axis)
// res.z = phi   (angle in radians from x-Axis)
// see https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
Vec3 vec3_to_spherical(Vec3 v)
{
    float r, theta, phi;

    r = vec3_mag(v);
    if (fabs(r) < EPS) return vec3s(0.0f);

    theta = atan2(
        sqrtf(pow2(v.x) + pow2(v.y)),
        v.z
    );

    phi = atan2(v.y, v.x);

    return vec3(r, theta, phi);
}

// Linear interpolation from a to b
// a + s * (b - a)
Vec3 vec3_lerp(Vec3 a, Vec3 b, float t)
{
    Vec3 s, v;

    s = vec3s(t);
    v = vec3_sub(b, a);
    v = vec3_mul(s, v);

    return vec3_add(a, v);
}
