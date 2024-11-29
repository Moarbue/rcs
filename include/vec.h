#ifndef _VEC_H_
#define _VEC_H_

typedef struct {
    float x, y, z;
} Vec3;

Vec3 vec3(float x, float y, float z);
Vec3 vec3s(float s);

float vec3_dot(Vec3 a, Vec3 b);
float vec3_mag(Vec3 v);
float vec3_mag_squared(Vec3 v);
float vec3_distance(Vec3 a, Vec3 b);
float vec3_distance_squared(Vec3 a, Vec3 b);

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_adds(Vec3 v, float s);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_subs(Vec3 v, float s);
Vec3 vec3_mul(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 v, float s);
Vec3 vec3_div(Vec3 a, Vec3 b);
Vec3 vec3_divs(Vec3 v, float s);

void vec3_negate(Vec3 *v);
Vec3 vec3_negate_to(Vec3 v);
void vec3_normalize(Vec3 *v);
Vec3 vec3_normalize_to(Vec3 v);
Vec3 vec3_cross(Vec3 a, Vec3 b);
Vec3 vec3_crossn(Vec3 a, Vec3 b);

Vec3 vec3_from_spherical(float radius, float theta, float phi);
Vec3 vec3_to_spherical(Vec3 v);

Vec3 vec3_lerp(Vec3 a, Vec3 b, float t);

#endif // _VEC_H_