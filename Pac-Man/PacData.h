#pragma once
#include <vector>
#include <string>
#include <glm/vec4.hpp>

namespace PacData
{
     const std::string PacJSONpath = "Resources/PacData.json";
     const std::string PacFont = "PacFont.otf";
     
     struct PacLevelData
     {
         std::vector<std::string> map;
     };

     struct PacGameData
     {
         using PacMap = std::vector<std::string>;
         std::vector<PacMap> maps;
     };

     constexpr char PacSpawn = 'P';
     constexpr char Dot = '.';
     constexpr char Wall = '#';
     constexpr char Empty = ' ';
     constexpr char PowerUp = 'o';
     constexpr char Gate = '=';


     constexpr glm::vec4 WallColor = { 0.f, 0.f, 255.f, 255.f };
     constexpr glm::vec4 EmptyColor = { 0.f, 0.f, 0.f, 255.f };
     constexpr glm::vec4 InteractableColor = { 255.f, 255.f, 0.f, 255.f };
     constexpr glm::vec4 WalkableColor = { 100.f, 100.f, 255.f, 255.f };

     constexpr int PacDotScore = 10;
     constexpr int PacPowerUpScore = 50;
    
     namespace PacTags
     {
         const std::string PacMan = "PacMan";
         const std::string Ghost = "Ghost";
     }

     enum class PacNodeType
     {
         EMPTY,
         WALL,
         DOT,
         POWERUP,
         GATE,
         PAC_SPAWN,
         NPC_SPAWN
     };

     enum class PacGhosts
     {
         BLINKY,
         PINKY,
         INKY,
         CLYDE
     };

     struct PacNodeInfo
     {
         PacNodeType type;
         bool hasItem;
     };
    
}