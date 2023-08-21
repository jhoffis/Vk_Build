#pragma once

#include <cstdint>
#include "gra/Entity.h"

namespace Villager {

    struct Personaltiy {
        uint8_t openness;
        uint8_t conscientiousness;
        uint8_t extraversion;
        uint8_t agreeableness;
        uint8_t neuroticism;
        uint8_t words;
        uint8_t craft;
    };

    struct Body {
        uint8_t lifting;
        uint8_t speed;
        uint8_t sight;
        uint8_t smell;
        uint8_t eyecolor;
        uint8_t haircolor;
        uint8_t melatone;
    };

    /*
     * Armor shall be based on the clothes the vill wears.
     */
    struct Vill {
        Entity *entity;
        uint16_t hp{};
        bool gender{}; // TODO well it is more complicated than this isnt it? Like a mother can do functions that a man cant

        // Genetics:
        Personaltiy personaltiy{};
    };

    void impregnate(Vill& mother, Vill& father);

    void createVillagers();
    void destroy();
}