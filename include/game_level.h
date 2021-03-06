#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

class GameLevel
{
    public:
    // level state
//    std::vector<std::vector<int>> grid;
    std::vector<GameObject> Boxes;
    std::vector<GameObject> Coins;
    std::vector<GameObject> Monsters;
    std::vector<std::vector<int>> grid;
    int exitIndex;
    // constructor
    GameLevel() { }
    // loads level from file
    void Load(unsigned int levelHeight, unsigned int levelWidth, unsigned int gridHeight, unsigned int gridWidth,
                         unsigned int blocks, unsigned int coins, unsigned int monsters, unsigned int monsterDistance);
    // render level
    void Draw(SpriteRenderer &renderer, int LightsOn, glm::vec2 PlayerPos);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
    private:
    // initialize level from tile data
    void init(unsigned int levelHeight, unsigned int levelWidth);
};

#endif