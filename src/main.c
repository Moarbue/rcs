#include "camera.h"
#include "cube.h"
#include "logging.h"
#include "smath.h"
#include "window.h"

void key_callback(int key, int action, int mods);
void window_size_callback(int width, int height);

void render_frame(void);

Mat4 proj, view_proj;
Rubiks_Cube *rc;
Camera *cam;

int main(void)
{
    set_logging_level(LOG_DEBUG);

    window_init(800, 600, "Rubiks Cube Simulation", 1);
    window_set_key_callback(key_callback);
    window_set_size_callback(window_size_callback);

    Rubiks_Cube_Config rcconf;
    rcconf.cubie_spacer_multiplier = 0.05f;
    rcconf.face_length_multiplier  = 0.95f;
    rcconf.face_offset_from_cubie  = 1e-3f;
    rcconf.vertex_path             = "shaders/cube.vert";
    rcconf.fragment_path           = "shaders/cube.frag";
    rcconf.scale                   = 1.0f;
    rcconf.origin                  = vec3(0, 0, 0);
    rcconf.width                   = 3;
    rcconf.height                  = 3;
    rcconf.depth                   = 3;
    rcconf.face_colors[0]          = color_from_hex(0x000000FF);
    rcconf.face_colors[1]          = color_from_hex(0xB90000FF);
    rcconf.face_colors[2]          = color_from_hex(0xFFD500FF);
    rcconf.face_colors[3]          = color_from_hex(0x0045ADFF);
    rcconf.face_colors[4]          = color_from_hex(0xFF5900FF);
    rcconf.face_colors[5]          = color_from_hex(0xFFFFFFFF);
    rcconf.face_colors[6]          = color_from_hex(0x009B48FF);

    rc = rubiks_cube(&rcconf);

    cam = camera(rcconf.origin, 3, rad(45), rad(0), rad(0));
    camera_set_animation_duration(cam, CAM_ANIM_ALL, 0.5f);
    camera_set_animation_easing_func(cam, CAM_ANIM_ALL, ease_in_out_sine);

    proj = mat4_perspective(rad(45.0f), window_get_aspect_ratio(), 0.01f, 100.0f);
    view_proj = mat4_mul(proj, camera_get_view_matrix(cam));

    window_main_loop(render_frame);

    rubiks_cube_free(rc);
    camera_free(cam);
    window_close();
    return 0;
}

void key_callback(int key, int action, int mods)
{
    (void) mods;

    if (key == KEY_ESCAPE && action == KEY_PRESS) {
        window_set_should_close(1);
    }
    if (key == KEY_F11 && action == KEY_PRESS) {
        window_toggle_fullscreen();
    }

    // Rotate +-90 degrees about X-Axis
    if (key == KEY_1 && action == KEY_PRESS) {
        camera_add_to_orbit_position(cam, 0.0f,  M_PI_2, 0.0f, 0.0f);
    }
    if (key == KEY_2 && action == KEY_PRESS) {
        camera_add_to_orbit_position(cam, 0.0f, -M_PI_2, 0.0f, 0.0f);
    }
    // Rotate +-90 degrees about Y-Axis
    if (key == KEY_3 && action == KEY_PRESS) {
        camera_add_to_orbit_position(cam, 0.0f, 0.0f,  M_PI_2, 0.0f);
    }
    if (key == KEY_4 && action == KEY_PRESS) {
        camera_add_to_orbit_position(cam, 0.0f, 0.0f, -M_PI_2, 0.0f);
    }
    // Rotate +-90 degrees about Z-Axis
    if (key == KEY_5 && action == KEY_PRESS) {
        camera_add_to_orbit_position(cam, 0.0f, 0.0f, 0.0f,  M_PI_2);
    }
    if (key == KEY_6 && action == KEY_PRESS) {
        camera_add_to_orbit_position(cam, 0.0f, 0.0f, 0.0f, -M_PI_2);
    }
}

void window_size_callback(int width, int height)
{
    mat4_perspective_resize(&proj, (float)width / (float) height);
    view_proj = mat4_mul(proj, camera_get_view_matrix(cam));
}

void render_frame(void)
{
    camera_update(cam, window_get_frame_time());
    view_proj = mat4_mul(proj, camera_get_view_matrix(cam));

    window_clear(color_from_hex(0xDFD3C3FF));
    rubiks_cube_draw(rc, view_proj);
}