#include "cube.h"

#include "logging.h"
#include "smath.h"

#include <inttypes.h>
#include <string.h>

void rotate_matrix_cw (uint64_t *a, uint64_t width, uint64_t height, uint64_t xstride, uint64_t ystride, uint64_t top_left);
void rotate_matrix_ccw(uint64_t *a, uint64_t width, uint64_t height, uint64_t xstride, uint64_t ystride, uint64_t top_left);
void rotate_matrix_180(uint64_t *a, uint64_t width, uint64_t height, uint64_t xstride, uint64_t ystride, uint64_t top_left);

Rubiks_Cube *rubiks_cube(Rubiks_Cube_Config *rcconf)
{
    Rubiks_Cube *rc;
    uint64_t corners, edges, center, w, h, d, ci, i;
    float cubie_spacer;
    Vec3 model_origin;
    Cubie_Config cconf;

    log_info("Generating new rubiks cube with dimensions %" PRIu64 "x%" PRIu64 "x%" PRIu64 "...", rcconf->width, rcconf->height, rcconf->depth);

    rc = (Rubiks_Cube *) calloc(1, sizeof (Rubiks_Cube));
    if (rc == NULL) {
        log_error("Failed to allocate memory for rubiks cube");
        return NULL;
    }

    // TODO: support different side lengths, by expanding cubie_indices array
    if (rcconf->width != rcconf->height || rcconf->width != rcconf->depth) {
        rc->w = rc->h = rc->d = u64max(rcconf->width, u64max(rcconf->height, rcconf->depth));
        log_warning("Only cubes with uniform side length are currently supported, changing side length to %" PRIu64, rc->w);
    } else {
        rc->w = rcconf->width;
        rc->h = rcconf->height;
        rc->d = rcconf->depth;
    }

    rc->cubie_indices = (uint64_t *) malloc(rc->w*rc->h*rc->d * sizeof (uint64_t));
    if (rc->cubie_indices == NULL) {
        log_error("Failed to allocate memory for cubie indices");
        rubiks_cube_free(rc);
        return NULL;
    }

    // TODO: maybe precompute rc->w - 2, etc.
    corners = 8;    // cube has always 8 corner cubies
    edges   = 4 * (rc->w - 2 + rc->h - 2 + rc->d - 2);  // side length - 2 corner stones * 4 edges per dimension
    center  = 2 * ((rc->w - 2)*(rc->h - 2) + (rc->w - 2)*(rc->d - 2) + (rc->h - 2)*(rc->d - 2)); // 6 smaller rectangles without corners and edges

    // Don't forget to change statement once variable side lengths are supported
    if (rc->w == 1)
        rc->cubie_count = 1;
    else
        rc->cubie_count = corners + edges + center;

    rc->cubies = (Cubie *) malloc(rc->cubie_count * sizeof (Cubie));
    if (rc->cubies == NULL) {
        log_error("Failed to allocate memory for cubies");
        rubiks_cube_free(rc);
        return NULL;
    }

    rc->pos   = rcconf->origin;
    rc->ori   = quat_identity();
    rc->scale = rcconf->scale;

    rc->prog = shader_new(rcconf->vertex_path, rcconf->fragment_path);
    if (rc->prog == NULL) {
        rubiks_cube_free(rc);
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

                i = d*rc->h*rc->w + h*rc->w + w;
                // Check if cubie is visible
                if (w > 0 && w < rc->w - 1 &&
                    h > 0 && h < rc->h - 1 &&
                    d > 0 && d < rc->d - 1) {
                        
                        rc->cubie_indices[i] = rc->cubie_count;
                        cconf.origin.x += cconf.side_length + cubie_spacer;
                        continue;
                    }
                    rc->cubie_indices[i] = ci;

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

void rubiks_cube_set_move_duration(Rubiks_Cube *rc, float duration)
{
    uint64_t ci;
    for (ci = 0; ci < rc->cubie_count; ci++)
        cubie_set_animation_duration(&rc->cubies[ci], duration);
}

void rubiks_cube_set_move_cooldownn(Rubiks_Cube *rc, float cooldown)
{
    rc->mcooldown = cooldown;
    rc->mc = cooldown;
}

void rubiks_cube_set_move_easing_func(Rubiks_Cube *rc, easing_func efunc)
{
    uint64_t ci;
    for (ci = 0; ci < rc->cubie_count; ci++)
        cubie_set_animation_easing_func(&rc->cubies[ci], efunc);
}

void rubiks_cube_rotate_slice(Rubiks_Cube *rc, Rubiks_Cube_Face face, Rubiks_Cube_Rotation rot, uint64_t top_left)
{
    Vec3 a;
    float r;
    uint64_t width, height, x, y, xstride, ystride, ci;

    if (top_left >= rc->d*rc->h*rc->w) {
        log_warning("top_left index is out of range...");
        return;
    }

    if (rc->mc < rc->mcooldown) return;
    rc->mc = 0.0f;

    // 90 degrees is counterclock-wise etc.
    r = M_PI_2 * (float)(rot+1);

    switch (face) {
        case FACE_FRONT:
            a = vec3(0.0f, 0.0f, 1.0f);
            width   = rc->w;
            height  = rc->h;
            xstride = 1;
            ystride = rc->w;
        break;

        case FACE_UP:
            a = vec3(0.0f, 1.0f, 0.0f);
            width   = rc->w;
            height  = rc->d;
            xstride = 1;
            ystride = -rc->w*rc->h;
        break;

        case FACE_LEFT:
            a = vec3(-1.0f, 0.0f, 0.0f);
            width   = rc->d;
            height  = rc->h;
            xstride = -rc->w*rc->h;
            ystride = rc->w;
        break;

        case FACE_BACK:
            a = vec3(0.0f, 0.0f, -1.0f);
            width   = rc->w;
            height  = rc->h;
            xstride = -1;
            ystride = rc->w;
        break;

        case FACE_DOWN:
            a = vec3(0.0f, -1.0f, 0.0f);
            width   = rc->w;
            height  = rc->d;
            xstride = 1;
            ystride = rc->w*rc->h;
        break;

        case FACE_RIGHT:
            a = vec3(1.0f, 0.0f, 0.0f);
            width   = rc->d;
            height  = rc->h;
            xstride = rc->w*rc->h;
            ystride = rc->w;
        break;
    }

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            ci = y * ystride + x * xstride + top_left;

            ci = rc->cubie_indices[ci];
            if (ci != rc->cubie_count)
                cubie_rotation_add(&rc->cubies[ci], a, r);
        }
    }

    switch (rot) {
        case ROTATION_CCW:
            rotate_matrix_ccw(rc->cubie_indices, width, height, xstride, ystride, top_left);
        break;
        
        case ROTATION_180:
            rotate_matrix_180(rc->cubie_indices, width, height, xstride, ystride, top_left);
        break;
        
        case ROTATION_CW:
            rotate_matrix_cw(rc->cubie_indices, width, height, xstride, ystride, top_left);
        break;
    }
}

void rubiks_cube_update(Rubiks_Cube *rc, float dt)
{
    uint64_t ci;
    for (ci = 0; ci < rc->cubie_count; ci++)
        cubie_update(&rc->cubies[ci], dt);
    
    if (rc->mc < rc->mcooldown) rc->mc += dt;
    else rc->mc = rc->mcooldown;
}

void rubiks_cube_draw(Rubiks_Cube *rc, Mat4 view_proj)
{
    uint64_t i;
    Mat4 m, mvp;

    m = mat4_translation(rc->pos);
    quat_rotatem4_at(&m, rc->ori, rc->pos);
    mat4_scale_s(&m, rc->scale);

    m = mat4_mul(view_proj, m);

    shader_bind(rc->prog);

    for (i = 0; i < rc->cubie_count; i++) {
        mvp = m;
        quat_rotatem4_at(&mvp, rc->cubies[i].ori, rc->pos);
        shader_set_uniform_mat4(rc->prog, "mvp", mvp.raw, GL_FALSE);

        glBindVertexArray(rc->cubies[i].vao);
        glDrawElements(GL_TRIANGLES, rc->cubies[i].index_count, GL_UNSIGNED_INT, NULL);
    }

    glBindVertexArray(0);
    shader_unbind(rc->prog);
}

void rubiks_cube_free(Rubiks_Cube *rc)
{
    uint64_t i;

    if (rc == NULL) return;

    if (rc->cubie_indices != NULL)
        free(rc->cubie_indices);

    for (i = 0; i < rc->cubie_count; i++) {
        cubie_free(rc->cubies[i]);
    }
    shader_free(rc->prog);

    if (rc->cubies != NULL)
        free(rc->cubies);

    free(rc);
}

void rotate_matrix_cw(uint64_t *a, uint64_t width, uint64_t height, uint64_t xstride, uint64_t ystride, uint64_t top_left)
{
    uint64_t x, y, tmp, i1, i2;

    // transpose matrix
    for (y = 0; y < height; y++) {
        for (x = y+1; x < width; x++) {
            // compute indices
            i1 = y*ystride + x*xstride + top_left;
            i2 = x*ystride + y*xstride + top_left;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }

    // change cols
    for (x = 0; x < width / 2; x++) {
        for (y = 0; y < width; y++) {
            // compute indices
            i1 = y*ystride +           x*xstride + top_left;
            i2 = y*ystride + (width-1-x)*xstride + top_left;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }
}

void rotate_matrix_ccw(uint64_t *a, uint64_t width, uint64_t height, uint64_t xstride, uint64_t ystride, uint64_t top_left)
{
    uint64_t x, y, tmp, i1, i2;

    // transpose matrix
    for (y = 0; y < height; y++) {
        for (x = y+1; x < width; x++) {
            // compute indices
            i1 = y*ystride + x*xstride + top_left;
            i2 = x*ystride + y*xstride + top_left;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }

    // change rows
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width; x++) {
            // compute indices
            i1 = y*ystride            + x*xstride + top_left;
            i2 = (height-1-y)*ystride + x*xstride + top_left;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }
}

void rotate_matrix_180(uint64_t *a, uint64_t width, uint64_t height, uint64_t xstride, uint64_t ystride, uint64_t top_left)
{
    uint64_t x, y, tmp, i1, i2;

    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width; x++) {
            // compute indices
            i1 = y*ystride            + x*xstride           + top_left;
            i2 = (height-1-y)*ystride + (width-1-x)*xstride + top_left;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }

    if (height % 2 == 0) return;

    // handle middle row
    for (x = 0; x < width / 2; x++) {
        // compute indices
        i1 = (height/2)*ystride   +           x*xstride + top_left;
        i2 = (height/2)*ystride   + (width-1-x)*xstride + top_left;

        // swap elements
        tmp   = a[i1];
        a[i1] = a[i2];
        a[i2] = tmp;
    }
}