#ifndef _QUAT_H_
#define _QUAT_H_

#include "vec.h"
#include "mat.h"

// w is real part, x, y, z are imaginary part
typedef struct {
    float x, y, z, w;
} Quat;

Quat quat(float x, float y, float z, float w);
Quat quats(float s);
Quat quat_identity(void);
Quat quat_from_axis_angle(Vec3 axis, float angle);
Quat quat_from_vecs(Vec3 a, Vec3 b);
Quat quat_from_dir(Vec3 dir, Vec3 up);
Quat quat_from_euler_xyz(Vec3 angles);
Quat quat_from_mat4(Mat4 m);

float quat_mag(Quat q);
float quat_mag_squared(Quat q);
void quat_normalize(Quat *q);
Quat quat_normalize_to(Quat q);
Quat quat_conjugate(Quat q);

float quat_real(Quat q);
Vec3  quat_imag(Quat q);
Vec3  quat_imagn(Quat q);
float quat_imag_len(Quat q);

float quat_angle(Quat q);
Vec3  quat_axis(Quat q);

Mat4 quat_to_mat4(Quat q);
Mat4 quat_to_mat4t(Quat q);
Mat4 quat_to_look(Vec3 pos, Quat ori);
Vec3 quat_to_euler_xyz(Quat q);

float quat_dot(Quat a, Quat b);
Quat quat_add(Quat a, Quat b);
Quat quat_sub(Quat a, Quat b);
Quat quat_scale(Quat q, float s);
Quat quat_mul(Quat a, Quat b);
Vec3 quat_rotatev3(Vec3 v, Quat q);
void quat_rotatem4(Mat4 *m, Quat q);
void quat_rotatem4_at(Mat4 *m, Quat q, Vec3 pivot);

Quat quat_lerp(Quat a, Quat b, float t);
Quat quat_slerp(Quat a, Quat b, float t);

#endif // _QUAT_H_