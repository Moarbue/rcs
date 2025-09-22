#ifndef _MAT_H_
#define _MAT_H_

#include "vec.h"

typedef struct {
    union {
        float e[4][4];
        float raw[16];
    };
} Mat4;

Mat4 mat4(void);
Mat4 mat4_copy(Mat4 m);
Mat4 mat4_fill(float val);

Mat4 mat4_mul(Mat4 a, Mat4 b);
Vec3 mat4_mulv3(Mat4 m, Vec3 v, float w);
void mat4_muls(Mat4 *m, float s);

void mat4_transpose(Mat4 *m);
Mat4 mat4_transpose_to(Mat4 m);

Mat4 mat4_translation(Vec3 v);
void mat4_translate(Mat4 *m, Vec3 v);
void mat4_translate_x(Mat4 *m, float x);
void mat4_translate_y(Mat4 *m, float y);
void mat4_translate_z(Mat4 *m, float z);

Mat4 mat4_rotation(Vec3 axis, float angle);
void mat4_rotate(Mat4 *m, Vec3 axis, float angle);
void mat4_rotate_at(Mat4 *m, Vec3 pivot, Vec3 axis, float angle);
void mat4_rotate_x(Mat4 *m, float angle);
void mat4_rotate_y(Mat4 *m, float angle);
void mat4_rotate_z(Mat4 *m, float angle);

Mat4 mat4_scaling(Vec3 v);
void mat4_scale(Mat4 *m, Vec3 v);
void mat4_scale_s(Mat4 *m, float s);
void mat4_scale_x(Mat4 *m, float x);
void mat4_scale_y(Mat4 *m, float y);
void mat4_scale_z(Mat4 *m, float z);

Mat4 mat4_look_at(Vec3 pos, Vec3 target, Vec3 up);
Mat4 mat4_look(Vec3 pos, Vec3 dir, Vec3 up);

Mat4 mat4_perspective(float fov, float aspectRatio, float znear, float zfar);
void mat4_perspective_resize(Mat4 *m, float aspectRatio);

Mat4 mat4_ortho(float left, float right, float top, float bottom, float znear, float zfar);

#endif // _MAT_H_