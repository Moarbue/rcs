#include "window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "logging.h"

typedef struct {
    GLFWwindow *win;
    Key_Callback kcallback;
    Framebuffer_Size_Callback fbcallback;

    int x, y, width, height;
    GLFWmonitor *mon;
} Window;

Window window;

void key_callback_wrapper(GLFWwindow *win, int key, int scancode, int action, int mods)
{
    (void) win; (void) scancode;
    (*window.kcallback)(key, action, mods);
}
void framebuffer_size_callback(GLFWwindow *win, int width, int height)
{
    (void) win;
    glViewport(0, 0, (GLint)width, (GLint)height);
    // make resizing smooth
    glfwPollEvents();
    glfwSwapBuffers(window.win);
}

void init_window(int width, int height, const char *title, int fullscreen)
{
    int version;
    GLint major_version, minor_version;

    if (glfwInit() == GLFW_FALSE) {
        log_error_and_exit(1, "Failed to initialize GLFW");
    }

    log_info("Initialized GLFW successfully");

    // Minimum supported version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // but higher version is okay
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);


    window.win = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window.win == NULL) {
        log_error_and_exit(1, "Failed to create window, width = %d height = %d fullscreen = %s", width, height, fullscreen ? "true" : "false");
    }

    log_info("Created window successfully, width = %d height = %d fullscreen = %s", width, height, fullscreen ? "true" : "false");

    window.mon = glfwGetPrimaryMonitor();
    glfwGetWindowSize(window.win, &window.width, &window.height);
    glfwGetWindowPos(window.win, &window.x, &window.y);

    if (fullscreen) {
        set_fullscreen(1);
    }

    glfwMakeContextCurrent(window.win);
    // TODO: Maybe let user set framebuffer-size callback?
    glfwSetFramebufferSizeCallback(window.win, framebuffer_size_callback);

    version = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (version == GL_FALSE) {
        log_error_and_exit(1, "Failed to load OpenGL functions");
    }

    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);

    log_info("Successfuly loaded OpenGL functions, using version %d.%d", major_version, minor_version);

    // 3D Options
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void set_key_callback(Key_Callback callback)
{
    window.kcallback = callback;
    glfwSetKeyCallback(window.win, key_callback_wrapper);

    log_info("Set key callback successfully");
}

int is_fullscreen(void)
{
    return (glfwGetWindowMonitor(window.win) != NULL);
}

void set_fullscreen(int val)
{
    GLFWvidmode *mode;

    if (val == is_fullscreen()) return;

    if (val) {
        // backup size and position
        glfwGetWindowSize(window.win, &window.width, &window.height);
        glfwGetWindowPos(window.win, &window.x, &window.y);

        mode = (GLFWvidmode *) glfwGetVideoMode(window.mon);

        glfwSetWindowMonitor(window.win, window.mon, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
    } else {
        glfwSetWindowMonitor(window.win, NULL, window.x, window.y, window.width, window.height, GLFW_DONT_CARE);
    }
}

void toggle_fullscreen(void)
{
    set_fullscreen(
        !is_fullscreen()
    );
}

void set_window_should_close(int val)
{
    // TODO: maybe check if 0 is GLFW_FALSE ?
    glfwSetWindowShouldClose(window.win, val);

    log_info("Set window-should-close option to %s", val ? "true" : "false");
}

int window_should_close(void)
{
    // Poll events and swap buffers in background
    glfwPollEvents();
    glfwSwapBuffers(window.win);

    return glfwWindowShouldClose(window.win);
}

void clear_window(Color c)
{
    // TODO: maybe implemnt set_clear_color() ?
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float get_frame_time(void)
{
    float now, dt;
    static float last_time = 0.0f;

    // First call to this is not the actual frame time since last_time starts out at 0
    now       = glfwGetTime();
    dt        = now - last_time;
    last_time = now;

    return dt;
}

void close_window(void)
{
    glfwDestroyWindow(window.win);
    glfwTerminate();

    log_info("Closed window successfully");
    log_info("Terminated GLFW successfully");
}
