#include "shader.h"

#include "logging.h"
#include "util.h"

#include <malloc.h>
#include <string.h>

GLint compile_shader(GLint type, const char *shader_path, GLuint *shader);

Shader_Program *shader_new(const char *vertex_path, const char *fragment_path)
{
    Shader_Program *prog;
    GLuint vertex_shader, fragment_shader;
    GLint success;
    char info_log[512];

    log_info("Creating shader program...");

    prog = (Shader_Program *) malloc(sizeof (Shader_Program));
    if (prog == NULL) {
        log_error("Failed to allocate memory for shader program");
        return NULL;
    }

    log_info("Compiling vertex shader...");
    success = compile_shader(GL_VERTEX_SHADER, vertex_path, &vertex_shader);
    if (success == 0) {
        free(prog);
        return NULL;
    }
    log_info("Finished compiling vertex shader");

    log_info("Compiling fragment shader...");
    success = compile_shader(GL_FRAGMENT_SHADER, fragment_path, &fragment_shader);
    if (success == 0) {
        free(prog);
        return NULL;
    }
    log_info("Finished compiling fragment shader");

    log_info("Linking shaders with shader program...");
    prog->id = glCreateProgram();
    glAttachShader(prog->id, vertex_shader);
    glAttachShader(prog->id, fragment_shader);
    glLinkProgram(prog->id);

    glGetProgramiv(prog->id, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(prog->id, 512, NULL, info_log);
        free(prog);
        log_error("Failed to link program with shaders: %s", info_log);
        return NULL;
    }
    log_info("Finished linking shaders to shader program");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    prog->uniform_count = 0;

    log_info("Finished creating shader program");

    return prog;
}

void shader_register_uniform(Shader_Program *prog, const char *name)
{
    size_t i;

    log_info("Registering uniform \'%s\'...", name);

    i = prog->uniform_count;
    if (i + 1 >= SHADER_MAX_UNIFORMS) {
        log_warning("Cannot register uniform \'%s\'. Shader class only supports up to %d uniforms", name, SHADER_MAX_UNIFORMS);
        return;
    }

    strncpy(prog->uniform_strings[i], name, SHADER_MAX_UNIFORM_NAME_LENGTH);

    prog->uniform_locations[i] = glGetUniformLocation(prog->id, name);
    prog->uniform_count++;

    log_info("Finished registering uniform");
}

void shader_set_uniform_mat4(Shader_Program *prog, const char *name, float *val, GLboolean transpose)
{
    size_t i;

    for (i = 0; i < prog->uniform_count; i++) {
        if (strcmp(name, prog->uniform_strings[i]) == 0) break;
    }

    if (i >= prog->uniform_count) {
        log_error("Unknown Uniform %s", name);
    }

    glUseProgram(prog->id);
    glUniformMatrix4fv(prog->uniform_locations[i], 1, transpose, val);
}

void shader_set_uniform_color(Shader_Program *prog, const char *name, Color col)
{
    size_t i;

    for (i = 0; i < prog->uniform_count; i++) {
        if (strcmp(name, prog->uniform_strings[i]) == 0) break;
    }

    if (i >= prog->uniform_count) {
        log_error("Unknown Uniform %s", name);
    }

    glUseProgram(prog->id);
    glUniform4f(prog->uniform_locations[i], col.r, col.g, col.b, col.a);
}

void shader_set_uniform_sampler2D(Shader_Program *prog, const char *name, unsigned int textureID)
{
    size_t i;

    for (i = 0; i < prog->uniform_count; i++) {
        if (strcmp(name, prog->uniform_strings[i]) == 0) break;
    }

    if (i >= prog->uniform_count) {
        log_error("Unknown Uniform %s", name);
    }

    glUseProgram(prog->id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(prog->uniform_locations[i], 0);   
}

void shader_bind(Shader_Program *prog)
{
    glUseProgram(prog->id);
}

void shader_unbind(Shader_Program *prog)
{
    (void) prog;
    glUseProgram(0);
}


GLint compile_shader(GLint type, const char *shader_path, GLuint *shader)
{
    char *shader_code;
    GLuint sh;
    GLint success;
    char info_log[512];

    shader_code = read_file(shader_path);
    if (shader_code == NULL) {
        log_error("Failed to load shader from file");
        return 0;
    }

    sh = glCreateShader(type);
    glShaderSource(sh, 1, (const GLchar *const *) &shader_code, NULL);
    glCompileShader(sh);

    free(shader_code);

    glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(sh, sizeof (info_log), NULL, info_log);
        log_error("Failed to compile shader from file \'%s\': %s", shader_path, info_log);
        return 0;
    }

    *shader = sh;
    return 1;
}

void shader_free(Shader_Program *prog)
{
    if (prog == NULL) return;

    glDeleteProgram(prog->id);
    free(prog);
}