#include "window.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "logging.h"

typedef struct {
    GLFWwindow *win;
    Key_Callback kcallback;
    Framebuffer_Size_Callback fbcallback;

    int x, y, width, height;
    GLFWmonitor *mon;
    GLFWvidmode *mode;
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
void window_size_callback(GLFWwindow *win, int width, int height)
{
    (void) win;

    // Resizing from fullscreen to windowed or vice versa displays the same value 2 times since glfwGetPrimaryMonitor already returns NULL
    // a fix would be to check if window_get_width() and width are the same, etc.
    log_debug("window size changed, width: %d -> %d, height: %d -> %d", window_get_width(), width, window_get_height(), height);

    if (window_is_fullscreen()) return;

    window.width  = width;
    window.height = height;
}

void window_init(int width, int height, const char *title, int fullscreen)
{
    int version;
    GLint major_version, minor_version;

    if (glfwInit() == GLFW_FALSE) {
        log_error_and_exit(1, "Failed to initialize GLFW");
    }

    log_info("Initialized GLFW");

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

    window.mon  = glfwGetPrimaryMonitor();
    window.mode = (GLFWvidmode *)glfwGetVideoMode(window.mon);

    glfwGetWindowSize(window.win, &window.width, &window.height);
    glfwGetWindowPos(window.win, &window.x, &window.y);

    if (fullscreen) {
        log_info("Created window, width = %d height = %d fullscreen = true", window.mode->width, window.mode->height);
    } else {
        log_info("Created window, width = %d height = %d fullscreen = false", window.width, window.height);
    }

    window_set_fullscreen(fullscreen);

    glfwMakeContextCurrent(window.win);
    // TODO: Maybe let user set framebuffer-size callback?
    glfwSetFramebufferSizeCallback(window.win, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window.win, window_size_callback);

    log_info("Loading OpenGL functions...");

    version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    if (version == GL_FALSE) {
        log_error_and_exit(1, "Failed to load OpenGL functions");
    }

    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);

    log_info("Finished loading OpenGL functions, using version %d.%d", major_version, minor_version);

    // 3D Options
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void window_set_key_callback(Key_Callback callback)
{
    window.kcallback = callback;
    glfwSetKeyCallback(window.win, key_callback_wrapper);

    log_info("Registered key callback for window");
}

int window_is_fullscreen(void)
{
    return (glfwGetWindowMonitor(window.win) != NULL);
}

void window_set_fullscreen(int val)
{
    if (val == window_is_fullscreen()) return;

    if (val) {
        // backup size and position
        glfwGetWindowSize(window.win, &window.width, &window.height);
        glfwGetWindowPos(window.win, &window.x, &window.y);

        window.mode = (GLFWvidmode *) glfwGetVideoMode(window.mon);

        glfwSetWindowMonitor(window.win, window.mon, 0, 0, window.mode->width, window.mode->height, GLFW_DONT_CARE);

        log_debug("Set window to fullscreen");
    } else {
        glfwSetWindowMonitor(window.win, NULL, window.x, window.y, window.width, window.height, GLFW_DONT_CARE);

        log_debug("Set window to windowed mode");
    }
}

void window_toggle_fullscreen(void)
{
    window_set_fullscreen(
        !window_is_fullscreen()
    );
}

int window_get_width(void)
{
    if (window_is_fullscreen()) {
        return window.mode->width;
    }

    return window.width;
}

int window_get_height(void)
{
    if (window_is_fullscreen()) {
        return window.mode->height;
    }

    return window.height;
}

float window_get_aspect_ratio(void)
{
    return (float)window_get_width() / (float)window_get_height();
}

void window_set_should_close(int val)
{
    // TODO: maybe check if 0 is GLFW_FALSE ?
    glfwSetWindowShouldClose(window.win, val);

    log_debug("Set window-should-close option to %s", val ? "true" : "false");
}

int window_should_close(void)
{
    // Poll events and swap buffers in background
    glfwPollEvents();
    glfwSwapBuffers(window.win);

    return glfwWindowShouldClose(window.win);
}

void window_clear(Color c)
{
    // TODO: maybe implemnt set_clear_color() ?
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float window_get_frame_time(void)
{
    float now, dt;
    static float last_time = 0.0f;

    // First call to this is not the actual frame time since last_time starts out at 0
    now       = glfwGetTime();
    dt        = now - last_time;
    last_time = now;

    return dt;
}

void window_close(void)
{
    glfwDestroyWindow(window.win);
    glfwTerminate();

    log_info("Closing window...");
    log_info("Terminating GLFW...");
}
