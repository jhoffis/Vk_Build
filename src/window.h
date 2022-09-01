#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
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
