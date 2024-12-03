#include "cubie.h"

#include "logging.h"
#include "smath.h"

#include <inttypes.h>
#include <malloc.h>
#include <stddef.h>

// unit cube, length = 1, origin at top left corner 
const Vec3 cubie_coords[] = {
    {0.0f,  0.0f,  0.0f}, {0.0f, -1.0f,  0.0f}, {1.0f, -1.0f,  0.0f}, {1.0f,  0.0f,  0.0f}, 
    {0.0f,  0.0f, -1.0f}, {0.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {1.0f,  0.0f, -1.0f},
};

// triangle indices inside the cubie_coords array
const uint8_t cubie_indices[] = {
    0, 1, 3,    1, 2, 3,    // front
    4, 5, 0,    5, 1, 0,    // up
    4, 0, 7,    0, 3, 7,    // left
    5, 4, 6,    4, 7, 6,    // back
    3, 2, 7,    2, 6, 7,    // down
    1, 5, 2,    5, 6, 2,    // right
};

// coordinates for faces, length = 1, origin at top left corner
const Vec3 face_coords[] = {
    // xy-plane
    {0.0f, 0.0f,  0.0f}, {0.0f, -1.0f,  0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 0.0f,  0.0f},
    // xz-plane
    {0.0f, 0.0f, -1.0f}, {0.0f,  0.0f,  0.0f}, {1.0f,  0.0f, 0.0f}, {1.0f, 0.0f, -1.0f},
    // yz-plane
    {0.0f, 0.0f, -1.0f}, {0.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f,  0.0f},
};

// triangle indices inside the face_coords array, that are later offset to the corresponding plane
// since they are added after the cubie coords, they are offset by the amount of cubie_coords
// clock and counter-clockwise so OpenGL doesn't cull the back-faces
const uint8_t face_indices[] = {
    // counter-clockwise (front, up, left)
    0 + ARRAY_LENGTH(cubie_coords), 1 + ARRAY_LENGTH(cubie_coords), 3 + ARRAY_LENGTH(cubie_coords),
    1 + ARRAY_LENGTH(cubie_coords), 2 + ARRAY_LENGTH(cubie_coords), 3 + ARRAY_LENGTH(cubie_coords),
    // clockwise (back down right)
    0 + ARRAY_LENGTH(cubie_coords), 3 + ARRAY_LENGTH(cubie_coords), 1 + ARRAY_LENGTH(cubie_coords), 
    1 + ARRAY_LENGTH(cubie_coords), 3 + ARRAY_LENGTH(cubie_coords), 2 + ARRAY_LENGTH(cubie_coords),
};

Cubie cubie(Vec3 origin, float side_length, float face_length, float face_offset_from_cube, uint8_t color_mask)
{
    Cubie c;
    uint8_t i, face_count;
    uint64_t vc, ic, face_indices_offset;
    Vec3 face_origin, scaled_face_coords[ARRAY_LENGTH(face_coords)];
    float border_width;

    face_count = 0;
    for (i = 0; i < COLOR_MASK_COUNT; i++) {
        if (color_mask & (1 << i)) face_count++;
    }

    // allocate memory for vertices, division by 3 because face_coords[] packs the 3 different planes a face can be on
    c.vertex_count = ARRAY_LENGTH(cubie_coords) + face_count * ARRAY_LENGTH(face_coords) / 3;
    c.verts = (Vertex *) malloc(c.vertex_count * sizeof (Vertex));

    // allocate memory for indices, division by 2 because face_indices[] packs counter- and clockwise rotated faces
    c.index_count = ARRAY_LENGTH(cubie_indices) + face_count * ARRAY_LENGTH(face_indices) / 2;
    c.indices = (uint32_t *) malloc(c.index_count * sizeof (uint32_t));

    // scale face_coords[] to face_length
    for (i = 0; i < ARRAY_LENGTH(face_coords); i++) {
        scaled_face_coords[i] = vec3_scale(face_coords[i], face_length);
    }

    // cubie_coords are the same for every cubie
    for (vc = 0; vc < ARRAY_LENGTH(cubie_coords); vc++) {
        // scale and offset the vertices
        c.verts[vc] = vertex(
            vec3_add(origin, vec3_scale(cubie_coords[vc], side_length)),
            color_from_hex(0x000000FF)
        );
    }

    // cubie_indices are the same for every cubie
    // TODO: maybe use memcpy() ?
    for (ic = 0; ic < ARRAY_LENGTH(cubie_indices); ic++) {
        c.indices[ic] = cubie_indices[ic];
    }

    // dont allow negative border_width
    border_width = fabsf(side_length - face_length) / 2.0f;

    // every additional face adds an offset of 4 (1 face = 4 vertices)
    face_indices_offset = 0;

    // generate the coordinates of the faces
    if (color_mask & COLOR_MASK_FRONT) {
        face_origin = vec3(
            origin.x + border_width,
            origin.y - border_width,
            origin.z + face_offset_from_cube
        );

        // face is in xy-plane so use first 4 coordinates in scaled_face_coords[]
        for (i = 0; i < ARRAY_LENGTH(face_coords) / 3; i++) {
            c.verts[vc++] = vertex(
                vec3_add(face_origin, scaled_face_coords[i]),
                color_from_hex(0xB90000FF)
            );
        }

        // offset indices by faces that where already added
        for (i = 0; i < ARRAY_LENGTH(face_indices) / 2; i++) {
            c.indices[ic++] = face_indices[i] + face_indices_offset;
        }

        face_indices_offset += ARRAY_LENGTH(face_coords) / 3;

        log_debug("Generated front face");
    }

    if (color_mask & COLOR_MASK_UP) {
        face_origin = vec3(
            origin.x + border_width,
            origin.y + face_offset_from_cube,
            origin.z - border_width
        );

        // face is in xz-plane so use second 4 coordinates in scaled_face_coords[]
        for (i = 0; i < ARRAY_LENGTH(face_coords) / 3; i++) {
            c.verts[vc++] = vertex(
                vec3_add(face_origin, scaled_face_coords[i + ARRAY_LENGTH(face_coords) / 3]),
                color_from_hex(0xFFD500FF)
            );
        }

        // offset indices by faces that where already added
        for (i = 0; i < ARRAY_LENGTH(face_indices) / 2; i++) {
            c.indices[ic++] = face_indices[i] + face_indices_offset;
        }

        face_indices_offset += ARRAY_LENGTH(face_coords) / 3;

        log_debug("Generated up face");
    }

    if (color_mask & COLOR_MASK_LEFT) {
        face_origin = vec3(
            origin.x - face_offset_from_cube,
            origin.y - border_width,
            origin.z - border_width
        );

        // face is in yz-plane so use third 4 coordinates in scaled_face_coords[]
        for (i = 0; i < ARRAY_LENGTH(face_coords) / 3; i++) {
            c.verts[vc++] = vertex(
                vec3_add(face_origin, scaled_face_coords[i + 2 * ARRAY_LENGTH(face_coords) / 3]),
                color_from_hex(0x0045ADFF)
            );
        }

        // offset indices by faces that where already added
        for (i = 0; i < ARRAY_LENGTH(face_indices) / 2; i++) {
            c.indices[ic++] = face_indices[i] + face_indices_offset;
        }

        face_indices_offset += ARRAY_LENGTH(face_coords) / 3;
        
        log_debug("Generated left face");
    }

    if (color_mask & COLOR_MASK_BACK) {
        face_origin = vec3(
            origin.x + border_width,
            origin.y - border_width,
            origin.z - face_offset_from_cube - side_length
        );

        // face is in xy-plane so use first 4 coordinates in scaled_face_coords[]
        for (i = 0; i < ARRAY_LENGTH(face_coords) / 3; i++) {
            c.verts[vc++] = vertex(
                vec3_add(face_origin, scaled_face_coords[i]),
                color_from_hex(0xFF5900FF)
            );
        }

        // offset indices by faces that where already added
        for (i = 0; i < ARRAY_LENGTH(face_indices) / 2; i++) {
            c.indices[ic++] = face_indices[i + ARRAY_LENGTH(face_indices) / 2] + face_indices_offset;
        }

        face_indices_offset += ARRAY_LENGTH(face_coords) / 3;
        
        log_debug("Generated back face");
    }

    if (color_mask & COLOR_MASK_DOWN) {
        face_origin = vec3(
            origin.x + border_width,
            origin.y - face_offset_from_cube - side_length,
            origin.z - border_width
        );

        // face is in xz-plane so use second 4 coordinates in scaled_face_coords[]
        for (i = 0; i < ARRAY_LENGTH(face_coords) / 3; i++) {
            c.verts[vc++] = vertex(
                vec3_add(face_origin, scaled_face_coords[i + ARRAY_LENGTH(face_coords) / 3]),
                color_from_hex(0xFFFFFFFF)
            );
        }

        // offset indices by faces that where already added
        for (i = 0; i < ARRAY_LENGTH(face_indices) / 2; i++) {
            c.indices[ic++] = face_indices[i + ARRAY_LENGTH(face_indices) / 2] + face_indices_offset;
        }

        face_indices_offset += ARRAY_LENGTH(face_coords) / 3;
        
        log_debug("Generated down face");
    }

    if (color_mask & COLOR_MASK_RIGHT) {
        face_origin = vec3(
            origin.x + face_offset_from_cube + side_length,
            origin.y - border_width,
            origin.z - border_width
        );

        // face is in yz-plane so use third 4 coordinates in scaled_face_coords[]
        for (i = 0; i < ARRAY_LENGTH(face_coords) / 3; i++) {
            c.verts[vc++] = vertex(
                vec3_add(face_origin, scaled_face_coords[i + 2 * ARRAY_LENGTH(face_coords) / 3]),
                color_from_hex(0x009B48FF)
            );
        }

        // offset indices by faces that where already added
        for (i = 0; i < ARRAY_LENGTH(face_indices) / 2; i++) {
            c.indices[ic++] = face_indices[i + ARRAY_LENGTH(face_indices) / 2] + face_indices_offset;
        }

        face_indices_offset += ARRAY_LENGTH(face_coords) / 3;
        
        log_debug("Generated right face");
    }

    log_debug("Generated %" PRIu64 "/%" PRIu64 " vertices", vc, c.vertex_count);
    log_debug("Generated %" PRIu64 "/%" PRIu64 " indices", ic, c.index_count);


    // Generate vertex array, vertex buffer, and element buffer
    glGenVertexArrays(1, &c.vao);
    glGenBuffers(1, &c.vbo);
    glGenBuffers(1, &c.ebo);

    glBindVertexArray(c.vao);

    glBindBuffer(GL_ARRAY_BUFFER, c.vbo);
    glBufferData(GL_ARRAY_BUFFER, c.vertex_count * sizeof (Vertex), c.verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, c.index_count * sizeof (uint32_t), c.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(VERTEX_POS, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void *) offsetof(Vertex, pos));
    glEnableVertexAttribArray(VERTEX_POS);

    glVertexAttribPointer(VERTEX_POS, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void *) offsetof(Vertex, col));
    glEnableVertexAttribArray(VERTEX_COL);

    return c;
}