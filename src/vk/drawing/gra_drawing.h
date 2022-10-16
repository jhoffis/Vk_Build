#pragma once

#include "src/window.h"

namespace Gra {

    extern uint32_t currentFrame;

    void createSyncObjects();
    void cleanupSyncObjects();
    void drawFrame();

}