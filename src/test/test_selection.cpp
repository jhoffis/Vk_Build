#include <cassert>
#include <iostream>
#include "test_selection.h"
#include "rendering/Entity.h"
#include "models/Villager.h"
#include "path_finding.h"
#include "math/math_stuff.h"
#include "timer_util.h"

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
            auto line0 = PathFinder::getLineTilesDDA({0,0}, {10, 7});

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
            auto expectedResult = {
                    40, 42, 32 // ult
//            10, 20, 30, 41, 32 // decent
// normal           10, 20, 30, 40, 41, 42, 32
            };

            auto res0 = PathFinder::findPath(
                    {0, 0},
                    {2, 3},
                    {.xy = 10,
                            .map = Map},
                    OutPath
            );
            assert(res0);
            assert(std::equal(OutPath.begin(), OutPath.end(),
                              expectedResult.begin(), expectedResult.end()));

            OutPath.clear();
            auto res1 = PathFinder::findPath(
                    {0, 0},
                    {0, 1},
                    {.xy = 10,
                            .map = Map},
                    OutPath
            );
            assert(res1);
            assert(OutPath[0] == 10);

            OutPath.clear();
            auto s1 = Timer::nowNanos();
            auto res2 = PathFinder::findPath(
                    {0, 0},
                    {9, 9},
                    {.xy = 10,
                            .map = Map},
                    OutPath
            );
            auto s2 = Timer::nowNanos();
            std::cout << "Time path: " << (s2 - s1)  << " ns, " << (s2 - s1)  / 1000000. << " ms" << std::endl;
            assert(res2);
            expectedResult = {
                    40, 42, 32, 34, 4, 6, 16, 17, 27, 28, 48, 49, 69, 68, 88, 89, 99
            };
            assert(std::equal(OutPath.begin(), OutPath.end(),
                              expectedResult.begin(), expectedResult.end()));

            auto map = Map::Map{.xy = 10,
                    .map = Map};
            Villager::Vill vill{};
            auto res3 = PathFinder::findPath(
                    {0, 0},
                    {9.1, 9.3},
                    map,
                    OutPath
            );

            assert(map.indexToWorld(9).x == 9*Map::tileSize);
            assert(map.indexToWorld(10).y == 1*Map::tileSize);
            PathFinder::convertMapPathToWorldPath(map, OutPath, (std::vector<Vec2> &) vill.path);

            auto index = PathFinder::nextClearLineTilesDDA(
                    map,
                    {0,0},
                    vill.path,
                    0
            );

            for (int i = 0; i < 100; i++) {
                vill.update(1);
            }

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
