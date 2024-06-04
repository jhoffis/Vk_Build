#include <iostream>
#include "test_selection.h"
#include "rendering/Entity.h"
#include "models/Villager.h"
#include "path_finding.h"

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
    },
    []() {


    std::vector<int> Map = {
            1, 0, 1, 0, 1, 1, 1, 0, 1, 1,
            1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
            1, 0, 0, 0, 1, 0, 0, 1, 1, 1,
            1, 0, 1, 1, 1, 1, 1, 0, 1, 0,
            1, 1, 1, 0, 0, 0, 1, 0, 1, 1,
            0, 0, 1, 1, 1, 0, 1, 0, 0, 1,
            1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
            0, 1, 0, 1, 1, 1, 1, 0, 1, 0,
            0, 1, 1, 1, 0, 0, 1, 0, 1, 1,
            1, 1, 0, 1, 1, 1, 1, 1, 0, 1,
    };
    std::vector<int> OutPath;

    auto res0 = PathFinder::findPath(
            {.pos = {0, 0, 0}},
            {2, 3},
            {.xy = 10,
                    .map = Map},
            OutPath
            );
    assert(res0);

    OutPath.clear();
    auto res1 = PathFinder::findPath(
            {.pos = {0, 0, 0}},
            {0, 1},
            {.xy = 10,
                    .map = Map},
            OutPath
    );
    assert(res1);
    assert(OutPath[0] == 10);

/*
//    int64_t dimension = 5000;
//    std::vector<int> Map(dimension*dimension, 1);
//    int n = 0;
//    int y = 1;
//    int x = 0;
//    for (auto i = dimension + x; i < dimension*dimension; i++) {
//        if (n < dimension-1) {
//            n++;
//        } else {
//            y += 2;
//            if (x == 0)
//                x = 1;
//            else
//                x = 0;
//            i = dimension*y + x;
//            n = 1;
//        }
//        Map[i] = 0;
//    }

//    std::vector<int> Map = {1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1};

//    std::vector<int> Map = {0, 0, 1, 0, 1, 1, 1, 0, 1};


            std::vector<int> OutPath;

            auto epoch = std::chrono::high_resolution_clock::from_time_t(0);
            auto before   = std::chrono::high_resolution_clock::now();
            auto before_mseconds = duration_cast<std::chrono::milliseconds>(before - epoch).count();

            std::cout << "Starting..." << std::endl;
//        auto foundPath = FindPath({0, 0}, {9, 10}, Map, {10, 11}, OutPath);
            auto foundPath = FindPath({0, 0}, {dimension-1, dimension-1}, Map, {dimension, dimension}, OutPath);
//    auto foundPath = FindPath({0, 0}, { n < y ? dimension - 1 : 0, dimension-1}, Map, {dimension, dimension}, OutPath);
//    auto foundPath = FindPath({0, 0}, {1, 2}, Map, {4, 3}, OutPath);
//    auto foundPath = FindPath({2, 0}, {0, 2}, Map, {3, 3}, OutPath);


            auto now   = std::chrono::high_resolution_clock::now();
            auto after_mseconds = duration_cast<std::chrono::milliseconds>(now - epoch).count();

            std::cout << "Found path: " << foundPath << std::endl;
//    if (foundPath) {
//        for (auto path: OutPath) {
//            std::cout << "Path: " << path << std::endl;
//        }
//        std::cout << "Length: " << OutPath.size() << std::endl;
//    }
            std::cout << "Time: " << (after_mseconds - before_mseconds) << " ms" << std::endl;

            return 0;
        }
        */


    },
};

void Test::run() {
    for (const auto& test : m_tests) {
        test();
    }
    std::cout << "Ended tests successfully" << std::endl;
}
