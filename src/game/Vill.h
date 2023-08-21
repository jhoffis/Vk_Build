#pragma once

#include <cstdint>
#include "gra/Entity.h"

namespace Villager {

    struct Personality {
        uint8_t openness{};
        uint8_t conscientiousness{};
        uint8_t extraversion{};
        uint8_t agreeableness{};
        uint8_t neuroticism{};
        uint8_t words{};
        uint8_t craft{};
    };

    struct Body {
        uint8_t lifting{};
        uint8_t speed{};
        uint8_t sight{};
        uint8_t smell{};
        uint8_t melatonin{};
        uint16_t weight{};
        uint16_t height{};
        Vec3 eyeColor{};
        Vec3 hairColor{};
    };

    /*
     * Armor shall be based on the clothes the vill wears.
     */
    struct Vill {
        Entity *entity{};
        uint16_t hp{};
        uint8_t age{};
        bool gender{};

        // Genetics:
        Personality personality{};
        Body body{};
    };
}