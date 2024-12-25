#include "config.h"

Config config_default(void)
{
    Config conf;

    conf.logging_stream = stderr;
    conf.logging_level  = LOG_INFO;

    conf.keys[KEY_WINDOW_CLOSE]         = (Key_ShortCut){KEY_ESCAPE, 0};
    conf.keys[KEY_TOGGLE_FULLSCREEN]    = (Key_ShortCut){KEY_F11,    0};
    conf.keys[KEY_ROTATE_CUBE_POS_90_X] = (Key_ShortCut){KEY_3,      0};
    conf.keys[KEY_ROTATE_CUBE_NEG_90_X] = (Key_ShortCut){KEY_4,      0};
    conf.keys[KEY_ROTATE_CUBE_POS_90_Y] = (Key_ShortCut){KEY_1,      0};
    conf.keys[KEY_ROTATE_CUBE_NEG_90_Y] = (Key_ShortCut){KEY_2,      0};
    conf.keys[KEY_ROTATE_CUBE_POS_90_Z] = (Key_ShortCut){KEY_5,      0};
    conf.keys[KEY_ROTATE_CUBE_NEG_90_Z] = (Key_ShortCut){KEY_6,      0};
    conf.keys[KEY_ROTATE_FRONT_CW]      = (Key_ShortCut){KEY_F,      0};
    conf.keys[KEY_ROTATE_FRONT_180]     = (Key_ShortCut){KEY_F,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_FRONT_CCW]     = (Key_ShortCut){KEY_F,      MOD_CONTROL};
    conf.keys[KEY_ROTATE_UP_CW]         = (Key_ShortCut){KEY_U,      0};
    conf.keys[KEY_ROTATE_UP_180]        = (Key_ShortCut){KEY_U,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_UP_CCW]        = (Key_ShortCut){KEY_U,      MOD_CONTROL};
    conf.keys[KEY_ROTATE_LEFT_CW]       = (Key_ShortCut){KEY_L,      0};
    conf.keys[KEY_ROTATE_LEFT_180]      = (Key_ShortCut){KEY_L,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_LEFT_CCW]      = (Key_ShortCut){KEY_L,      MOD_CONTROL};
    conf.keys[KEY_ROTATE_BACK_CW]       = (Key_ShortCut){KEY_B,      0};
    conf.keys[KEY_ROTATE_BACK_180]      = (Key_ShortCut){KEY_B,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_BACK_CCW]      = (Key_ShortCut){KEY_B,      MOD_CONTROL};
    conf.keys[KEY_ROTATE_DOWN_CW]       = (Key_ShortCut){KEY_D,      0};
    conf.keys[KEY_ROTATE_DOWN_180]      = (Key_ShortCut){KEY_D,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_DOWN_CCW]      = (Key_ShortCut){KEY_D,      MOD_CONTROL};
    conf.keys[KEY_ROTATE_RIGHT_CW]      = (Key_ShortCut){KEY_R,      0};
    conf.keys[KEY_ROTATE_RIGHT_180]     = (Key_ShortCut){KEY_R,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_RIGHT_CCW]     = (Key_ShortCut){KEY_R,      MOD_CONTROL};
    conf.keys[KEY_ROTATE_M_CW]          = (Key_ShortCut){KEY_M,      0};
    conf.keys[KEY_ROTATE_M_180]         = (Key_ShortCut){KEY_M,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_M_CCW]         = (Key_ShortCut){KEY_M,      MOD_CONTROL};
    conf.keys[KEY_ROTATE_E_CW]          = (Key_ShortCut){KEY_E,      0};
    conf.keys[KEY_ROTATE_E_180]         = (Key_ShortCut){KEY_E,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_E_CCW]         = (Key_ShortCut){KEY_E,      MOD_CONTROL};
    conf.keys[KEY_ROTATE_S_CW]          = (Key_ShortCut){KEY_S,      0};
    conf.keys[KEY_ROTATE_S_180]         = (Key_ShortCut){KEY_S,      MOD_SHIFT};
    conf.keys[KEY_ROTATE_S_CCW]         = (Key_ShortCut){KEY_S,      MOD_CONTROL};

    conf.background_color = color_from_hex(0xDFD3C3FF);

    conf.fullscreen            = 1;
    conf.default_window_width  = 800;
    conf.default_window_height = 600;

    conf.default_fov   = rad(45.0f);
    conf.default_nearZ = 0.01f;
    conf.default_farZ  = 100.0f;

    conf.default_cam_target        = vec3s(0.0f);
    conf.default_cam_distance      = 3.0f;
    conf.default_cam_xrot          = rad(30.0f);
    conf.default_cam_yrot          = rad(45.0f);
    conf.default_cam_zrot          = rad( 0.0f);
    conf.default_cam_anim_duration = 0.5f;
    conf.default_cam_anim_efunc    = ease_in_out_sine;

    conf.rcconf.cubie_spacer_multiplier   = 0.0f;
    conf.rcconf.face_length_multiplier    = 0.92f;
    conf.rcconf.face_offset_from_cubie    = 1e-3f;
    conf.rcconf.vertex_path               = "shaders/cube.vert";
    conf.rcconf.fragment_path             = "shaders/cube.frag";
    conf.rcconf.scale                     = 1.0f;
    conf.rcconf.origin                    = vec3s(0.0f);
    conf.rcconf.width                     = 3;
    conf.rcconf.height                    = 3;
    conf.rcconf.depth                     = 3;
    conf.rcconf.wobble_height             = 0.05f;
    conf.rcconf.wobble_duration           = 1.3f;
    conf.rcconf.wobble_efunc              = ease_in_out_sine;
    conf.rcconf.scale_duration            = 0.5f;
    conf.rcconf.scale_efunc               = ease_in_out_sine;
    conf.rcconf.ori_duration              = 0.5f;
    conf.rcconf.ori_efunc                 = ease_in_out_sine;
    conf.rcconf.move_duration             = 0.5f;
    conf.rcconf.move_cooldown             = 0.2f;
    conf.rcconf.move_easing_func          = ease_in_out_sine;
    conf.rcconf.face_colors[COLOR_BORDER] = color_from_hex(0x000000FF);
    conf.rcconf.face_colors[COLOR_FRONT]  = color_from_hex(0xB90000FF);
    conf.rcconf.face_colors[COLOR_UP]     = color_from_hex(0xFFD500FF);
    conf.rcconf.face_colors[COLOR_LEFT]   = color_from_hex(0x0045ADFF);
    conf.rcconf.face_colors[COLOR_BACK]   = color_from_hex(0xFF5900FF);
    conf.rcconf.face_colors[COLOR_DOWN]   = color_from_hex(0xFFFFFFFF);
    conf.rcconf.face_colors[COLOR_RIGHT]  = color_from_hex(0x009B48FF);

    return conf;
}