#include "mat.h"
#include "smath.h"

// Create identity Mat4
Mat4 mat4(void)
{
    Mat4 m = {0};

    m.e[0][0] = 1.0f;
    m.e[1][1] = 1.0f;
    m.e[2][2] = 1.0f;
    m.e[3][3] = 1.0f;

    return m;
}

// Create matrix from existing Mat4
Mat4 mat4_copy(Mat4 m)
{
    Mat4 res;

    res.e[0][0] = m.e[0][0];  res.e[1][0] = m.e[1][0];
    res.e[0][1] = m.e[0][1];  res.e[1][1] = m.e[1][1];
    res.e[0][2] = m.e[0][2];  res.e[1][2] = m.e[1][2];
    res.e[0][3] = m.e[0][3];  res.e[1][3] = m.e[1][3];

    res.e[2][0] = m.e[2][0];  res.e[3][0] = m.e[3][0];
    res.e[2][1] = m.e[2][1];  res.e[3][1] = m.e[3][1];
    res.e[2][2] = m.e[2][2];  res.e[3][2] = m.e[3][2];
    res.e[2][3] = m.e[2][3];  res.e[3][3] = m.e[3][3];

    return res;
}

// Fill every element of Mat4 wth val
Mat4 mat4_fill(float val)
{
    Mat4 res;

    res.e[0][0] = val;  res.e[1][0] = val;
    res.e[0][1] = val;  res.e[1][1] = val;
    res.e[0][2] = val;  res.e[1][2] = val;
    res.e[0][3] = val;  res.e[1][3] = val;

    res.e[2][0] = val;  res.e[3][0] = val;
    res.e[2][1] = val;  res.e[3][1] = val;
    res.e[2][2] = val;  res.e[3][2] = val;
    res.e[2][3] = val;  res.e[3][3] = val;

    return res;
}

// Multiply two Mat4s
Mat4 mat4_mul(Mat4 a, Mat4 b)
{
    Mat4 res;

    res.e[0][0] = a.e[0][0]*b.e[0][0] + a.e[0][1]*b.e[1][0] + a.e[0][2]*b.e[2][0] + a.e[0][3]*b.e[3][0];
    res.e[0][1] = a.e[0][0]*b.e[0][1] + a.e[0][1]*b.e[1][1] + a.e[0][2]*b.e[2][1] + a.e[0][3]*b.e[3][1];
    res.e[0][2] = a.e[0][0]*b.e[0][2] + a.e[0][1]*b.e[1][2] + a.e[0][2]*b.e[2][2] + a.e[0][3]*b.e[3][2];
    res.e[0][3] = a.e[0][0]*b.e[0][3] + a.e[0][1]*b.e[1][3] + a.e[0][2]*b.e[2][3] + a.e[0][3]*b.e[3][3];

    res.e[1][0] = a.e[1][0]*b.e[0][0] + a.e[1][1]*b.e[1][0] + a.e[1][2]*b.e[2][0] + a.e[1][3]*b.e[3][0];
    res.e[1][1] = a.e[1][0]*b.e[0][1] + a.e[1][1]*b.e[1][1] + a.e[1][2]*b.e[2][1] + a.e[1][3]*b.e[3][1];
    res.e[1][2] = a.e[1][0]*b.e[0][2] + a.e[1][1]*b.e[1][2] + a.e[1][2]*b.e[2][2] + a.e[1][3]*b.e[3][2];
    res.e[1][3] = a.e[1][0]*b.e[0][3] + a.e[1][1]*b.e[1][3] + a.e[1][2]*b.e[2][3] + a.e[1][3]*b.e[3][3];

    res.e[2][0] = a.e[2][0]*b.e[0][0] + a.e[2][1]*b.e[1][0] + a.e[2][2]*b.e[2][0] + a.e[2][3]*b.e[3][0];
    res.e[2][1] = a.e[2][0]*b.e[0][1] + a.e[2][1]*b.e[1][1] + a.e[2][2]*b.e[2][1] + a.e[2][3]*b.e[3][1];
    res.e[2][2] = a.e[2][0]*b.e[0][2] + a.e[2][1]*b.e[1][2] + a.e[2][2]*b.e[2][2] + a.e[2][3]*b.e[3][2];
    res.e[2][3] = a.e[2][0]*b.e[0][3] + a.e[2][1]*b.e[1][3] + a.e[2][2]*b.e[2][3] + a.e[2][3]*b.e[3][3];

    res.e[3][0] = a.e[3][0]*b.e[0][0] + a.e[3][1]*b.e[1][0] + a.e[3][2]*b.e[2][0] + a.e[3][3]*b.e[3][0];
    res.e[3][1] = a.e[3][0]*b.e[0][1] + a.e[3][1]*b.e[1][1] + a.e[3][2]*b.e[2][1] + a.e[3][3]*b.e[3][1];
    res.e[3][2] = a.e[3][0]*b.e[0][2] + a.e[3][1]*b.e[1][2] + a.e[3][2]*b.e[2][2] + a.e[3][3]*b.e[3][2];
    res.e[3][3] = a.e[3][0]*b.e[0][3] + a.e[3][1]*b.e[1][3] + a.e[3][2]*b.e[2][3] + a.e[3][3]*b.e[3][3];

    return res;
}

// Multiply Mat4 with Vec3, w is 4th element of Vec3 since a Vec4 is required for multiplication
Vec3 mat4_mulv3(Mat4 m, Vec3 v, float w)
{
    Vec3 res;

    res.x = m.e[0][0]*v.x + m.e[0][1]*v.y + m.e[0][2]*v.z + m.e[0][3]*w;
    res.y = m.e[1][0]*v.x + m.e[1][1]*v.y + m.e[1][2]*v.z + m.e[1][3]*w;
    res.z = m.e[2][0]*v.x + m.e[2][1]*v.y + m.e[2][2]*v.z + m.e[2][3]*w;

    return res;
}

// Scale Mat4 by a scalar
void mat4_muls(Mat4 *m, float s)
{
    m->e[0][0] *= s;  m->e[1][0] *= s;
    m->e[0][1] *= s;  m->e[1][1] *= s;
    m->e[0][2] *= s;  m->e[1][2] *= s;
    m->e[0][3] *= s;  m->e[1][3] *= s;

    m->e[2][0] *= s;  m->e[3][0] *= s;
    m->e[2][1] *= s;  m->e[3][1] *= s;
    m->e[2][2] *= s;  m->e[3][2] *= s;
    m->e[2][3] *= s;  m->e[3][3] *= s;
}

// Transpose Mat4
void mat4_transpose(Mat4 *m)
{
    Mat4 t;

    t  = mat4_transpose_to(*m);
    *m = mat4_copy(t);
}

// Transpose Mat4 and return result
Mat4 mat4_transpose_to(Mat4 m)
{
    Mat4 res;

    res.e[0][0] = m.e[0][0];  res.e[1][0] = m.e[0][1];
    res.e[0][1] = m.e[1][0];  res.e[1][1] = m.e[1][1];
    res.e[0][2] = m.e[2][0];  res.e[1][2] = m.e[2][1];
    res.e[0][3] = m.e[3][0];  res.e[1][3] = m.e[3][1];

    res.e[2][0] = m.e[0][2];  res.e[3][0] = m.e[0][3];
    res.e[2][1] = m.e[1][2];  res.e[3][1] = m.e[1][3];
    res.e[2][2] = m.e[2][2];  res.e[3][2] = m.e[2][3];
    res.e[2][3] = m.e[3][2];  res.e[3][3] = m.e[3][3];

    return res;
}

// Create a translatation Mat4
Mat4 mat4_translation(Vec3 v)
{
    Mat4 m;

    m = mat4();
    m.e[0][3] = v.x;
    m.e[1][3] = v.y;
    m.e[2][3] = v.z;

    return m;
}

// Translate existing Mat4 by Vec3
void mat4_translate(Mat4 *m, Vec3 v)
{
    m->e[0][3] += m->e[0][0]*v.x + m->e[0][1]*v.y + m->e[0][2]*v.z;
    m->e[1][3] += m->e[1][0]*v.x + m->e[1][1]*v.y + m->e[1][2]*v.z;
    m->e[2][3] += m->e[2][0]*v.x + m->e[2][1]*v.y + m->e[2][2]*v.z;
}

// Translate existing Mat4 by x
void mat4_translate_x(Mat4 *m, float x)
{
    m->e[0][3] += m->e[0][0]*x;
    m->e[1][3] += m->e[1][0]*x;
    m->e[2][3] += m->e[2][0]*x;
}

// Translate existing Mat4 by y
void mat4_translate_y(Mat4 *m, float y)
{
    m->e[0][3] += m->e[0][1]*y;
    m->e[1][3] += m->e[1][1]*y;
    m->e[2][3] += m->e[2][1]*y;
}

// Translate existing Mat4 by z
void mat4_translate_z(Mat4 *m, float z)
{
    m->e[0][3] += m->e[0][2]*z;
    m->e[1][3] += m->e[1][2]*z;
    m->e[2][3] += m->e[2][2]*z;
}

// Create a rotation matrix by rotation axis and angle
Mat4 mat4_rotation(Vec3 axis, float angle)
{
    Mat4 m;
    float c, s, C;
    float axyC, axzC, ayzC, axs, ays, azs;

    c = cosf(angle);
    s = sinf(angle);
    C = 1.0f - c;

    vec3_normalize(&axis);

    axyC = axis.x * axis.y * C;
    axzC = axis.x * axis.z * C;
    ayzC = axis.y * axis.z * C;
    axs  = axis.x * s;
    ays  = axis.y * s;
    azs  = axis.z * s;

    m.e[0][0] = pow2(axis.x)*C + c;
    m.e[1][1] = pow2(axis.y)*C + c;
    m.e[2][2] = pow2(axis.z)*C + c;

    m.e[1][0] = axyC + azs;
    m.e[0][1] = axyC - azs;

    m.e[0][2] = axzC + ays;
    m.e[2][0] = axzC - ays;

    m.e[2][1] = ayzC + axs;
    m.e[1][2] = ayzC - axs;

    m.e[3][0] = m.e[3][1] = m.e[3][2] = m.e[0][3] = m.e[1][3] = m.e[2][3] = 0.0f;
    m.e[3][3] = 1.0f;

    return m;
}

// Rotate existing Mat4 by rotation axis and angle
void mat4_rotate(Mat4 *m, Vec3 axis, float angle)
{
    Mat4 t;
    t  = mat4_rotation(axis, angle);
    *m = mat4_mul(*m, t);
}

// Rotate existing Mat4 by rotation axis and angle with a specified rotation center (pivot)
void mat4_rotate_at(Mat4 *m, Vec3 pivot, Vec3 axis, float angle)
{
    mat4_translate(m, pivot);
    mat4_rotate(m, axis, angle);
    mat4_translate(m, vec3_negate_to(pivot));
}

// Rotate existing Mat4 around x-Axis by angle
void mat4_rotate_x(Mat4 *m, float angle)
{
    Mat4 t;
    float c, s;

    t = mat4();
    c = cosf(angle);
    s = sinf(angle);

    t.e[1][1] =  c;
    t.e[2][1] =  s;
    t.e[1][2] = -s;
    t.e[2][2] =  c;

    *m = mat4_mul(*m, t);
}

// Rotate existing Mat4 around y-Axis by angle
void mat4_rotate_y(Mat4 *m, float angle)
{
    Mat4 t;
    float c, s;

    t = mat4();
    c = cosf(angle);
    s = sinf(angle);

    t.e[0][0] =  c;
    t.e[2][0] = -s;
    t.e[0][2] =  s;
    t.e[2][2] =  c;

    *m = mat4_mul(*m, t);
}

// Rotate existing Mat4 around z-Axis by angle
void mat4_rotate_z(Mat4 *m, float angle)
{
    Mat4 t;
    float c, s;

    t = mat4();
    c = cosf(angle);
    s = sinf(angle);

    t.e[0][0] =  c;
    t.e[1][0] =  s;
    t.e[0][1] = -s;
    t.e[1][1] =  c;

    *m = mat4_mul(*m, t);
}

// Creates a scaling Mat4
Mat4 mat4_scaling(Vec3 v)
{
    Mat4 m;

    m = mat4();
    m.e[0][0] = v.x;
    m.e[1][1] = v.y;
    m.e[2][2] = v.z;

    return m;
}

// Scales a existing Mat4
void mat4_scale(Mat4 *m, Vec3 v)
{
    m->e[0][0] *= v.x;  m->e[1][0] *= v.x;
    m->e[0][1] *= v.y;  m->e[1][1] *= v.y;
    m->e[0][2] *= v.z;  m->e[1][2] *= v.z;

    m->e[2][0] *= v.x;  m->e[3][0] *= v.x;
    m->e[2][1] *= v.y;  m->e[3][1] *= v.y;
    m->e[2][2] *= v.z;  m->e[3][2] *= v.z;
}

// Scale a existing Mat4 by scalar
void mat4_scale_s(Mat4 *m, float s)
{
    mat4_scale(m, vec3s(s));
}

// Scale a existing Mat4 in x direction
void mat4_scale_x(Mat4 *m, float x)
{
    mat4_scale(m, vec3(x, 0, 0));
}

// Scale a existing Mat4 in y direction
void mat4_scale_y(Mat4 *m, float y)
{
    mat4_scale(m, vec3(0, y, 0));
}

// Scale a existing Mat4 in z direction
void mat4_scale_z(Mat4 *m, float z)
{
    mat4_scale(m, vec3(0, 0, z));
}

// Create a look at Mat4 (right handed)
// pos = camera position
// target = camera target
// up = up vector of the world (typically 0, 1, 0)
Mat4 mat4_look_at(Vec3 pos, Vec3 target, Vec3 up)
{
    Mat4 m;
    Vec3 f, u, s;

    f = vec3_sub(target, pos);
    vec3_normalize(&f);

    s = vec3_crossn(f, up);
    u = vec3_cross(s, f);

    m.e[0][0] =  s.x;
    m.e[0][1] =  s.y;
    m.e[0][2] =  s.z;
    m.e[1][0] =  u.x;
    m.e[1][1] =  u.y;
    m.e[1][2] =  u.z;
    m.e[2][0] = -f.x;
    m.e[2][1] = -f.y;
    m.e[2][2] = -f.z;
    m.e[0][3] = -vec3_dot(s, pos);
    m.e[1][3] = -vec3_dot(u, pos);
    m.e[2][3] =  vec3_dot(f, pos);
    m.e[3][0] = m.e[3][1] = m.e[3][2] = 0.0f;
    m.e[3][3] = 1.0f;

    return m;
}

Mat4 mat4_look(Vec3 pos, Vec3 dir, Vec3 up)
{
    return mat4_look_at(pos, vec3_add(pos, dir), up);
}

Mat4 mat4_perspective(float fov, float aspectRatio, float znear, float zfar)
{
    Mat4 m;
    float f, fn;

    m = mat4_fill(0);

    f  = 1.0f / tanf(fov * 0.5f);
    fn = 1.0f / (znear - zfar);

    m.e[0][0] = f / aspectRatio;
    m.e[1][1] = f;
    m.e[2][2] = (znear + zfar) * fn;
    m.e[2][3] = 2.0f * znear * zfar * fn;
    m.e[3][2] = -1.0f;

    return m;
}

void mat4_perspective_resize(Mat4 *m, float aspectRatio)
{
    if (fabsf(m->e[0][0]) < EPS) return;

    m->e[0][0] = m->e[1][1] / aspectRatio;
}
