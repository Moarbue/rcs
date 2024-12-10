#include "camera.h"

#include "logging.h"

#include <malloc.h>

Camera *camera(Vec3 target, float distance, float theta, float phi, float zrot)
{
    Camera *cam;
    int i;

    log_info("Creating camera...");

    cam = (Camera *) malloc(sizeof (Camera));
    if (cam == NULL) {
        log_error("Failed to allocate memory for camera");
        return NULL;
    }

    cam->target = target;
    cam->dir    = vec3_from_spherical(1.0f, theta, phi);
    cam->dst    = distance;
    cam->iori   = quat_from_euler_xyz(vec3(phi, theta, zrot));

    for (i = 0; i < CAM_ANIM_ALL; i++) {
        cam->a[i] = (Animation) {0};
        cam->a[i].efunc = linear;
    }

    log_info("Finished creating camera");

    return cam;
}

void camera_set_orbit_position(Camera *cam, float distance, float theta, float phi, float zrot)
{
    Animation *a;

    a = &cam->a[CAM_ANIM_ORIENTATION];
    *a = animate_quaternion(
        &cam->iori,
        quat_from_euler_xyz(vec3(phi, theta, zrot)),
        a->duration,
        a->efunc
    );
    
    a = &cam->a[CAM_ANIM_DISTANCE];
    *a = animate_scalar(
        &cam->dst,
        distance,
        a->duration,
        a->efunc
    );
}

void camera_add_to_orbit_position(Camera *cam, float distance, float theta, float phi, float zrot)
{
    Animation *a;

    a = &cam->a[CAM_ANIM_ORIENTATION];
    *a = animate_quaternion(
        &cam->iori,
        quat_mul(cam->iori, quat_from_euler_xyz(vec3(phi, theta, zrot))),
        a->duration,
        a->efunc
    );
    
    a = &cam->a[CAM_ANIM_DISTANCE];
    *a = animate_scalar(
        &cam->dst,
        cam->dst + distance,
        a->duration,
        a->efunc
    );
}

void camera_set_target(Camera *cam, Vec3 target)
{
    Animation *a;

    a = &cam->a[CAM_ANIM_TARGET];
    *a = animate_vector3(
        &cam->target,
        target,
        a->duration,
        a->efunc
    );
}

void camera_add_to_target(Camera *cam, Vec3 target)
{
    Animation *a;

    a = &cam->a[CAM_ANIM_TARGET];
    *a = animate_vector3(
        &cam->target,
        vec3_add(cam->target, target),
        a->duration,
        a->efunc
    );
}

void camera_set_animation_duration(Camera *cam, Camera_Animation type, float duration)
{
    int i;

    if (type == CAM_ANIM_ALL) {
        for (i = 0; i < CAM_ANIM_ALL; i++)
            cam->a[i].duration = duration;
    } else {
        cam->a[type].duration = duration;
    }
}

void camera_set_animation_easing_func(Camera *cam, Camera_Animation type, easing_func efunc)
{
    int i;

    if (type == CAM_ANIM_ALL) {
        for (i = 0; i < CAM_ANIM_ALL; i++)
            cam->a[i].efunc = efunc;
    } else {
        cam->a[type].efunc = efunc;
    }
}

Mat4 camera_get_view_matrix(Camera *cam)
{
    // position of camera is target + dst * dir
    // conjugate ori because functions accepts camera orientation
    return quat_to_look(
        vec3_add(cam->target, vec3_scale(cam->dir, cam->dst)),
        quat_conjugate(cam->iori)
    );
}

void camera_update(Camera *cam, float dt)
{
    int i;

    for (i = 0; i < CAM_ANIM_ALL; i++)
        update_animation(&cam->a[i], dt);

    // calculate new direction based on current orientation
    // basically rotate the vector with zero rotation (0, 0, 1) by the inverse camera orientation
    if (animation_is_running(&cam->a[CAM_ANIM_ORIENTATION])) {
        cam->dir = quat_rotatev3(vec3(0, 0, 1), quat_conjugate(cam->iori));
    }
}

void camera_free(Camera *cam)
{
    if (cam == NULL) return;
    free(cam);
}