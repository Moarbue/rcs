#include "cube.h"

#include "logging.h"
#include "smath.h"

#include <inttypes.h>
#include <string.h>

void rotate_matrix_cw (uint64_t *a, uint64_t dimension, uint64_t xstride, uint64_t ystride, uint64_t start_index);
void rotate_matrix_ccw(uint64_t *a, uint64_t dimension, uint64_t xstride, uint64_t ystride, uint64_t start_index);
void rotate_matrix_180(uint64_t *a, uint64_t dimension, uint64_t xstride, uint64_t ystride, uint64_t start_index);

Rubiks_Cube *rubiks_cube(Rubiks_Cube_Config *rcconf)
{
    Rubiks_Cube *rc;
    uint64_t cw, ch, cd, w, h, d, ci, i;
    float cubie_spacer;
    Vec3 model_origin;
    Cubie_Config cconf;

    log_info("Generating new rubiks cube with dimensions %" PRIu64 "x%" PRIu64 "x%" PRIu64 "...", rcconf->width, rcconf->height, rcconf->depth);

    rc = (Rubiks_Cube *) calloc(1, sizeof (Rubiks_Cube));
    if (rc == NULL) {
        log_error("Failed to allocate memory for rubiks cube");
        return NULL;
    }

    rc->w = rcconf->width;
    rc->h = rcconf->height;
    rc->d = rcconf->depth;

    // take the longest side to ensure rotation always works, non existent indices are filled with dummy indices
    rc->max_length = u64max(rc->w, u64max(rc->h, rc->d));

    rc->cubie_indices = (uint64_t *) malloc(rc->max_length*rc->max_length*rc->max_length * sizeof (uint64_t));
    if (rc->cubie_indices == NULL) {
        log_error("Failed to allocate memory for cubie indices");
        rubiks_cube_free(rc);
        return NULL;
    }

    rc->cubie_count = rc->w*rc->h*rc->d;
    // there are cubies that aren't visible so subtract them from the total count
    if (rc->w > 2 && rc->h > 2 && rc->d > 2) {
        cw = rc->w - 2; ch = rc->h - 2; cd = rc->d - 2;
        rc->cubie_count -= cw*ch*cd;
    }

    // TODO: when rotating width/height/depth changes layer-wise, so cube needs to store dimensions layer-wise
    if (rc->w != rc->h || rc->w != rc->d) {
        log_warning("Variable side lengths are only supported experimentally, rotating will not work as intended!");
    }

    rc->cubies = (Cubie *) malloc(rc->cubie_count * sizeof (Cubie));
    if (rc->cubies == NULL) {
        log_error("Failed to allocate memory for cubies");
        rubiks_cube_free(rc);
        return NULL;
    }

    rc->pos   = rcconf->origin;
    rc->ori   = quat_identity();
    rc->scale = rcconf->scale;

    rc->wobble_height = rcconf->wobble_height;
    rc->wobble_anim = (Animation) {0};
    rc->wobble_anim.duration = rcconf->wobble_duration;
    rc->wobble_anim.efunc = rcconf->wobble_efunc;

    rc->scale_anim = (Animation) {0};
    rc->scale_anim.duration = rcconf->scale_duration;
    rc->scale_anim.efunc = rcconf->scale_efunc;

    rc->ori_anim = (Animation) {0};
    rc->ori_anim.duration = rcconf->ori_duration;
    rc->ori_anim.efunc = rcconf->ori_efunc;
    rc->ori_anim.data.q.end = quat_identity();

    rc->prog = shader_new(rcconf->vertex_path, rcconf->fragment_path);
    if (rc->prog == NULL) {
        rubiks_cube_free(rc);
        return NULL;
    }

    // TODO: maybe make uniforms configurable through config and allow custom shaders etc.
    shader_register_uniform(rc->prog, "mvp");

    log_info("Generating cubies...");

    cconf.side_length = 1.0f / (float)rc->max_length;
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

    for (i = 0; i < rc->max_length*rc->max_length*rc->max_length; i++)
        rc->cubie_indices[i] = rc->cubie_count;

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
                    
                i = d*rc->h*rc->w + h*rc->w + w;
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

    rubiks_cube_set_move_duration(rc, rcconf->move_duration);
    rubiks_cube_set_move_cooldownn(rc, rcconf->move_cooldown);
    rubiks_cube_set_move_easing_func(rc, rcconf->move_easing_func);


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

void rubiks_cube_rotate_slice(Rubiks_Cube *rc, Rubiks_Cube_Face face, Rubiks_Cube_Rotation rot, uint64_t slice)
{
    Vec3 a;
    float r;
    uint64_t start_index, width, height, x, y, xstride, ystride, ci;

    if (rc->mc < rc->mcooldown) return;
    rc->mc = 0.0f;

    // 90 degrees is counterclock-wise etc.
    r = M_PI_2 * (float)(rot+1);

    switch (face) {
        case FACE_FRONT:
            if (slice >= rc->d) {
                log_warning("Slice index is out of range %" PRIu64 " > %" PRIu64 ", skipping Front rotation", slice, rc->d);
                return;
            }
            start_index = slice*rc->w*rc->h;
            a = vec3(0.0f, 0.0f, 1.0f);
            width   = rc->w;
            height  = rc->h;
            xstride = 1;
            ystride = rc->w;
        break;

        case FACE_UP:
            if (slice >= rc->h) {
                log_warning("Slice index is out of range %" PRIu64 " > %" PRIu64 ", skipping Up rotation", slice, rc->h);
                return;
            }
            start_index = (rc->d-1)*rc->h*rc->w + slice*rc->w;
            a = vec3(0.0f, 1.0f, 0.0f);
            width   = rc->w;
            height  = rc->d;
            xstride = 1;
            ystride = -rc->w*rc->h;
        break;

        case FACE_LEFT:
            if (slice >= rc->w) {
                log_warning("Slice index is out of range %" PRIu64 " > %" PRIu64 ", skipping Left rotation", slice, rc->w);
                return;
            }
            start_index = (rc->d-1)*rc->h*rc->w + slice;
            a = vec3(-1.0f, 0.0f, 0.0f);
            width   = rc->d;
            height  = rc->h;
            xstride = -rc->w*rc->h;
            ystride = rc->w;
        break;

        case FACE_BACK:
            if (slice >= rc->d) {
                log_warning("Slice index is out of range %" PRIu64 " > %" PRIu64 ", skipping Back rotation", slice, rc->d);
                return;
            }
            start_index = (rc->d-1-slice)*rc->h*rc->w + (rc->w-1);
            a = vec3(0.0f, 0.0f, -1.0f);
            width   = rc->w;
            height  = rc->h;
            xstride = -1;
            ystride = rc->w;
        break;

        case FACE_DOWN:
            if (slice >= rc->h) {
                log_warning("Slice index is out of range %" PRIu64 " > %" PRIu64 ", skipping Down rotation", slice, rc->h);
                return;
            }
            start_index = (rc->h-1-slice)*(rc->w);
            a = vec3(0.0f, -1.0f, 0.0f);
            width   = rc->w;
            height  = rc->d;
            xstride = 1;
            ystride = rc->w*rc->h;
        break;

        case FACE_RIGHT:
            if (slice >= rc->w) {
                log_warning("Slice index is out of range %" PRIu64 " > %" PRIu64 ", skipping Right rotation", slice, rc->w);
                return;
            }
            start_index = (rc->w-1-slice);
            a = vec3(1.0f, 0.0f, 0.0f);
            width   = rc->d;
            height  = rc->h;
            xstride = rc->w*rc->h;
            ystride = rc->w;
        break;
    }

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            ci = y * ystride + x * xstride + start_index;

            ci = rc->cubie_indices[ci];
            if (ci != rc->cubie_count)
                cubie_rotation_add(&rc->cubies[ci], a, r);
        }
    }

    switch (face) {
        case FACE_FRONT:
            start_index = slice*rc->max_length*rc->max_length;
            xstride = 1;
            ystride = rc->max_length;
        break;

        case FACE_UP:
            start_index = (rc->max_length-1)*rc->max_length*rc->max_length + slice*rc->max_length;
            xstride = 1;
            ystride = -rc->max_length*rc->max_length;
        break;

        case FACE_LEFT:
            start_index = (rc->max_length-1)*rc->max_length*rc->max_length + slice;
            xstride = -rc->max_length*rc->max_length;
            ystride = rc->max_length;
        break;

        case FACE_BACK:
            start_index = (rc->max_length-1-slice)*rc->max_length*rc->max_length + (rc->max_length-1);
            xstride = -1;
            ystride = rc->max_length;
        break;

        case FACE_DOWN:
            start_index = (rc->max_length-1-slice)*(rc->max_length);
            xstride = 1;
            ystride = rc->max_length*rc->max_length;
        break;

        case FACE_RIGHT:
            start_index = (rc->max_length-1-slice);
            xstride = rc->max_length*rc->max_length;
            ystride = rc->max_length;
        break;
    }

    switch (rot) {
        case ROTATION_CCW:
            rotate_matrix_ccw(rc->cubie_indices, rc->max_length, xstride, ystride, start_index);
        break;
        
        case ROTATION_180:
            rotate_matrix_180(rc->cubie_indices, rc->max_length, xstride, ystride, start_index);
        break;
        
        case ROTATION_CW:
            rotate_matrix_cw (rc->cubie_indices, rc->max_length, xstride, ystride, start_index);
        break;
    }
}

void rubiks_cube_rotate(Rubiks_Cube *rc, Vec3 axis, float angle)
{
    rc->ori_anim = animate_quaternion(
        &rc->ori,
        quat_mul(rc->ori_anim.data.q.end, quat_from_axis_angle(axis, angle)),
        rc->ori_anim.duration,
        rc->ori_anim.efunc
    );
}

void rubiks_cube_scale(Rubiks_Cube *rc, float scale)
{
    rc->scale_anim = animate_scalar(
        &rc->scale,
        scale,
        rc->scale_anim.duration,
        rc->scale_anim.efunc
    );
}

void rubiks_cube_update(Rubiks_Cube *rc, float dt)
{
    uint64_t ci;
    for (ci = 0; ci < rc->cubie_count; ci++)
        cubie_update(&rc->cubies[ci], dt);
    
    if (rc->mc < rc->mcooldown) rc->mc += dt;
    else rc->mc = rc->mcooldown;

    if (!animation_is_running(&rc->wobble_anim)) {
        rc->wobble_anim = animate_vector3(
            &rc->pos,
            vec3_sub(rc->pos, vec3(0, rc->wobble_height, 0)),
            rc->wobble_anim.duration,
            rc->wobble_anim.efunc
        );

        rc->wobble_height *= -1.0f;
    }

    update_animation(&rc->wobble_anim, dt);
    update_animation(&rc->ori_anim, dt);
    update_animation(&rc->scale_anim, dt);
}

void rubiks_cube_draw(Rubiks_Cube *rc, Mat4 view_proj)
{
    uint64_t i;
    Mat4 m, mvp;

    m = mat4_translation(rc->pos);
    quat_rotatem4(&m, rc->ori);
    mat4_scale_s(&m, rc->scale);

    m = mat4_mul(view_proj, m);

    // 3D Options
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    shader_bind(rc->prog);

    for (i = 0; i < rc->cubie_count; i++) {
        mvp = mat4_copy(m);
        quat_rotatem4(&mvp, rc->cubies[i].ori);
        shader_set_uniform_mat4(rc->prog, "mvp", mvp.raw, GL_FALSE);

        glBindVertexArray(rc->cubies[i].vao);
        glDrawElements(GL_TRIANGLES, rc->cubies[i].index_count, GL_UNSIGNED_INT, NULL);
    }

    glBindVertexArray(0);
    shader_unbind(rc->prog);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
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

void rotate_matrix_cw(uint64_t *a, uint64_t dimension, uint64_t xstride, uint64_t ystride, uint64_t start_index)
{
    uint64_t x, y, tmp, i1, i2;

    // transpose matrix
    for (y = 0; y < dimension; y++) {
        for (x = y+1; x < dimension; x++) {
            // compute indices
            i1 = y*ystride + x*xstride + start_index;
            i2 = x*ystride + y*xstride + start_index;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }

    // change cols
    for (x = 0; x < dimension / 2; x++) {
        for (y = 0; y < dimension; y++) {
            // compute indices
            i1 = y*ystride +               x*xstride + start_index;
            i2 = y*ystride + (dimension-1-x)*xstride + start_index;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }
}

void rotate_matrix_ccw(uint64_t *a, uint64_t dimension, uint64_t xstride, uint64_t ystride, uint64_t start_index)
{
    uint64_t x, y, tmp, i1, i2;

    // transpose matrix
    for (y = 0; y < dimension; y++) {
        for (x = y+1; x < dimension; x++) {
            // compute indices
            i1 = y*ystride + x*xstride + start_index;
            i2 = x*ystride + y*xstride + start_index;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }

    // change rows
    for (y = 0; y < dimension / 2; y++) {
        for (x = 0; x < dimension; x++) {
            // compute indices
            i1 = y*ystride               + x*xstride + start_index;
            i2 = (dimension-1-y)*ystride + x*xstride + start_index;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }
}

void rotate_matrix_180(uint64_t *a, uint64_t dimension, uint64_t xstride, uint64_t ystride, uint64_t start_index)
{
    uint64_t x, y, tmp, i1, i2;

    for (y = 0; y < dimension / 2; y++) {
        for (x = 0; x < dimension; x++) {
            // compute indices
            i1 = y*ystride               +               x*xstride + start_index;
            i2 = (dimension-1-y)*ystride + (dimension-1-x)*xstride + start_index;

            // swap elements
            tmp   = a[i1];
            a[i1] = a[i2];
            a[i2] = tmp;
        }
    }

    if (dimension % 2 == 0) return;

    // handle middle row
    for (x = 0; x < dimension / 2; x++) {
        // compute indices
        i1 = (dimension/2)*ystride   +               x*xstride + start_index;
        i2 = (dimension/2)*ystride   + (dimension-1-x)*xstride + start_index;

        // swap elements
        tmp   = a[i1];
        a[i1] = a[i2];
        a[i2] = tmp;
    }
}