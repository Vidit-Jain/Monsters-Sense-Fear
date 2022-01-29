#include <sprite_renderer.h>
#include "game.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include <bits/stdc++.h>
#include "game_level.h"
SpriteRenderer  *Renderer;
using namespace std;
GameObject *Player;
Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    
}

void Game::Init()
{

    ResourceManager::LoadShader("../shaders/vertex.vs", "../shaders/fragment.vs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("../textures/crate.jpg", false, "box");
    ResourceManager::LoadTexture("../textures/alien.png", true, "alien");
    ResourceManager::LoadTexture("../textures/coin.png", true, "coin");
    ResourceManager::LoadTexture("../textures/cowboy.png", true, "cowboy");
    ResourceManager::LoadTexture("../textures/background.png", false, "background");
    GameLevel s;
    int gridHeight = 20, gridWidth = 20, blocks = 25, coins = 5, monsters = 5, monsterDistance = 6;
    float unit_width = this->Width / (float)gridWidth, unit_height = this->Height / (float)gridHeight;
    auto player_size = glm::vec2(unit_width, unit_height);
    Player = new GameObject(player_size, player_size, ResourceManager::GetTexture("cowboy"));

    s.Load(this->Height, this->Width, gridHeight, gridWidth, blocks, coins, monsters, monsterDistance);
    this->Levels.push_back(s);
    this->Level = 0;
}

void Game::Update(float dt)
{
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
//        float velocity = 4.5f * dt;
//        if (this->Keys[GLFW_KEY_A]) {
//            position -= glm::vec2(1.0f, 0.0f) * 8.0f;
//        }
//        if (this->Keys[GLFW_KEY_D]) {
//            position += glm::vec2(1.0f, 0.0f) * 8.0f;
//        }
//        if (this->Keys[GLFW_KEY_W]) {
//            position -= glm::vec2(0.0f, 1.0f) * 8.0f;
//        }
//        if (this->Keys[GLFW_KEY_S]) {
//            position += glm::vec2(0.0f, 1.0f) * 8.0f;
//        }
//        float velocity = PLAYER_VELOCITY * dt;
//        // move playerboard
//        if (this->Keys[GLFW_KEY_A])
//        {
//            if (Player->Position.x >= 0.0f)
//            {
//                Player->Position.x -= velocity;
//                if (Ball->Stuck)
//                    Ball->Position.x -= velocity;
//            }
//        }
//        if (this->Keys[GLFW_KEY_D])
//        {
//            if (Player->Position.x <= this->Width - Player->Size.x)
//            {
//                Player->Position.x += velocity;
//                if (Ball->Stuck)
//                    Ball->Position.x += velocity;
//            }
//        }
//        if (this->Keys[GLFW_KEY_SPACE])
//            Ball->Stuck = false;
    }
}

void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                         glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    this->Levels[this->Level].Draw(*Renderer);
    Player->Draw(*Renderer);
}