#include "camera.h"
#include "config.h"
#include "cube.h"
#include "logging.h"
#include "smath.h"
#include "window.h"

void key_callback(int key, int action, int mods);
void window_size_callback(int width, int height);

void render_frame(void);

// global variables
Config conf;
Mat4 proj, view_proj;
Camera *cam;
Rubiks_Cube *rc;

int main(void)
{
    conf = config_default();

    set_logging_level (conf.logging_level);
    set_logging_stream(conf.logging_stream);

    window_init(conf.default_window_width, conf.default_window_height, "Rubiks Cube Simulation", conf.fullscreen);
    window_set_key_callback(key_callback);
    window_set_size_callback(window_size_callback);
    window_set_clear_color(conf.background_color);

    proj = mat4_perspective(
        conf.default_fov,
        window_get_aspect_ratio(),
        conf.default_nearZ,
        conf.default_farZ
    );

    cam = camera(
        conf.default_cam_target,
        conf.default_cam_distance,
        conf.default_cam_xrot,
        conf.default_cam_yrot,
        conf.default_cam_zrot
    );
    camera_set_animation_duration   (cam, CAM_ANIM_ALL, conf.default_cam_anim_duration);
    camera_set_animation_easing_func(cam, CAM_ANIM_ALL, conf.default_cam_anim_efunc);

    rc = rubiks_cube(&conf.rcconf);


    window_main_loop(render_frame);


    camera_free(cam);
    rubiks_cube_free(rc);
    window_close();
    return 0;
}

void key_callback(int key, int action, int mods)
{
    if (key  == conf.keys[KEY_WINDOW_CLOSE].key &&
        mods == conf.keys[KEY_WINDOW_CLOSE].mod && action == KEY_PRESS) {
            
        window_set_should_close(1);
    }
    if (key  == conf.keys[KEY_TOGGLE_FULLSCREEN].key &&
        mods == conf.keys[KEY_TOGGLE_FULLSCREEN].mod && action == KEY_PRESS) {
            
        window_toggle_fullscreen();
    }


    if (key  == conf.keys[KEY_ROTATE_CUBE_POS_90_X].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_POS_90_X].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate(rc, vec3(1.0f, 0.0f, 0.0f),  M_PI_2);
    }
    if (key  == conf.keys[KEY_ROTATE_CUBE_NEG_90_X].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_NEG_90_X].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate(rc, vec3(1.0f, 0.0f, 0.0f), -M_PI_2);
    }

    if (key  == conf.keys[KEY_ROTATE_CUBE_POS_90_Y].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_POS_90_Y].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate(rc, vec3(0.0f, 1.0f, 0.0f),  M_PI_2);
    }
    if (key  == conf.keys[KEY_ROTATE_CUBE_NEG_90_Y].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_NEG_90_Y].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate(rc, vec3(0.0f, 1.0f, 0.0f), -M_PI_2);
    }

    if (key  == conf.keys[KEY_ROTATE_CUBE_POS_90_Z].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_POS_90_Z].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate(rc, vec3(0.0f, 0.0f, 1.0f),  M_PI_2);
    }
    if (key  == conf.keys[KEY_ROTATE_CUBE_NEG_90_Z].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_NEG_90_Z].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate(rc, vec3(0.0f, 0.0f, 1.0f), -M_PI_2);
    }


    if (key  == conf.keys[KEY_ROTATE_FRONT_CW].key  &&
        mods == conf.keys[KEY_ROTATE_FRONT_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_FRONT_180].key &&
        mods == conf.keys[KEY_ROTATE_FRONT_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_FRONT_CCW].key &&
        mods == conf.keys[KEY_ROTATE_FRONT_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_UP_CW].key  &&
        mods == conf.keys[KEY_ROTATE_UP_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_UP, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_UP_180].key &&
        mods == conf.keys[KEY_ROTATE_UP_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_UP, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_UP_CCW].key &&
        mods == conf.keys[KEY_ROTATE_UP_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_UP, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_LEFT_CW].key  &&
        mods == conf.keys[KEY_ROTATE_LEFT_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_LEFT_180].key &&
        mods == conf.keys[KEY_ROTATE_LEFT_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_LEFT_CCW].key &&
        mods == conf.keys[KEY_ROTATE_LEFT_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_BACK_CW].key  &&
        mods == conf.keys[KEY_ROTATE_BACK_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_BACK, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_BACK_180].key &&
        mods == conf.keys[KEY_ROTATE_BACK_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_BACK, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_BACK_CCW].key &&
        mods == conf.keys[KEY_ROTATE_BACK_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_BACK, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_DOWN_CW].key  &&
        mods == conf.keys[KEY_ROTATE_DOWN_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_DOWN_180].key &&
        mods == conf.keys[KEY_ROTATE_DOWN_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_DOWN_CCW].key &&
        mods == conf.keys[KEY_ROTATE_DOWN_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_RIGHT_CW].key  &&
        mods == conf.keys[KEY_ROTATE_RIGHT_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_RIGHT, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_RIGHT_180].key &&
        mods == conf.keys[KEY_ROTATE_RIGHT_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_RIGHT, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_RIGHT_CCW].key &&
        mods == conf.keys[KEY_ROTATE_RIGHT_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_RIGHT, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_M_CW].key  &&
        mods == conf.keys[KEY_ROTATE_M_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_CW,  1);
    }
    if (key  == conf.keys[KEY_ROTATE_M_180].key &&
        mods == conf.keys[KEY_ROTATE_M_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_180, 1);
    }
    if (key  == conf.keys[KEY_ROTATE_M_CCW].key &&
        mods == conf.keys[KEY_ROTATE_M_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_CCW, 1);
    }

    if (key  == conf.keys[KEY_ROTATE_E_CW].key  &&
        mods == conf.keys[KEY_ROTATE_E_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_CW,  1);
    }
    if (key  == conf.keys[KEY_ROTATE_E_180].key &&
        mods == conf.keys[KEY_ROTATE_E_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_180, 1);
    }
    if (key  == conf.keys[KEY_ROTATE_E_CCW].key &&
        mods == conf.keys[KEY_ROTATE_E_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_CCW, 1);
    }

    if (key  == conf.keys[KEY_ROTATE_S_CW].key  &&
        mods == conf.keys[KEY_ROTATE_S_CW].mod  && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_CW,  1);
    }
    if (key  == conf.keys[KEY_ROTATE_S_180].key &&
        mods == conf.keys[KEY_ROTATE_S_180].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_180, 1);
    }
    if (key  == conf.keys[KEY_ROTATE_S_CCW].key &&
        mods == conf.keys[KEY_ROTATE_S_CCW].mod && action == KEY_PRESS) {
            
        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_CCW, 1);
    }
}

void window_size_callback(int width, int height)
{
    mat4_perspective_resize(&proj, (float)width / (float) height);
    view_proj = mat4_mul(proj, camera_get_view_matrix(cam));
}

void render_frame(void)
{
    float dt;
    dt = window_get_frame_time();

    rubiks_cube_update(rc, dt);
    camera_update(cam, dt);
    view_proj = mat4_mul(proj, camera_get_view_matrix(cam));

    window_clear();
    rubiks_cube_draw(rc, view_proj);
}