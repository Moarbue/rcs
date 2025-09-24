#ifndef _FONT_H_
#define _FONT_H_

#include <stdlib.h>

#include "color.h"
#include "glad/gl.h"
#include "mat.h"
#include "shader.h"
#include "vec.h"

#ifndef MAX_FONT_COUNT
#   define MAX_FONT_COUNT 5
#endif

typedef struct {
    unsigned int textureID;
    Vec2 size;
    Vec2 bearing;
    unsigned int advance;
} Glyph;

typedef struct {
    Glyph *glyphs;
    size_t glyph_count;
} Font;

void font_load(const char *font_path);
void font_delete(size_t font_index);

void init_text_renderer(const char *vertex_path, const char *fragment_path);
void render_text(Vec2 pos, float size, Color col, char *fmt, ...);
int  get_text_width(float size, char *fmt, ...);
int  get_text_height(float size, char *fmt, ...);
void text_set_projection_matrix(Mat4 proj);
void set_active_font(size_t font_index);
void delete_text_renderer(void);

#endif // _FONT_H_