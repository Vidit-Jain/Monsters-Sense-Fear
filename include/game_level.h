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
    // constructor
    GameLevel() { }
    // loads level from file
    void Load(unsigned int levelHeight, unsigned int levelWidth, unsigned int gridHeight, unsigned int gridWidth,
                         unsigned int blocks, unsigned int coins, unsigned int monsters, unsigned int monsterDistance);
    // render level
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
    private:
    // initialize level from tile data
    void init(std::vector<std::vector<int>> tileData, unsigned int levelHeight, unsigned int levelWidth);
};

#endif