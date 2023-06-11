#pragma once
#include <vector>
#include <string>
#include <glm/vec4.hpp>

namespace PacData
{
    namespace PacFiles
    {
        const std::string PacJSON = "PacData.json";
        const std::string PacFont = "Fonts/PacFont.otf";
        const std::string PacMan = "Img/pacman.png";
        const std::string SinglePlayerBtn = "Img/SinglePlayer.png";
        const std::string CoopBtn = "Img/Coop.png";
        const std::string VersusBtn = "Img/Versus.png";
        const std::string Blinky = "Img/Blinky.png";
        const std::string Pinky = "Img/Pinky.png";
        const std::string Inky = "Img/Inky.png";
        const std::string Clyde = "Img/Clyde.png";
        const std::string VulnerableGhost = "Img/VulnerableGhost.png";
        const std::string InvertedGhost = "Img/InvertedGhost.png";
        const std::string NPCEyes = "Img/NPCEyes.png";
        const std::string ImpostorGhost = "Img/ImpostorGhost.png";
        const std::string PacDeathSound = "Audio/pacdeath.mp3";
        const std::string WAKAWAKAWAKA = "Audio/wakawakawaka.mp3";
    }

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
         CLYDE,
     };

     struct PacNodeInfo
     {
         PacNodeType type;
         bool hasItem;
     };

     struct PacPlayerInfo
     {
         char name[3];
         int score;
     };

     enum class PacGameMode
     {
         SINGLE,
         COOP,
         VERSUS
     };


     struct PacLevelData
     {
         std::vector<std::string> map;
         int dotCount;
         int powerupCount;
     };

     struct PacGameData
     {
         using PacMap = std::vector<std::string>;

         PacGameMode mode;
         std::vector<PacMap> maps;
     };
    
}