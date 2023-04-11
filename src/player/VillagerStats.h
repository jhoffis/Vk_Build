//
// Created by jonhof on 4/11/2023.
//

#ifndef VULKAN_VILLAGERSTATS_H
#define VULKAN_VILLAGERSTATS_H

#include <cstdint>
#include <bitset>

const uint64_t
    gender = 0b0001,
    attractedFemale = 0b0010,
    attractedMale = 0b0100

    // good:
//    humble      = 0b1000,
    // evil:
//    sadistic    = 0b0001'000,
    // sexual preference:
//    chest       = 0b0001'000,
//    face        = 0b0010'000,
//    organ       = 0b0100'000,
//    butt        = 0b1000'000,
//    feet        = 0b0001'0000'000

    // at least five of each of these to represent a spectrum that takes up less space than a 16-bit float
    // open:
    // conscientious:
    // extraverted:
    // disagreeable:
    // agreeable:
    // neurotic:

    ;

struct VillagerStats {
    // thought, want, personality, feelings. allergi, gutbiome, iq
    // save events or things in memory when you're happy or sad enough.
    uint64_t personality{0};

};


#endif //VULKAN_VILLAGERSTATS_H
