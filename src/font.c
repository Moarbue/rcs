#include "font.h"
#include "ft2build.h"
#include "freetype/freetype.h"
#include "logging.h"
#include "smath.h"
#include "util.h"
#include "vertex.h"

#include <stdarg.h>

typedef struct {
    Font fonts[MAX_FONT_COUNT];
    size_t font_count;
    size_t active_font;
    GLuint vao, vbo;
    Shader_Program *prog;
    Mat4 proj;
} TextRenderer;

const size_t CHAR_COUNT = 128;
TextRenderer tr = {0};

void init_text_renderer(const char *vertex_path, const char *fragment_path)
{
    log_info("Initializing text renderer...");

    glGenVertexArrays(1, &tr.vao);
    glGenBuffers(1, &tr.vbo);

    glBindVertexArray(tr.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, tr.vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof (Font_Vertex), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(FONT_VERTEX_POS, 2, GL_FLOAT, GL_FALSE, sizeof (Font_Vertex), (void *) offsetof(Font_Vertex, pos));
    glEnableVertexAttribArray(FONT_VERTEX_POS);

    glVertexAttribPointer(FONT_VERTEX_TEX, 2, GL_FLOAT, GL_FALSE, sizeof (Font_Vertex), (void *) offsetof(Font_Vertex, tex));
    glEnableVertexAttribArray(FONT_VERTEX_TEX);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    log_info("Created Vertex Array and Array Buffer for font rendering");

    tr.proj = mat4_ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.01f, 100.0f);
    tr.font_count = 0;
    tr.active_font = 0;
    tr.prog = shader_new(vertex_path, fragment_path);
    if (tr.prog == NULL) {
        log_error_and_exit(1, "Failed to load font shaders");
    }

    shader_register_uniform(tr.prog, "proj");
    shader_register_uniform(tr.prog, "text");
    shader_register_uniform(tr.prog, "textColor");
}

void font_load(const char *font_path)
{
    if (tr.font_count+1 > MAX_FONT_COUNT) {
        log_error("Maximum amount of fonts (%d) already loaded, cannot load %s", MAX_FONT_COUNT, font_path);
        return;
    }

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        log_error_and_exit(1, "Failed to load Freetype2 library");
    }

    
    log_info("Loading font from file %s...", font_path);

    // TODO: Load more than just the ASCII characters

    // Create new font object
    tr.fonts[tr.font_count].glyphs = (Glyph *) malloc(CHAR_COUNT * sizeof (Glyph));
    if (tr.fonts[tr.font_count].glyphs == NULL) {
        // only abort if no other fonts are available
        if (tr.font_count == 0)
            log_error_and_exit(1, "Failed to create font, aborting since no other fonts are loaded");
        else {
            log_error("Failed to create font, falling back to default font");
            return;
        }
    }
    tr.fonts[tr.font_count].glyph_count = 0;

    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face)) {
        // only abort if no other fonts are available
        if (tr.font_count == 0)
            log_error_and_exit(1, "Failed to load font, aborting since no other fonts are loaded");
        else {
            log_error("Failed to load font, falling back to default font");
            return;
        }
    }


    // default font size is 32pt
    FT_Set_Char_Size(face, 0, 128*64, 0, 0);

    log_info("Loading font characters...");

    for (unsigned char c = 0; c < CHAR_COUNT; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            log_warning("Failed to load character \'%c\'", (char)c);
            continue;
        }

        // generate the texture for rendering
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        tr.fonts[tr.font_count].glyphs[(size_t) c] = (Glyph) {
            .textureID = texture,
            .size      = vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            .bearing   = vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            .advance   = face->glyph->advance.x
        };
    }
    tr.fonts[tr.font_count].glyph_count = CHAR_COUNT;
    tr.font_count++;

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    log_info("Finished loading font");
}

void font_delete(size_t font_index)
{
    if (font_index >= tr.font_count) {
        log_error("Cannot delete font, index out of range: %zu >= %zu (font count)", font_index, tr.font_count);
        return;
    };

    for (size_t i = 0; i < CHAR_COUNT; i++) {
        glDeleteTextures(1, &tr.fonts[font_index].glyphs[i].textureID);
    }
    free(tr.fonts[font_index].glyphs);
    tr.fonts[font_index].glyphs = 0;
    tr.font_count--;

    log_info("Deleted font with index %zu", font_index);
}

void render_text(Vec2 pos, float size, Color col, char *fmt, ...)
{
    if (tr.font_count == 0) {
        log_error("Cannot render text, no font is loaded");
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    
    shader_bind(tr.prog);
    shader_set_uniform_color(tr.prog, "textColor", col);
    glBindVertexArray(tr.vao);


    char buf[2048] = {0};
    va_list args;
    va_start(args, fmt);  
    vsprintf(buf, fmt, args);
    va_end(args);


    size_t len;
    len = strnlen(buf, 1024);
    // TODO: maybe check if string is null-terminated
    // TODO: accumulate vertices inside a buffer and do batch-rendering for better performance, instead of calling a draw-function
    // for each character
    for (size_t i = 0; i < len; i++) {
        Glyph g;
        g = tr.fonts[tr.active_font].glyphs[(int) buf[i]];

        float xpos = pos.x + g.bearing.x * size;
        float ypos = pos.y + (g.size.y - g.bearing.y) * size;

        float w =  g.size.x * size;
        float h = -g.size.y * size;
        // update VBO for each character
        Font_Vertex vertices[6] = {
            {.pos = (Vec2) {xpos,     ypos + h}, .tex = (Vec2) {0.0f, 0.0f}},
            {.pos = (Vec2) {xpos,     ypos    }, .tex = (Vec2) {0.0f, 1.0f}},
            {.pos = (Vec2) {xpos + w, ypos    }, .tex = (Vec2) {1.0f, 1.0f}},
            {.pos = (Vec2) {xpos,     ypos + h}, .tex = (Vec2) {0.0f, 0.0f}},
            {.pos = (Vec2) {xpos + w, ypos    }, .tex = (Vec2) {1.0f, 1.0f}},
            {.pos = (Vec2) {xpos + w, ypos + h}, .tex = (Vec2) {1.0f, 0.0f}},
        };
        
        shader_set_uniform_sampler2D(tr.prog, "text", g.textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, tr.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (vertices), vertices); 
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        pos.x += (g.advance >> 6) * size; // bitshift by 6 to get value in pixels (2^6 = 64):
    }

    glBindVertexArray(0);
    shader_unbind(tr.prog);
    glDisable(GL_BLEND);
}

// TODO: this code uses a lot of copied code, refactoring is due
int get_text_width(float size, char *fmt, ...)
{
    if (tr.font_count == 0) {
        log_error("Cannot calculate text width, no font is loaded");
        return 0;
    }

    char buf[2048] = {0};
    va_list args;
    va_start(args, fmt);  
    vsprintf(buf, fmt, args);
    va_end(args);


    float width;
    width = 0.f;
    size_t len;
    len = strnlen(buf, 1024);
    for (size_t i = 0; i < len; i++) {
        Glyph g;
        g = tr.fonts[tr.active_font].glyphs[(int) buf[i]];

        width += (g.advance >> 6) * size;
    }

    return (int) width;
}

int get_text_height(float size, char *fmt, ...)
{
    if (tr.font_count == 0) {
        log_error("Cannot calculate text height, no font is loaded");
        return 0;
    }

    char buf[2048] = {0};
    va_list args;
    va_start(args, fmt);  
    vsprintf(buf, fmt, args);
    va_end(args);


    float height;
    height = 0.f;
    size_t len;
    len = strnlen(buf, 1024);
    for (size_t i = 0; i < len; i++) {
        Glyph g;
        g = tr.fonts[tr.active_font].glyphs[(int) buf[i]];

        height = max(height, g.size.y * size);
    }

    return (int) height;
}

void text_set_projection_matrix(Mat4 proj)
{
    log_debug("Set text projection matrix");
    tr.proj = proj;
    shader_set_uniform_mat4(tr.prog, "proj", tr.proj.raw, GL_FALSE);
}

void set_active_font(size_t font_index)
{
    if (font_index >= tr.font_count) {
        log_error("Cannot activate font, index out of range: %zu >= %zu (font count)", font_index, tr.font_count);
        return;
    };

    log_info("Set active font index to %zu", font_index);

    tr.active_font = font_index;
}

void delete_text_renderer(void)
{
    shader_free(tr.prog);

    glDeleteVertexArrays(1, &tr.vao);
    glDeleteBuffers(1, &tr.vbo);

    for (size_t i = 0; i < tr.font_count; i++) {
        free(tr.fonts[i].glyphs);
    }
}