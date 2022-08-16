#include "window.h"

#include <stdexcept>
#include <algorithm>
#include <stb_image.h>
#include <filesystem>


GLFWcursor* glfwCursorNormal, * glfwCursorCanPoint, * glfwCursorIsPoint, * glfwCursorCanHold, * glfwCursorIsHold;
bool m_focused;
int m_fullscreen = -1;
GLFWwindow* m_window;
GLFWmonitor* m_monitor;
bool m_previousMouseStateVisible;
Window::CursorType m_cursorTypeSelected;


GLFWwindow* Window::getWindow() {
    return m_window;
}

void glfwErrors(int error_code, const char *description) {
    throw std::runtime_error(std::string("GLFW ERROR: ").append(reinterpret_cast<const char *>(error_code)).append("\n").append(description));
}

void updateWithinWindow(int currWidth) {
    int client_width = currWidth * .3f;

    const int incVal = 64;
    int foundWidth;
    int i = 1;
    do {
        foundWidth = incVal * i;
        if (client_width <= foundWidth)
            break;
        i++;
    } while (true);

    client_width = foundWidth;

    int client_height = client_width * 9 / 16;

    Window::WIDTH = client_width;
    Window::HEIGHT = client_height;

    //    TODO if (sceneHandler != null)
    //        sceneHandler.updateResolution();
}

GLFWmonitor* getCurrentMonitor(GLFWwindow *window, GLFWmonitor *monitor) {
    int wx, wy, ww, wh;
    int mx, my, mw, mh;
    int overlap, bestoverlap = 0;
    GLFWmonitor *monitorTemp, *bestmonitor = monitor;
    const GLFWvidmode *mode;

    glfwGetWindowPos(window, &wx, &wy);
    glfwGetWindowSize(window, &ww, &wh);
    int monitorCount = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    for (int i = 0; i < monitorCount; i++) {
        monitorTemp = monitors[i];
        mode = glfwGetVideoMode(monitorTemp);
        glfwGetMonitorPos(monitorTemp, &mx, &my);
        mw = mode->width;
        mh = mode->height;

        overlap = std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx))
                  * std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

        if (bestoverlap < overlap) {
            bestoverlap = overlap;
            bestmonitor = monitorTemp;
        }
    }
    return bestmonitor;
}


void Window::mouseStateHide(bool lock) {
    m_previousMouseStateVisible = glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    glfwSetInputMode(m_window, GLFW_CURSOR,
                     lock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::mouseStateToPrevious() {
    mouseStateHide(m_previousMouseStateVisible);
}

void Window::setCursor(CursorType cursor) {
    m_cursorTypeSelected = cursor;
    GLFWcursor *glfwCursor;
    switch (cursor) {
        case CursorType::cursorCanHold :
            glfwCursor = glfwCursorCanHold;
            break;
        case CursorType::cursorCanPoint :
            glfwCursor = glfwCursorCanPoint;
            break;
        case CursorType::cursorIsHold :
            glfwCursor = glfwCursorIsHold;
            break;
        case CursorType::cursorIsPoint :
            glfwCursor = glfwCursorIsPoint;
            break;
        case CursorType::cursorNormal :
            glfwCursor = glfwCursorNormal;
            break;
    }
    glfwSetCursor(m_window, glfwCursor);
}

GLFWimage createGLFWImage(const char *path) {
    int w;
    int h;
    int comp;
    std::string currPath = std::filesystem::current_path().string().append("/res/");
    const char *realPath = reinterpret_cast<const char *>(currPath.append(path).c_str());
    unsigned char *image = stbi_load(realPath, &w, &h, &comp, STBI_rgb_alpha);
    // TODO free stb images

    if (image == nullptr)
        throw std::runtime_error(std::string("Failed to load texture at ").append(realPath));

    GLFWimage resultImg;
    resultImg.width = w;
    resultImg.height = h;
    resultImg.pixels = image;

    return resultImg;
}

GLFWcursor* createCursor(const char* path, float xPercent) {
    GLFWimage cursor = createGLFWImage(path);
    return glfwCreateCursor(&cursor, (int)(cursor.width * xPercent), 0);
}

void Window::setFullscreen(bool fullscreenTemp) {

    GLFWmonitor *monitorTemp = getCurrentMonitor(m_window, m_monitor);
    const GLFWvidmode *vidmode = glfwGetVideoMode(monitorTemp);

    if (m_monitor == monitorTemp && m_fullscreen == (fullscreenTemp ? 1 : 0))
        return;

    m_monitor = monitorTemp;
    m_fullscreen = fullscreenTemp ? 1 : 0;

    if (fullscreenTemp) {
        // switch to fullscreen

        // set width based on the right monitor
        WIDTH = vidmode->width;
        HEIGHT = vidmode->height;
    } else {
        // switch to windowed
        updateWithinWindow(vidmode->width);
        monitorTemp = nullptr;
    }

    int wx, wy;
    glfwGetWindowPos(m_window, &wx, &wy);

    glfwSetWindowMonitor(m_window, monitorTemp, wx, wy,
                         WIDTH, HEIGHT, monitorTemp == nullptr ? GLFW_DONT_CARE : vidmode->refreshRate);

    // if windowed
    if (monitorTemp == nullptr && wx == 0 && wy == 0) {
        glfwSetWindowPos(m_window, (vidmode->width - WIDTH) / 2,
                         (vidmode->height - HEIGHT) / 2);
    }

    // move drawing of graphics to the right place
}

void Window::switchFullscreen() {
    setFullscreen(m_fullscreen == 0);
}

void Window::createWindow(bool fullscreen, bool vsync) {

    // Set client size to one resolution lower than the current one

    if (!glfwInit()) {
        glfwTerminate();
        throw std::runtime_error("Unable to initialize glfw");
    }

    m_monitor = glfwGetPrimaryMonitor();
    auto mode = glfwGetVideoMode(m_monitor);
    updateWithinWindow(mode->width);

    glfwSetErrorCallback(glfwErrors);

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, true);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    
    // vk
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


    m_monitor = glfwGetPrimaryMonitor();

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "vk", nullptr, nullptr);
    if (m_window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create the glfw window");
    }

    glfwSetWindowFocusCallback(m_window, [](auto window, auto f)
        {
            m_focused = f;
        });

    // ICON
    const GLFWimage icon = createGLFWImage("pics/icon.png");
    glfwSetWindowIcon(m_window, 1, &icon);

    // Cursor
    glfwCursorNormal = createCursor("pics/cursor.png", 0);
    float xPercentCursorHand = 0.27f;
    glfwCursorCanPoint = createCursor("pics/cursorCanPoint.png", xPercentCursorHand);
    glfwCursorIsPoint = createCursor("pics/cursorIsPoint.png", xPercentCursorHand);
    glfwCursorCanHold = createCursor("pics/cursorCanHold.png", xPercentCursorHand);
    glfwCursorIsHold = createCursor("pics/cursorIsHold.png", xPercentCursorHand);
    setCursor(CursorType::cursorNormal);
    mouseStateHide(false);

        // center
    setFullscreen(fullscreen);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface");
    }
}


void Window::destroyWindow() {
    glfwDestroyWindow(m_window);
    glfwDestroyCursor(glfwCursorNormal);
    glfwDestroyCursor(glfwCursorCanPoint);
    glfwDestroyCursor(glfwCursorIsPoint);
    glfwDestroyCursor(glfwCursorCanHold);
    glfwDestroyCursor(glfwCursorIsHold);
//    delete window;
//    delete monitor;
    glfwTerminate();
}
