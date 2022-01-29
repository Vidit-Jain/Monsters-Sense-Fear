#include <sprite_renderer.h>
#include "game.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include <bits/stdc++.h>
#include "game_level.h"
SpriteRenderer  *Renderer;
using namespace std;
GameObject *Player;
const int gridHeight = 20;
const int gridWidth = 20;
float unit_width, unit_height;
int points = 0;
int dy[4] = {1, 0, -1, 0};
int dx[4] = {0, -1, 0, 1};
Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    unit_width = width / (float) gridWidth;
    unit_height = width / (float) gridHeight;
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
    int blocks = 25, coins = 5, monsters = 5, monsterDistance = 6;
    auto player_pos = glm::vec2(unit_width, unit_height);
    auto player_size = glm::vec2(unit_width * 0.85f, unit_height * 0.85f);
    Player = new GameObject(player_pos, player_size, ResourceManager::GetTexture("cowboy"));

    s.Load(this->Height, this->Width, gridHeight, gridWidth, blocks, coins, monsters, monsterDistance);
    this->Levels.push_back(s);
    this->Level = 0;
}

void Game::Update(float dt)
{
}
void Game::reAdjust(int i, GameObject& a, GameObject b, float velocity) {
    if (i == 1 && this->CheckCollision(a, b)) {
        a.Position += glm::vec2(0.0f, 1.0f) * velocity;
        a.Position -= glm::vec2(0.0f, a.Position.y - b.Size.y - b.Position.y);
    }
    if (i == 2 && this->CheckCollision(a, b)) {
        a.Position -= glm::vec2(1.0f, 0.0f) * velocity;
        a.Position += glm::vec2(b.Position.x - a.Position.x - a.Size.x, 0.0f);
    }
    if (i == 3 && this->CheckCollision(a, b)) {
        a.Position -= glm::vec2(0.0f, 1.0f) * velocity;
        a.Position += glm::vec2(0.0f, b.Position.y - a.Position.y - a.Size.y);
    }
    if (i == 4 && this->CheckCollision(*Player, b)) {
        a.Position += glm::vec2(1.0f, 0.0f) * velocity;
        a.Position -= glm::vec2(a.Position.x - b.Size.x - b.Position.x, 0.0f);
    }
}
void Game::MonsterMove(float dt) {
    auto& level = this->Levels[this->Level];
    vector<vector<int>> currpos(gridHeight, vector<int>(gridWidth, 0));
    int px = Player->Position.x / unit_width;
    int py = Player->Position.y / unit_height;
    currpos[py][px] = 1;
    queue<pair<int, int>> q;
    q.push({py, px});
    while (!q.empty()) {
        pair<int,int> curr = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int y = curr.first + dy[i];
            int x = curr.second + dx[i];
            if (y < 0 || x < 0) continue;
            if (x >= gridWidth || y >= gridHeight) continue;
            if (currpos[y][x] == 0 && level.grid[y][x] != 4) {
                currpos[y][x] = i + 1;
                q.push({y, x});
            }
        }
    }
    glm::vec2 movement[4] = {glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 0.0f),glm::vec2(0.0f, 1.0f), glm::vec2(-1.0f,0.0f)};
    for (auto& s : level.Monsters) {
        int mx = s.Position.x / unit_width;
        int my = s.Position.y / unit_height;
        int a = currpos[my][mx] - 1;
        s.Position += movement[currpos[my][mx] - 1] * 150.0f * dt;
    }
}
void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = 150.0f * dt;
        if (this->Keys[GLFW_KEY_W]) {
            MonsterMove(dt);
            Player->Position -= glm::vec2(0.0f, 1.0f) * velocity;
            for (auto& box : this->Levels[this->Level].Boxes) {
                if (box.Destroyed) continue;
                reAdjust(1, *Player, box, velocity);
//                if (this->CheckCollision(*Player, box)) {
//                    Player->Position += glm::vec2(0.0f, 1.0f) * velocity;
//                    Player->Position -= glm::vec2(0.0f, Player->Position.y - box.Size.y - box.Position.y);
//                }
            }
        }
        if (this->Keys[GLFW_KEY_D]) {
            Player->Position += glm::vec2(1.0f, 0.0f) * velocity;
            for (auto& box : this->Levels[this->Level].Boxes) {
                if (box.Destroyed) continue;
                reAdjust(2, *Player, box, velocity);
//                if (this->CheckCollision(*Player, box)) {
//                    Player->Position -= glm::vec2(1.0f, 0.0f) * velocity;
//                    Player->Position += glm::vec2(box.Position.x - Player->Position.x - Player->Size.x, 0.0f);
//                }
            }
        }
        if (this->Keys[GLFW_KEY_S]) {
            Player->Position += glm::vec2(0.0f, 1.0f) * velocity;
            for (auto& box : this->Levels[this->Level].Boxes) {
                if (box.Destroyed) continue;
                reAdjust(3, *Player, box, velocity);
//                if (this->CheckCollision(*Player, box)) {
//                    Player->Position -= glm::vec2(0.0f, 1.0f) * velocity;
//                    Player->Position += glm::vec2(0.0f, box.Position.y - Player->Position.y - Player->Size.y);
//                }
            }
        }
        if (this->Keys[GLFW_KEY_A]) {
            MonsterMove(dt);
            Player->Position -= glm::vec2(1.0f, 0.0f) * velocity;
            for (auto& box : this->Levels[this->Level].Boxes) {
                if (box.Destroyed) continue;
                reAdjust(4, *Player, box, velocity);
//                if (this->CheckCollision(*Player, box)) {
//                    Player->Position += glm::vec2(1.0f, 0.0f) * velocity;
//                    Player->Position -= glm::vec2(Player->Position.x - box.Size.x - box.Position.x, 0.0f);
//                }
            }
        }
    }
}

void Game::Render()
{
    this->DoCollision();
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                         glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    this->Levels[this->Level].Draw(*Renderer);
    Player->Draw(*Renderer);
}

bool Game::CheckCollision(GameObject &one, GameObject &two)
{
    // collision x-axis?
    bool collisionX = (min(two.Position.x + two.Size.x, one.Position.x + one.Size.x)
                        -  max(two.Position.x, one.Position.x)) > 0;
    // collision y-axis?
    bool collisionY = (min(two.Position.y + two.Size.y, one.Position.y + one.Size.y)
                       -  max(two.Position.y, one.Position.y)) > 0;
    // collision x-axis?
//    bool collisionX = one.Position.x + one.Size.x > two.Position.x &&
//                      two.Position.x + two.Size.x > one.Position.x;
//    // collision y-axis?
//    bool collisionY = one.Position.y + one.Size.y > two.Position.y &&
//                      two.Position.y + two.Size.y > one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}
void Game::DoCollision() {
    for (auto& coin : this->Levels[this->Level].Coins) {
        if (coin.Destroyed) continue;
        if (this->CheckCollision(*Player, coin)) {
            coin.Destroyed = true;
            points++;
            if (points == this->Levels[this->Level].Coins.size()) {
                auto& x = this->Levels[this->Level];
                x.Boxes[x.exitIndex].Destroyed = true;
            }
        }
    }
}