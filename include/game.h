#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"
#include <vector>
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOSE,
};

class Game
{
public:
    GameState               State;	
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    unsigned int            Level;
    int                     LightsOn;
    int                     CheatsOn;
    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    bool CheckCollision(GameObject &one, GameObject &two);
    void DoCollision();
    void MonsterMove(float velocity);
    void reAdjust(int i, GameObject& a, GameObject b, float velocity);
};

#endif