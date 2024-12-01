#ifndef _SHADER_H_
#define _SHADER_H_

#include "glad/gl.h"

#ifndef SHADER_MAX_UNIFORMS
#   define SHADER_MAX_UNIFORMS 10
#endif 

#ifndef SHADER_MAX_UNIFORM_NAME_LENGTH
#   define SHADER_MAX_UNIFORM_NAME_LENGTH 128
#endif

typedef struct {
    GLuint id;
    GLuint uniform_locations[SHADER_MAX_UNIFORMS];
    char uniform_strings[SHADER_MAX_UNIFORMS][SHADER_MAX_UNIFORM_NAME_LENGTH];
    size_t uniform_count;
} Shader_Program;

Shader_Program shader_new(const char *vertex_path, const char *fragment_path);
void shader_register_uniform(Shader_Program *prog, const char *name);
void shader_set_uniform_mat4(Shader_Program *prog, const char *name, float *val, GLboolean transpose);
void shader_bind(Shader_Program *prog);
void shader_unbind(Shader_Program *prog);

#endif // _SHADER_H_