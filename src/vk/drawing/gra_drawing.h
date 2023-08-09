#pragma once

#include "src/window.h"

namespace Drawing {

    extern uint32_t currSwapFrame;

    void createSyncObjects();
    void cleanupSyncObjects();
    void drawFrame();

}