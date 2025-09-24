#include "animation.h"
#include "camera.h"
#include "config.h"
#include "cube.h"
#include "font.h"
#include "logging.h"
#include "smath.h"
#include "window.h"

#include <string.h>

void key_callback(int key, int action, int mods);
void window_size_callback(int width, int height);

void render_frame(void);

// global variables
Config conf;
Mat4 proj, view_proj, ortho;
Camera *cam;
Rubiks_Cube *rc;
Animation text_opacity_anim;
float text_opacity;
char move[3] = {0};

int main(void)
{
    conf = config_default();

    set_logging_level (conf.logging_level);
    set_logging_stream(conf.logging_stream);

    window_init(conf.default_window_width, conf.default_window_height, "Rubiks Cube Simulation", conf.fullscreen);
    window_set_key_callback(key_callback);
    window_set_size_callback(window_size_callback);
    window_set_clear_color(conf.background_color);
    window_set_fps(conf.fps);

    proj = mat4_perspective(
        conf.fov,
        window_get_aspect_ratio(),
        conf.nearZ,
        conf.farZ
    );

    init_text_renderer(conf.font_vertex_path, conf.font_fragment_path);
    ortho = mat4_ortho(
        0.0f, window_get_width(), 
        0.0f, window_get_height(), 
        conf.nearZ, conf.farZ
    );
    text_set_projection_matrix(ortho);

    cam = camera(
        conf.cam_target,
        conf.cam_distance,
        conf.cam_xrot,
        conf.cam_yrot,
        conf.cam_zrot
    );
    camera_set_animation_duration   (cam, CAM_ANIM_ALL, conf.cam_anim_duration);
    camera_set_animation_easing_func(cam, CAM_ANIM_ALL, conf.cam_anim_efunc);

    rc = rubiks_cube(&conf.rcconf);

    font_load("fonts/open-sans-latin-400-normal.ttf");
    set_active_font(0);

    window_main_loop(render_frame);


    delete_text_renderer();
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
            
        text_opacity = 255.f;
        strcpy(move, "x'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate(rc, vec3(1.0f, 0.0f, 0.0f),  M_PI_2);
    }
    if (key  == conf.keys[KEY_ROTATE_CUBE_NEG_90_X].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_NEG_90_X].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "x");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate(rc, vec3(1.0f, 0.0f, 0.0f), -M_PI_2);
    }

    if (key  == conf.keys[KEY_ROTATE_CUBE_POS_90_Y].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_POS_90_Y].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "y'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate(rc, vec3(0.0f, 1.0f, 0.0f),  M_PI_2);
    }
    if (key  == conf.keys[KEY_ROTATE_CUBE_NEG_90_Y].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_NEG_90_Y].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "y");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate(rc, vec3(0.0f, 1.0f, 0.0f), -M_PI_2);
    }

    if (key  == conf.keys[KEY_ROTATE_CUBE_POS_90_Z].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_POS_90_Z].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "z'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate(rc, vec3(0.0f, 0.0f, 1.0f),  M_PI_2);
    }
    if (key  == conf.keys[KEY_ROTATE_CUBE_NEG_90_Z].key &&
        mods == conf.keys[KEY_ROTATE_CUBE_NEG_90_Z].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "z");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate(rc, vec3(0.0f, 0.0f, 1.0f), -M_PI_2);
    }


    if (key  == conf.keys[KEY_ROTATE_FRONT_CW].key  &&
        mods == conf.keys[KEY_ROTATE_FRONT_CW].mod  && action == KEY_PRESS) {
        
        text_opacity = 255.f;
        strcpy(move, "F");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_FRONT_180].key &&
        mods == conf.keys[KEY_ROTATE_FRONT_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "F2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_FRONT_CCW].key &&
        mods == conf.keys[KEY_ROTATE_FRONT_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "F'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_UP_CW].key  &&
        mods == conf.keys[KEY_ROTATE_UP_CW].mod  && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "U");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_UP, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_UP_180].key &&
        mods == conf.keys[KEY_ROTATE_UP_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "U2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_UP, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_UP_CCW].key &&
        mods == conf.keys[KEY_ROTATE_UP_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "U'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_UP, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_LEFT_CW].key  &&
        mods == conf.keys[KEY_ROTATE_LEFT_CW].mod  && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "L");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_LEFT_180].key &&
        mods == conf.keys[KEY_ROTATE_LEFT_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "L2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_LEFT_CCW].key &&
        mods == conf.keys[KEY_ROTATE_LEFT_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "L'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_BACK_CW].key  &&
        mods == conf.keys[KEY_ROTATE_BACK_CW].mod  && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "B");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_BACK, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_BACK_180].key &&
        mods == conf.keys[KEY_ROTATE_BACK_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "B2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_BACK, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_BACK_CCW].key &&
        mods == conf.keys[KEY_ROTATE_BACK_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "B'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_BACK, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_DOWN_CW].key  &&
        mods == conf.keys[KEY_ROTATE_DOWN_CW].mod  && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "D");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_DOWN_180].key &&
        mods == conf.keys[KEY_ROTATE_DOWN_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "D2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_DOWN_CCW].key &&
        mods == conf.keys[KEY_ROTATE_DOWN_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "D'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_RIGHT_CW].key  &&
        mods == conf.keys[KEY_ROTATE_RIGHT_CW].mod  && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "R");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_RIGHT, ROTATION_CW,  0);
    }
    if (key  == conf.keys[KEY_ROTATE_RIGHT_180].key &&
        mods == conf.keys[KEY_ROTATE_RIGHT_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "R2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_RIGHT, ROTATION_180, 0);
    }
    if (key  == conf.keys[KEY_ROTATE_RIGHT_CCW].key &&
        mods == conf.keys[KEY_ROTATE_RIGHT_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "R'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_RIGHT, ROTATION_CCW, 0);
    }

    if (key  == conf.keys[KEY_ROTATE_M_CW].key  &&
        mods == conf.keys[KEY_ROTATE_M_CW].mod  && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "M");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_CW,  1);
    }
    if (key  == conf.keys[KEY_ROTATE_M_180].key &&
        mods == conf.keys[KEY_ROTATE_M_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "M2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_180, 1);
    }
    if (key  == conf.keys[KEY_ROTATE_M_CCW].key &&
        mods == conf.keys[KEY_ROTATE_M_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "M'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_LEFT, ROTATION_CCW, 1);
    }

    if (key  == conf.keys[KEY_ROTATE_E_CW].key  &&
        mods == conf.keys[KEY_ROTATE_E_CW].mod  && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "E");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_CW,  1);
    }
    if (key  == conf.keys[KEY_ROTATE_E_180].key &&
        mods == conf.keys[KEY_ROTATE_E_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "E2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_180, 1);
    }
    if (key  == conf.keys[KEY_ROTATE_E_CCW].key &&
        mods == conf.keys[KEY_ROTATE_E_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "E'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_DOWN, ROTATION_CCW, 1);
    }

    if (key  == conf.keys[KEY_ROTATE_S_CW].key  &&
        mods == conf.keys[KEY_ROTATE_S_CW].mod  && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "S");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_CW,  1);
    }
    if (key  == conf.keys[KEY_ROTATE_S_180].key &&
        mods == conf.keys[KEY_ROTATE_S_180].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "S2");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_180, 1);
    }
    if (key  == conf.keys[KEY_ROTATE_S_CCW].key &&
        mods == conf.keys[KEY_ROTATE_S_CCW].mod && action == KEY_PRESS) {
            
        text_opacity = 255.f;
        strcpy(move, "S'");
        text_opacity_anim = animate_scalar(&text_opacity, 0.f, 1.f, ease_out_sine);

        rubiks_cube_rotate_slice(rc, FACE_FRONT, ROTATION_CCW, 1);
    }
}

void window_size_callback(int width, int height)
{
    mat4_perspective_resize(&proj, (float)width / (float) height);
    view_proj = mat4_mul(proj, camera_get_view_matrix(cam));

    ortho = mat4_ortho(0.0f, width, 0.0f, height, conf.nearZ, conf.farZ);
    text_set_projection_matrix(ortho);
}

// TODO: maybe split updating and rendering into separate functions
// and implement a function update_frame inside window.c
void render_frame(void)
{
    float dt;
    dt = window_get_frame_time();

    rubiks_cube_update(rc, dt);
    camera_update(cam, dt);
    view_proj = mat4_mul(proj, camera_get_view_matrix(cam));

    window_clear();
    rubiks_cube_draw(rc, view_proj);

    int ww, wh;
    ww = window_get_width();
    wh = window_get_height();

    char text[9] = {0};
    sprintf(text, "FPS: %3u", (unsigned int) roundf(1.0f/dt));

    float ts; int tw, th; Vec2 tp; Color tc;
    ts = ww*2.5e-4;
    tw = get_text_width(ts, text);
    th = get_text_height(ts, text);
    tp = vec2(ww*0.01, ww*0.01 + th);
    tc = color_from_hex(0xFF4412FF);
    render_text(tp, ts, tc, text);

    update_animation(&text_opacity_anim, dt);
    if (animation_is_running(&text_opacity_anim)) {
        ts = ww*4e-4;
        tw = get_text_width(ts, move);
        th = get_text_height(ts, move);
        tp = vec2((ww - tw)*0.5f, wh*0.85 + th);
        tc = color_from_rgba(255, 68, 18, text_opacity);
        render_text(tp, ts, tc, move);
    }
}