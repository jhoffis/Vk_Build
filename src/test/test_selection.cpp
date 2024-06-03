#include <iostream>
#include "test_selection.h"
#include "rendering/Entity.h"

const std::vector<std::function<void()>> m_tests{
    []() {
        std::cout << "Entity within bounds" << std::endl;
        Entity e{
            .size = {1, 1}
        };
        assert(e.isWithin(.0, .0, .0, .0));
        assert(e.isWithin(.1, .1, .1, .1));
        assert(e.isWithin(-.1, .1, .1, .1));
        assert(!e.isWithin(-.1, -.1, -.1, -.1));
    }
};

void Test::run() {
    for (const auto& test : m_tests) {
        test();
    }
    std::cout << "Ended tests successfully" << std::endl;
}
