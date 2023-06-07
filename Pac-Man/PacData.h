#pragma once
#include <vector>
#include <string>

namespace dae
{
    namespace LevelData
    {
        const char PacMan = 'P';
        const char Dot = '.';
        const char Wall = '#';
        const char Empty = ' ';
        const char PowerUp = 'o';
        const char Gate = '=';

        const glm::vec4 WallColor = { 0.f, 0.f, 255.f, 255.f };
        const glm::vec4 EmptyColor = { 0.f, 0.f, 0.f, 255.f };
        const glm::vec4 WalkableColor = { 100.f, 100.f, 255.f, 255.f };
  
        using PacLevelData = std::vector<std::string>;
        const std::vector<PacLevelData> PacLevels
        {
            { // LEVEL 0 21x21
                "#####################",
                "##........#........##",
                "##o##.###.#.###.##o##",
                "##.................##",
                "##.##.#.#####.#.##.##",
                "##....#...#...#....##",
                "#####.### # ###.#####",
                "#####.#   0   #.#####",
                "#####.# ##=## #.#####",
                "     .  #123#  .     ",
                "#####.# ##### #.#####",
                "#####.#       #.#   #",
                "#####.# ##### #.#####",
                "##........#........##",
                "##.##.###.#.###.##.##",
                "##o.#.....P.....#.o##",
                "###.#.#.#####.#.#.###",
                "##....#...#...#....##",
                "##.######.#.######.##",
                "##.................##",
                "#####################"
            }
        };
    }

    namespace PacGridData
    {
        enum class PacNodeType
        {
            Empty,
            Wall,
            Dot,
            PowerUp,
            Gate,
            Spawn
        };

        struct PacNodeInfo
        {
            PacNodeType type;
            bool hasItem;
        };
    }
}