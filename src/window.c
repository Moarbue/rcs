#include "window.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "logging.h"

typedef struct {
    GLFWwindow *win;
    Key_Callback kcallback;
    Window_Size_Callback wsbcallback;
    Render_Function render_func;

    int x, y, width, height;
    GLFWmonitor *mon;
    GLFWvidmode *mode;

    Color clear_color;
} Window;

Window window;

void key_callback_wrapper(GLFWwindow *win, int key, int scancode, int action, int mods)
{
    (void) win; (void) scancode;
    (*window.kcallback)(key, action, mods);
}
void window_size_callback_wrapper(GLFWwindow *win, int width, int height)
{
    static int was_fullscreen = 0;

    (void) win;

    // don't update window.width and height if it was changed to fullscreen
    if (window_is_fullscreen()) {
        log_debug("window size changed, width: %d -> %d, height: %d -> %d", window.width, width, window.height, height);
        was_fullscreen = 1;
        return;
    } else if (was_fullscreen) {
        // window changed from fullscreen to windowed, so window_get_width()/height() will already return the new width/height
        log_debug("window size changed, width: %d -> %d, height: %d -> %d", window.mode->width, width, window.mode->height, height);
        was_fullscreen = 0;
    } else {
        log_debug("window size changed, width: %d -> %d, height: %d -> %d", window_get_width(), width, window_get_height(), height);
    }

    window.width  = width;
    window.height = height;

    (*window.wsbcallback)(width, height);

    // make resizing smooth by rendering a frame
    (*window.render_func)();
    glfwPollEvents();
    glfwSwapBuffers(window.win);
}
void framebuffer_size_callback(GLFWwindow *win, int width, int height)
{
    (void) win;
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void key_callback_default(int key, int action, int mods) {(void) key; (void) action; (void) mods;}
void window_size_callback_default(int width, int height) {(void) width; (void) height;}

void render_func_default(void) {}

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

    window.kcallback   = key_callback_default;
    window.wsbcallback = window_size_callback_default;

    window.render_func = render_func_default;

    glfwGetWindowSize(window.win, &window.width, &window.height);
    glfwGetWindowPos(window.win, &window.x, &window.y);

    if (fullscreen) {
        log_info("Created window, width = %d height = %d fullscreen = true", window.mode->width, window.mode->height);
    } else {
        log_info("Created window, width = %d height = %d fullscreen = false", window.width, window.height);
    }

    glfwMakeContextCurrent(window.win);
    glfwSetFramebufferSizeCallback(window.win, framebuffer_size_callback);
    glfwSetKeyCallback(window.win, key_callback_wrapper);
    glfwSetWindowSizeCallback(window.win, window_size_callback_wrapper);

    log_info("Loading OpenGL functions...");

    version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    if (version == GL_FALSE) {
        log_error_and_exit(1, "Failed to load OpenGL functions");
    }

    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);

    log_info("Finished loading OpenGL functions, using version %d.%d", major_version, minor_version);

    window_set_fullscreen(fullscreen);

    // 3D Options
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void window_set_key_callback(Key_Callback callback)
{
    window.kcallback = callback;

    log_info("Registered key callback for window");
}

void window_set_size_callback(Window_Size_Callback callback)
{
    window.wsbcallback = callback;

    log_info("Registered window size callback for window");
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
    return glfwWindowShouldClose(window.win);
}

void window_set_clear_color(Color c)
{
    window.clear_color = c;
}

void window_clear(void)
{
    glClearColor(window.clear_color.r, window.clear_color.g, window.clear_color.b, window.clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_main_loop(Render_Function render_frame)
{
    window.render_func = render_frame;

    while(!window_should_close()) {
        (*window.render_func)();

        // Poll events and swap buffers in background
        glfwPollEvents();
        glfwSwapBuffers(window.win);
    }
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
