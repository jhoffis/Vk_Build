#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <string>
#include <vulkan/vulkan.h>

namespace Window {
    enum CursorType {
        cursorNormal,
        cursorCanPoint,
        cursorIsPoint,
        cursorCanHold,
        cursorIsHold
    };
    extern GLFWwindow* m_window;
    extern bool m_framebufferResized;

    inline static int WIDTH, HEIGHT;
    void createWindow(bool fullscreen, bool vsync);
    void destroyWindow();
    void setCursor(CursorType cursor);
    void setFullscreen(bool fullscreen);
    void switchFullscreen();
    void mouseStateHide(bool lock);
    void mouseStateToPrevious();
    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
}

#endif //VULKAN_WINDOW_H