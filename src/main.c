#include "window.h"

void key_callback(int key, int action, int mods);

int main(void)
{
    window_init(800, 600, "Rubiks Cube Simulation", 1);
    window_set_key_callback(key_callback);

    while(!window_should_close()) {
        window_clear(color_from_hex(0xDFD3C3FF));
    }

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
