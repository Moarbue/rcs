#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "color.h"
#include "cube_config.h"
#include "logging.h"
#include "vec.h"
#include "window.h"

typedef enum {
    // window controls
    KEY_WINDOW_CLOSE,
    KEY_TOGGLE_FULLSCREEN,
    // cube controls
    KEY_ROTATE_CUBE_POS_90_X,
    KEY_ROTATE_CUBE_NEG_90_X,
    KEY_ROTATE_CUBE_POS_90_Y,
    KEY_ROTATE_CUBE_NEG_90_Y,
    KEY_ROTATE_CUBE_POS_90_Z,
    KEY_ROTATE_CUBE_NEG_90_Z,
    // cube move controls
    KEY_ROTATE_FRONT_CW,
    KEY_ROTATE_FRONT_180,
    KEY_ROTATE_FRONT_CCW,

    KEY_ROTATE_UP_CW,
    KEY_ROTATE_UP_180,
    KEY_ROTATE_UP_CCW,

    KEY_ROTATE_LEFT_CW,
    KEY_ROTATE_LEFT_180,
    KEY_ROTATE_LEFT_CCW,

    KEY_ROTATE_BACK_CW,
    KEY_ROTATE_BACK_180,
    KEY_ROTATE_BACK_CCW,

    KEY_ROTATE_DOWN_CW,
    KEY_ROTATE_DOWN_180,
    KEY_ROTATE_DOWN_CCW,

    KEY_ROTATE_RIGHT_CW,
    KEY_ROTATE_RIGHT_180,
    KEY_ROTATE_RIGHT_CCW,

    KEY_ROTATE_M_CW,
    KEY_ROTATE_M_180,
    KEY_ROTATE_M_CCW,

    KEY_ROTATE_E_CW,
    KEY_ROTATE_E_180,
    KEY_ROTATE_E_CCW,

    KEY_ROTATE_S_CW,
    KEY_ROTATE_S_180,
    KEY_ROTATE_S_CCW,

    KEY_CONTROLS_COUNT,
} Key_Controls;

typedef struct {
    int key;
    int mod;
} Key_ShortCut;

typedef struct {
    FILE *logging_stream;           // where to output logging messages
    Logging_Level logging_level;    // logging level, see logging.h for all logging levels

    Key_ShortCut keys[KEY_CONTROLS_COUNT];  // all the keys which perform an action, see Key_Controls for all actions
    Color background_color;                 // background color of the window

    int fullscreen;                 // should the window start in fullscreen mode
    int default_window_width;       // if not fullscreen start out with this width
    int default_window_height;      // if not fullscreen start out with this height
    int fps;                        // the frames per second to render the game at

    float fov;                      // Field-Of-View, zoom-variable
    float nearZ;                    // near clipping plane, z-values smaller than this are clipped
    float farZ;                     // far clipping plane, z-values greater than this are clipped

    Vec3  cam_target;               // camera target, where the camera looks at, normally the cube origin
    float cam_distance;             // distance from camera target
    float cam_xrot;                 // camera rotation about X-Axis, rotation center is at target
    float cam_yrot;                 // camera rotation about Y-Axis, rotation center is at target
    float cam_zrot;                 // camera rotation about Z-Axis, rotation center is at target
    float cam_anim_duration;        // camera animation duration, includes position and orientation animation
    easing_func *cam_anim_efunc;    // camera easing function for animations, includes position and orientation animation

    Rubiks_Cube_Config rcconf;  // all configurations specific to the rubiks cube, look at cube_config.h for more information 

    char *font_vertex_path;     // path to vertex shader of text renderer
    char *font_fragment_path;   // path to fragment shader of text renderer
} Config;

Config config_default(void);

#endif // _CONFIG_H_