#include "cube.h"
#include "logging.h"
#include "smath.h"
#include "window.h"

void key_callback(int key, int action, int mods);

Mat4 view, proj, view_proj;

int main(void)
{
    set_logging_level(LOG_INFO);

    window_init(800, 600, "Rubiks Cube Simulation", 1);
    window_set_key_callback(key_callback);

    Rubiks_Cube *rc;
    Rubiks_Cube_Config rcconf;
    rcconf.cubie_spacer_multiplier = 0.05f;
    rcconf.face_length_multiplier  = 0.95f;
    rcconf.face_offset_from_cubie  = 1e-3f;
    rcconf.vertex_path             = "shaders/cube.vert";
    rcconf.fragment_path           = "shaders/cube.frag";
    rcconf.scale                   = 1.0f;
    rcconf.origin                  = vec3s(0);
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

    // view = mat4_look_at(vec3_from_spherical(3.0f, rad(0.0f), rad(0.0f)), rcconf.origin, vec3(0.0f, 1.0f, 0.0f));
    view = mat4_look_at(vec3(0, 0, 0), rcconf.origin, vec3(0.0f, 1.0f, 0.0f));
    proj = mat4_perspective(rad(45.0f), window_get_aspect_ratio(), 0.01f, 100.0f);
    view_proj = mat4_mul(proj, view);

    while(!window_should_close()) {
        window_clear(color_from_hex(0xDFD3C3FF));

        rubiks_cube_draw(rc, view_proj);
    }

    rubiks_cube_delete(rc);
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
}
