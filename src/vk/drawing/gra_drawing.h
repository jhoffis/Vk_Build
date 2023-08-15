#pragma once

#include "src/window.h"

namespace Drawing {

    extern uint32_t currSwapFrame;
    extern bool clear;

    void createSyncObjects();
    void cleanupSyncObjects();
    void drawFrame();

}