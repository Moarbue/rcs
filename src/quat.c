#include "quat.h"
#include "smath.h"

// Create Quat from raw values
Quat quat(float x, float y, float z, float w)
{
    return (Quat) {x, y, z, w};
}

// Create Quat from single value
Quat quats(float s)
{
    return (Quat) {s, s, s, s};
}

// Create identity Quat
Quat quat_identity(void)
{
    return quat(0, 0, 0, 1);
}

// Create Quat from rotation axis and angle
Quat quat_from_axis_angle(Vec3 axis, float angle)
{
    float a, c, s;

    a = angle * 0.5f;
    c = cosf(a);
    s = sinf(a);

    vec3_normalize(&axis);

    return quat(
        s * axis.x,
        s * axis.y,
        s * axis.z,
        c
    );
}

// Create Quat that rotates a to b
Quat quat_from_vecs(Vec3 a, Vec3 b)
{
    Vec3 axis;
    float cos_theta, cos_half_theta;

    cos_theta = vec3_dot(a, b);

    if (cos_theta >= 1.0f - EPS) return quat_identity();
    if (cos_theta < -1.0f + EPS) {
        axis = vec3_cross(vec3(1, 0, 0), a);
        cos_half_theta = 0.0f;
    } else {
        axis = vec3_cross(a, b);
        cos_half_theta = 1.0f + cos_theta;
    }

    return quat_normalize_to(
        quat(axis.x, axis.y, axis.z, cos_half_theta)
    );
}

// Create Quat from direction and up vector
Quat quat_from_dir(Vec3 dir, Vec3 up)
{
    Mat4 m;
    Vec3 t;

    m = mat4();

    vec3_normalize(&dir);
    vec3_negate(&dir);

    m.e[0][2] = dir.x;
    m.e[1][2] = dir.y;
    m.e[2][2] = dir.z;

    t = vec3_crossn(up, dir);

    m.e[0][0] = t.x;
    m.e[1][0] = t.y;
    m.e[2][0] = t.z;

    t = vec3_cross(dir, t);

    m.e[0][1] = t.x;
    m.e[1][1] = t.y;
    m.e[2][1] = t.z;

    return quat_from_mat4(m);
}

// Create Quat from euler angles
Quat quat_from_euler_xyz(Vec3 angles)
{
    float xc, xs, yc, ys, zc, zs, yczc, yszs;

    xc = cosf(angles.x * 0.5f);     xs = sinf(angles.x * 0.5f);
    yc = cosf(angles.y * 0.5f);     ys = sinf(angles.y * 0.5f);
    zc = cosf(angles.z * 0.5f);     zs = sinf(angles.z * 0.5f);
    yczc = yc * zc;
    yszs = ys * zs;

    return quat(
        xc * yszs  + xs * yczc,
        xc * ys*zc - xs * yc*zs,
        xc * yc*zs + xs * ys*zc,
        xc * yczc  - xs * yszs
    );
}

// Create Quat from Mat4
Quat quat_from_mat4(Mat4 m)
{
    float trace, r, rinv;

    trace = m.e[0][0] + m.e[1][1] + m.e[2][2];
    if (trace >= 0.0f) {
        r    = sqrtf(1.0f + trace);
        rinv = 0.5f / r;

        return quat(
            rinv * (m.e[2][1] - m.e[1][2]),
            rinv * (m.e[0][2] - m.e[2][0]),
            rinv * (m.e[1][0] - m.e[0][1]),
            r    * 0.5f
        );
    } else if (m.e[0][0] >= m.e[1][1] && m.e[0][0] >= m.e[2][2]) {
        r       = sqrtf(1.0f - m.e[1][1] - m.e[2][2] + m.e[0][0]);
        rinv    = 0.5f / r;

        return quat(
            r    * 0.5f,
            rinv * (m.e[1][0] + m.e[0][1]),
            rinv * (m.e[0][2] + m.e[2][0]),
            rinv * (m.e[2][1] - m.e[1][2])
        );
    } else if (m.e[1][1] >= m.e[2][2]) {
        r       = sqrtf(1.0f - m.e[0][0] - m.e[2][2] + m.e[1][1]);
        rinv    = 0.5f / r;

        return quat(
            rinv * (m.e[1][0] + m.e[0][1]),
            r    * 0.5f,
            rinv * (m.e[2][1] + m.e[1][2]),
            rinv * (m.e[0][2] - m.e[2][0])
        );
    } else {
        r       = sqrtf(1.0f - m.e[0][0] - m.e[1][1] + m.e[2][2]);
        rinv    = 0.5f / r;

        return quat(
            rinv * (m.e[0][2] + m.e[2][0]),
            rinv * (m.e[2][1] + m.e[1][2]),
            r    * 0.5f,
            rinv * (m.e[1][0] - m.e[0][1])
        );
    }
}

// Compute the magnitude of a Quat
float quat_mag(Quat q)
{
    return sqrtf(
        quat_mag_squared(q)
    );
}

// Compute the squared magnitude of a Quat
float quat_mag_squared(Quat q)
{
    return pow2(q.x) + pow2(q.y) + pow2(q.z) + pow2(q.w);
}

// Normalize a Quat
void quat_normalize(Quat *q)
{
    *q = quat_normalize_to(*q);
}

// Normalize a Quat and return the result
Quat quat_normalize_to(Quat q)
{
    float mag;

    mag = quat_mag_squared(q);
    
    // quat is already normalized
    if (fabsf(mag - 1.0f) < EPS) return q;

    mag = sqrtf(mag);
    // quat has magnitude 0
    if (mag < EPS) return quat_identity();

    mag = 1.0f / mag;

    return quat(
        q.x * mag, q.y * mag, q.z * mag, q.w * mag
    );
}

// Conjugate a Quat, negate the imaginary part
Quat quat_conjugate(Quat q)
{
    return quat(-q.x, -q.y, -q.z, q.w);
}

// Returns the real part of a Quat
float quat_real(Quat q)
{
    return q.w;
}

// Returns the imaginary part of a Quat
Vec3 quat_imag(Quat q)
{
    return vec3(q.x, q.y, q.z);
}

// Returns the normalized imaginary part of a Quat
Vec3 quat_imagn(Quat q)
{
    return vec3_normalize_to(
        quat_imag(q)
    );
}

// Computes the magnitude of the imaginary part of a Quat
float quat_imag_len(Quat q)
{
    return vec3_mag(
        quat_imag(q)
    );
}

// Computes the rotation angle of a Quat
float quat_angle(Quat q)
{
    return 2.0f * atan2f(quat_imag_len(q), quat_real(q));
}

// Computes the rotation axis of a Quat
Vec3 quat_axis(Quat q)
{
    return quat_imagn(q);
}

// Converts a Quat to a rotation Mat4
Mat4 quat_to_mat4(Quat q)
{
    Mat4 m;
    float qxx, qyy, qzz,
          qxy, qyz, qxz,
          qwx, qwy, qwz, mag, s;

    mag = quat_mag(q);
    s   = mag > EPS ? 2.0f / mag : 0.0f;

    qxx = s * pow2(q.x);  qxy = s * q.x * q.y;  qwx = s * q.w * q.x;
    qyy = s * pow2(q.y);  qyz = s * q.y * q.z;  qwy = s * q.w * q.y;
    qzz = s * pow2(q.z);  qxz = s * q.x * q.z;  qwz = s * q.w * q.z;

    m = mat4();

    m.e[0][0] = 1.0f - qyy - qzz;
    m.e[1][1] = 1.0f - qxx - qzz;
    m.e[2][2] = 1.0f - qxx - qyy;

    m.e[1][0] = qxy + qwz;
    m.e[0][1] = qxy - qwz;

    m.e[0][2] = qxz + qwy;
    m.e[2][0] = qxz - qwy;

    m.e[2][1] = qyz + qwx;
    m.e[1][2] = qyz - qwx;

    return m;
}

// Converts a Quat to a rotation Mat4 and transposes it
Mat4 quat_to_mat4t(Quat q)
{
    return mat4_transpose_to(
        quat_to_mat4(q)
    );
}

// Converts a Quat to a view matrix
Mat4 quat_to_look(Vec3 pos, Quat ori)
{
    Mat4 m;
    Vec3 p;

    m = quat_to_mat4t(ori);
    p = mat4_mulv3(m, pos, 1.0f);

    m.e[0][3] = -p.x;
    m.e[1][3] = -p.y;
    m.e[2][3] = -p.z;

    return m;
}

// Converts a Quat into euler angles
Vec3 quat_to_euler_xyz(Quat q)
{
    float qxx, qyy, qzz, qww, mag2, test;

    qxx = pow2(q.x);  qzz = pow2(q.z);
    qyy = pow2(q.y);  qww = pow2(q.w);
    
    mag2 = qxx + qyy + qzz + qww;
    test = q.w*q.y + q.x*q.z;

    if (test > (0.5f - EPS) * mag2) {
        return vec3(
            2.0f * atan2f(q.x, q.w),
            (float)M_PI_2,
            0.0f
        );
    } else if (test < (EPS - 0.5f) * mag2) {
        return vec3(
            2.0f * atan2f(q.x, q.w),
            (float)-M_PI_2,
            0.0f
        );
    }

    return vec3(
        atan2f(2.0f*q.x*q.w - 2.0f*q.y*q.z, -qxx - qyy + qzz + qww),
        asinf(2.0f*test / mag2),
        atan2f(2.0f*q.z*q.w - 2.0f*q.x*q.y,  qxx - qyy - qzz + qww)
    );
}

// Dot product of two Quats (Vec4)
float quat_dot(Quat a, Quat b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

// Component-wise addition of two Quats
Quat quat_add(Quat a, Quat b)
{
    return quat(
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w
    );
}

// Component-wise subtraction of two Quats
Quat quat_sub(Quat a, Quat b)
{
    return quat(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    );
}

// Scale a Quat by a scalar
Quat quat_scale(Quat q, float s)
{
    return quat(
        q.x * s,
        q.y * s,
        q.z * s,
        q.w * s
    );
}

// Multiplies two Quats (Hamilton Product)
Quat quat_mul(Quat a, Quat b)
{
    return quat(        
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
    );
}

// Rotates a vec with a Quat
Vec3 quat_rotatev3(Vec3 v, Quat q)
{
    Vec3 u, v1, v2;
    float s;

    quat_normalize(&q);

    u = quat_imag(q);
    s = quat_real(q);

    v1 = vec3_scale(u, 2.0f*vec3_dot(u, v));
    v2 = vec3_scale(v, pow2(s) - vec3_dot(u, u));
    v1 = vec3_add(v1, v2);

    v2 = vec3_cross(u, v);
    v2 = vec3_scale(v2, 2.0f*s);

    return vec3_add(v1, v2);
}

// Rotates a existing Mat4 with a Quat
void quat_rotatem4(Mat4 *m, Quat q)
{
    *m = mat4_mul(
        *m, quat_to_mat4(q)
    );
}

// Rotates a existing Mat4 with a Quat about a rotation center (pivot)
void quat_rotatem4_at(Mat4 *m, Quat q, Vec3 pivot)
{
    mat4_translate(m, pivot);
    quat_rotatem4(m, q);
    mat4_translate(m, vec3_negate_to(pivot));
}

// Linear Interpolate two Quats
Quat quat_lerp(Quat a, Quat b, float t)
{
    Quat v;

    v = quat_sub(b, a);
    v = quat_scale(v, t);

    return quat_add(a, v);
}

Quat quat_slerp(Quat a, Quat b, float t)
{
    float cos_theta, sin_theta, angle;

    cos_theta = quat_dot(a, b);

    if (fabsf(cos_theta) >= 1.0f) return a;

    if (cos_theta < 0.0f) {
        a.x = -a.x;  a.y = -a.y;
        a.z = -a.z;  a.w = -a.w;
        cos_theta = -cos_theta;
    }

    sin_theta = sqrtf(1.0f - pow2(cos_theta));

    if (fabsf(sin_theta) < EPS) return quat_lerp(a, b, t);

    angle = acosf(cos_theta);
    a = quat_scale(a, sinf((1.0f - t) * angle));
    b = quat_scale(b, sinf(t * angle));

    return quat_scale(
        quat_add(a, b),
        1.0f / sin_theta
    );
}
