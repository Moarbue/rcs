#include "shader.h"

#include "logging.h"
#include "util.h"

#include <string.h>

GLuint compile_shader(GLint type, const char *shader_path);

Shader_Program shader_new(const char *vertex_path, const char *fragment_path)
{
    Shader_Program prog;
    GLuint vertex_shader, fragment_shader;
    GLint success;
    char info_log[512];

    log_info("Compiling vertex shader from \'%s\'...",   vertex_path);
    vertex_shader   = compile_shader(GL_VERTEX_SHADER,   vertex_path);
    log_info("Compiling fragment shader from \'%s\'...", fragment_path);
    fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_path);

    log_info("Linking shaders with shader program...");
    prog.id = glCreateProgram();
    glAttachShader(prog.id, vertex_shader);
    glAttachShader(prog.id, fragment_shader);
    glLinkProgram(prog.id);

    glGetProgramiv(prog.id, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(prog.id, 512, NULL, info_log);
        log_error_and_exit(1, "Failed to link program with shaders: %s", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    prog.uniform_count = 0;

    return prog;
}

void shader_register_uniform(Shader_Program *prog, const char *name)
{
    size_t i;

    log_info("Registering uniform \'%s\'", name);

    i = prog->uniform_count;
    if (i + 1 >= SHADER_MAX_UNIFORMS) {
        log_warning("Cannot register uniform \'%s\'. Shader class only supports up to %d uniforms", name, SHADER_MAX_UNIFORMS);
        return;
    }

    strncpy(prog->uniform_strings[i], name, SHADER_MAX_UNIFORM_NAME_LENGTH);

    prog->uniform_locations[i] = glGetUniformLocation(prog->id, name);
    prog->uniform_count++;
}

void shader_set_uniform_mat4(Shader_Program *prog, const char *name, float *val, GLboolean transpose)
{
    size_t i;

    for (i = 0; i < prog->uniform_count; i++) {
        if (strcmp(name, prog->uniform_strings[i]) == 0) break;
    }

    if (i >= prog->uniform_count) {
        log_error_and_exit(1, "Unknown Uniform %s", name);
    }

    glUniformMatrix4fv(prog->uniform_locations[i], 1, transpose, val);
}

GLuint compile_shader(GLint type, const char *shader_path)
{
    char *shader_code;
    GLuint shader;
    GLint success;
    char info_log[512];

    shader_code = read_file(shader_path);
    if (shader_code == NULL) {
        log_error_and_exit(1, "Failed to load shader from file");
    }

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar *const *) &shader_code, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(shader, sizeof (info_log), NULL, info_log);
        free(shader_code);
        log_error_and_exit(1, "Failed to compile shader from file \'%s\': %s", shader_path, info_log);
    }
    free(shader_code);

    return shader;
}