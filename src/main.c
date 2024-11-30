#include "window.h"

void key_callback(int key, int action, int mods);

int main(void)
{
    init_window(800, 600, "Rubiks Cube Simulation", 1);
    set_key_callback(key_callback);

    while(!window_should_close()) {
        clear_window(color_from_hex(0xDFD3C3FF));
    }
    return 0;
}

void key_callback(int key, int action, int mods)
{
    (void) mods;

    if (key == KEY_ESCAPE && action == KEY_PRESS) {
        set_window_should_close(1);
    }
    if (key == KEY_F11 && action == KEY_PRESS) {
        toggle_fullscreen();
    }
}
