#include "animation.h"
#include "util.h"

Animation animation_init(Animation_Type type, float duration, easing_func *efunc)
{
    Animation a;

    if (fabsf(duration) < EPS) duration = EPS;

    a.duration = duration;
    a.d        = 0;
    a.efunc    = efunc;
    a.running  = 1;
    a.type     = type;
    
    return a;
}

Animation animate_scalar(float *scalar, float endValue, float duration, easing_func *efunc)
{
    Animation a = animation_init(ANIMATED_SCALAR, duration, efunc);

    a.data.s.start = *scalar;
    a.data.s.end   = endValue;
    a.data.s.val   = scalar;

    return a;
}

Animation animate_vector3(Vec3 *vector, Vec3 end_value, float duration, easing_func *efunc)
{
    Animation a = animation_init(ANIMATED_VECTOR3, duration, efunc);

    a.data.v.start = *vector;
    a.data.v.end   = end_value;
    a.data.v.val   = vector;

    return a;
}

Animation animate_quaternion(Quat *quaternion, Quat end_value, float duration, easing_func *efunc)
{
    Animation a = animation_init(ANIMATED_QUATERNION, duration, efunc);

    a.data.q.start = *quaternion;
    a.data.q.end   = end_value;
    a.data.q.val   = quaternion;

    return a;
}

void update_animation(Animation *a, float dt)
{
    if (a->running) {
        if ((a->d + dt) < a->duration)
            a->d += dt;
        else {
            a->d = a->duration;
            a->running = 0;
        }

        float t = (a->efunc)(a->d / a->duration);

        switch (a->type) {
            case ANIMATED_SCALAR:
                *a->data.s.val = lerp(a->data.s.start, a->data.s.end, t);
            break;

            case ANIMATED_VECTOR3:
                *a->data.v.val = vec3_lerp(a->data.v.start, a->data.v.end, t);
            break;

            case ANIMATED_QUATERNION:
                *a->data.q.val = quat_slerp(a->data.q.start, a->data.q.end, t);
            break;
        }
    }
}

int animation_is_running(Animation *a)
{
    return a->running;
}


float linear(float t)
{
    return t;
}

float ease_in_sine(float t)
{
    return 1.0f - cosf(t * M_PI_2);
}

float ease_out_sine(float t)
{
    return sinf(t * M_PI_2);
}

float ease_in_out_sine(float t)
{
    return (1.0f - cosf(t * M_PI)) / 2.0f;
}