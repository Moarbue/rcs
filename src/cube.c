#include "cube.h"

#include "logging.h"
#include "smath.h"

#include <inttypes.h>
#include <string.h>

Rubiks_Cube *rubiks_cube(Rubiks_Cube_Config *rcconf)
{
    Rubiks_Cube *rc;
    uint64_t corners, edges, center, w, h, d, ci;
    float cubie_spacer;
    Vec3 model_origin;
    Cubie_Config cconf;

    log_info("Generating new rubiks cube with dimensions %" PRIu64 "x%" PRIu64 "x%" PRIu64 "...", rcconf->width, rcconf->height, rcconf->depth);

    rc = (Rubiks_Cube *) malloc(sizeof (Rubiks_Cube));
    if (rc == NULL) {
        log_error("Failed to allocate memory for rubiks cube");
        return NULL;
    }

    rc->w = rcconf->width;
    rc->h = rcconf->height;
    rc->d = rcconf->depth;

    // TODO: maybe precompute rc->w - 2, etc.
    corners = 8;    // cube has always 8 corner cubies
    edges   = 4 * (rc->w - 2 + rc->h - 2 + rc->d - 2);  // side length - 2 corner stones * 4 edges per dimension
    center  = 2 * ((rc->w - 2)*(rc->h - 2) + (rc->w - 2)*(rc->d - 2) + (rc->h - 2)*(rc->d - 2)); // 6 smaller rectangles without corners and edges

    rc->cubie_count = corners + edges + center;
    rc->cubies = (Cubie *) malloc(rc->cubie_count * sizeof (Cubie));
    if (rc->cubies == NULL) {
        log_error("Failed to allocate memory for cubies");
        free(rc);
        return NULL;
    }

    rc->origin = rcconf->origin;
    rc->ori    = quat_identity();
    rc->scale  = rcconf->scale;

    rc->prog = shader_new(rcconf->vertex_path, rcconf->fragment_path);
    if (rc->prog == NULL) {
        free(rc->cubies);
        free(rc);
        return NULL;
    }

    // TODO: maybe make uniforms configurable through config and allow custom shaders etc.
    shader_register_uniform(rc->prog, "mvp");

    log_info("Generating cubies...");

    cconf.side_length = 1.0f / (float)u64max(rc->w, u64max(rc->h, rc->d));
    cubie_spacer = cconf.side_length * rcconf->cubie_spacer_multiplier;

    model_origin = vec3(
        -(cconf.side_length * rc->w + cubie_spacer * (rc->w - 1)) * 0.5f,
         (cconf.side_length * rc->h + cubie_spacer * (rc->h - 1)) * 0.5f,
         (cconf.side_length * rc->d + cubie_spacer * (rc->d - 1)) * 0.5f
    );
    cconf.origin = model_origin;

    cconf.face_length_multiplier = rcconf->face_length_multiplier;
    cconf.face_offset_from_cubie = rcconf->face_offset_from_cubie;

    memcpy(cconf.face_colors, rcconf->face_colors, CUBE_COLOR_COUNT * sizeof (Color));

    ci = 0;
    for (d = 0; d < rc->d; d++) {
        for (h = 0; h < rc->h; h++) {
            for (w = 0; w < rc->w; w++) {
                // Check if cubie is visible
                if (w > 0 && w < rc->w - 1 &&
                    h > 0 && h < rc->h - 1 &&
                    d > 0 && d < rc->d - 1) {
                        
                        cconf.origin.x += cconf.side_length + cubie_spacer;
                        continue;
                    }

                    cconf.color_mask = 0;

                    if (d == 0)         cconf.color_mask |= COLOR_MASK_FRONT;
                    if (h == 0)         cconf.color_mask |= COLOR_MASK_UP;
                    if (w == 0)         cconf.color_mask |= COLOR_MASK_LEFT;
                    if (d == rc->d - 1) cconf.color_mask |= COLOR_MASK_BACK;
                    if (h == rc->h - 1) cconf.color_mask |= COLOR_MASK_DOWN;
                    if (w == rc->w - 1) cconf.color_mask |= COLOR_MASK_RIGHT;
                    
                    rc->cubies[ci++] = cubie(&cconf);

                    cconf.origin.x += cconf.side_length + cubie_spacer;
            }

            cconf.origin.x  = model_origin.x;
            cconf.origin.y -= cconf.side_length + cubie_spacer;
        }

        cconf.origin.y  = model_origin.y;
        cconf.origin.z -= cconf.side_length + cubie_spacer;
    }

    log_info("Finished generating cubies");
    log_info("Finished generating rubiks cube");

    return rc;
}

void rubiks_cube_draw(Rubiks_Cube *rc, Mat4 view_proj)
{
    uint64_t i;
    Mat4 m, mvp;

    m = mat4_translation(rc->origin);
    quat_rotatem4_at(&m, rc->ori, rc->origin);
    mat4_scale_s(&m, rc->scale);

    mvp = mat4_mul(view_proj, m);

    shader_bind(rc->prog);

    for (i = 0; i < rc->cubie_count; i++) {
        mvp = mat4_mul(mvp, quat_to_mat4(rc->cubies[i].ori));
        shader_set_uniform_mat4(rc->prog, "mvp", mvp.raw, GL_FALSE);

        glBindVertexArray(rc->cubies[i].vao);
        glDrawElements(GL_TRIANGLES, rc->cubies[i].index_count, GL_UNSIGNED_INT, NULL);
    }

    glBindVertexArray(0);
    shader_unbind(rc->prog);
}

void rubiks_cube_delete(Rubiks_Cube *rc)
{
    uint64_t i;

    for (i = 0; i < rc->cubie_count; i++) {
        cubie_free(rc->cubies[i]);
    }
    shader_free(rc->prog);
    free(rc->cubies);
    free(rc);
}
