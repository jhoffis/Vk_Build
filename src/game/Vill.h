#pragma once

#include <cstdint>
#include "gra/Entity.h"

namespace Villager {

    struct Deeds {
        uint8_t badOnes;
//        treesChopped
//        hadSex
//        arrowsShot
//        swordSlashed
    };

    struct HealthPoints {
        uint8_t head{};
        uint8_t leftArm{};
        uint8_t rightArm{};
        uint8_t torso{};
        uint8_t leftFoot{};
        uint8_t rightFoot{};
    };

    struct Personality {
        uint8_t openness{};
        uint8_t conscientiousness{};
        uint8_t extraversion{};
        uint8_t agreeableness{};
        uint8_t neuroticism{};
        uint8_t words{};
        uint8_t craft{};
//        greed
//        lust etc
//        dark tetrad
//        bad deeds
    };

    struct Body {
        uint8_t lifting{};
        uint8_t speed{};
        uint8_t melatonin{};
        uint16_t weight{};
        uint16_t height{};

        // Face:
        uint8_t skinFairness{};
        uint8_t sight{};
        uint8_t smell{};
        uint8_t hearing{};
        Vec3 eyeColor{};
        Vec3 hairColor{};
    };

    /*
     * Armor shall be based on the clothes the vill wears.
     */
    struct Vill {
        // Genetics:
        Personality personality{};
        Body body{};

        HealthPoints hp{};
        uint8_t age{};
        uint8_t interestInSex{};
        uint8_t blueness{};
        uint8_t gender{}; // includes male female bi and gay

        Entity entity{};
        char *name{};

    };
}