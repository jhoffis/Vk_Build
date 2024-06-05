#include <iostream>
#include "test_selection.h"
#include "rendering/Entity.h"
#include "models/Villager.h"
#include "path_finding.h"
#include "math/math_stuff.h"

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
            auto sqrt0 = MyMath::fast_sqrt(9);
            assert((int) sqrt0 == 3);
            auto sqrt1 = MyMath::fast_sqrt(5434);
            assert((int) sqrt1 == 73);
//            auto sqrt3 = MyMath::Q_rsprt(5434);
//            auto sqrt4 = MyMath::Q_rsprt2(5434);
//            assert(sqrt3 == sqrt4);
        },
        []() {


            std::vector<int> Map = {
                    1, 0, 1, 0, 1, 1, 1, 0, 1, 1,
                    1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
                    1, 1, 0, 0, 1, 0, 0, 1, 1, 1,
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
            auto expectedResult = {
                    30, 41, 32 // ult
//            10, 20, 30, 41, 32 // decent
// normal           10, 20, 30, 40, 41, 42, 32
            };
            assert(std::equal(OutPath.begin(), OutPath.end(),
                              expectedResult.begin(), expectedResult.end()));

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

            OutPath.clear();
            auto res2 = PathFinder::findPath(
                    {.pos = {0, 0, 0}},
                    {9, 9},
                    {.xy = 10,
                            .map = Map},
                    OutPath
            );
            assert(res2);
            expectedResult = {
                    30, 41, 42, 24, 14, 5, 38, 48, 59, 69, 78, 88, 99,
            };
            assert(std::equal(OutPath.begin(), OutPath.end(),
                              expectedResult.begin(), expectedResult.end()));

            auto map = Map::Map{.xy = 10,
                    .map = Map};
            Villager::Vill vill{};
            auto res3 = PathFinder::findPath(
                    *vill.entity,
                    {9.1, 9.3},
                    map,
                    OutPath
            );

            PathFinder::convertMapPathToWorldPath(map, OutPath, vill.path);
            vill.update(1);
            vill.update(1);
            vill.update(1);
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
    for (const auto &test: m_tests) {
        test();
    }
    std::cout << "Ended tests successfully" << std::endl;
}
