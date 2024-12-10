#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "animation.h"
#include "mat.h"
#include "quat.h"
#include "vec.h"

typedef enum {
    CAM_ANIM_ORIENTATION,
    CAM_ANIM_DISTANCE,
    CAM_ANIM_TARGET,
    CAM_ANIM_ALL,
} Camera_Animation;

typedef struct {
    float dst;          // distance from target
    Vec3 dir;           // direction from target
    Vec3 target;        // target to look at
    Quat iori;          // inverse orientation of camera

    Animation a[CAM_ANIM_ALL];
} Camera;

Camera *camera(Vec3 target, float distance, float theta, float phi, float zrot);
void camera_set_orbit_position(Camera *cam, float distance, float theta, float phi, float zrot);
void camera_add_to_orbit_position(Camera *cam, float distance, float theta, float phi, float zrot);
void camera_set_target(Camera *cam, Vec3 target);
void camera_add_to_target(Camera *cam, Vec3 target);
void camera_set_animation_duration(Camera *cam, Camera_Animation type, float duration);
void camera_set_animation_easing_func(Camera *cam, Camera_Animation type, easing_func efunc);
Mat4 camera_get_view_matrix(Camera *cam);
void camera_update(Camera *cam, float dt);
void camera_free(Camera *cam);

#endif // _CAMERA_H_