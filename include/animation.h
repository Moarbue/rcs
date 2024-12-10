#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "smath.h"

typedef float (easing_func) (float t);

typedef enum {
    ANIMATED_SCALAR,
    ANIMATED_VECTOR3,
    ANIMATED_QUATERNION,
} Animation_Type;

typedef struct {
    float start;
    float end;
    float *val;
} Animated_Scalar;

typedef struct {
    Vec3 start;
    Vec3 end;
    Vec3 *val;
} Animated_Vector3;

typedef struct {
    Quat start;
    Quat end;
    Quat *val;
} Animated_Quaternion;


typedef struct {
    float duration; // total duration
    float d;        // current duration
    int running;

    Animation_Type type;   // Type of Animation
    union {
        Animated_Scalar     s;
        Animated_Vector3    v;
        Animated_Quaternion q;
    } data;

    easing_func *efunc; // determines the rate of change of the animation
} Animation;

Animation animate_scalar    (float *scalar,     float endValue, float duration, easing_func *efunc);
Animation animate_vector3   (Vec3  *vector,     Vec3  endValue, float duration, easing_func *efunc);
Animation animate_quaternion(Quat  *quaternion, Quat  endValue, float duration, easing_func *efunc);
void update_animation(Animation *a, float dt);
int animation_is_running(Animation *a);

// easing functions from https://easings.net/

float linear(float t);

float ease_in_sine(float t);
float ease_out_sine(float t);
float ease_in_out_sine(float t);

#endif // _ANIMATION_H_