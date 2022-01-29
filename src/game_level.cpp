#include "game_level.h"

#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;


int movey[4] = {0, 0, -1, 1};
int movex[4] = {1, -1, 0, 0};
bool validgrid(vector<vector<int>>& grid, int blocks) {
    int y = grid.size(), x = grid[0].size();
    vector<vector<int>> bfs = grid;
    queue<pair<int,int>> q;
    q.push({0,0});
    bfs[0][0] = 2;
    int count = 1;
    while (!q.empty()) {
        pair<int, int> currpos = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int newy = currpos.first + movey[i];
            int newx = currpos.second + movex[i];
            if (newy < 0 || newx < 0) continue;
            if (newy >= y || newx >= x) continue;
            if (!bfs[newy][newx]) {
                bfs[newy][newx] = 2;
                count++;
                q.push({newy, newx});
            }
        }
    }
    return x * y == blocks + count;
}
void generateSprites(vector<vector<int>>& grid, unsigned int spriteCount, int spriteId, unsigned int manhattan = 0)
{
    int height = grid.size(), width = grid[0].size();
    for (int i = 0; i < spriteCount; i++) {
        int h = 0;
        int w = 0;
        while ((h == 0 && w == 0) || (h == height- 1 && w == width -1 ) || grid[h][w] != 0 || (h + w < manhattan)) {
            h = rand() % height;
            w = rand() % width;
        }
        grid[h][w] = spriteId;
    }
}
vector<vector<int>> generateGrid(unsigned int height, unsigned int width, unsigned int blocks, unsigned int coins,
                                 unsigned int monsters, unsigned int monsterDistance)
{
    srand(time(0));
    int count = 0;
    height -= 2;
    width -= 2;
    vector<vector<int>> smallgrid;
    while (1) {
        vector<vector<int>> grid(height, vector<int>(width, 0));
        for (int i = 0; i < blocks; i++) {
            int h = 0;
            int w = 0;
            while ((h == 0 && w == 0) || (h == height- 1 && w == width -1 ) || grid[h][w] != 0) {
                h = rand() % height;
                w = rand() % width;
            }
            grid[h][w] = 4;
        }
        if (validgrid(grid, blocks)) {
            cout << count << "\n";
            generateSprites(grid, coins, 2);
            generateSprites(grid, monsters, 3, monsterDistance);
            smallgrid = grid;
            break;
        }
        count++;
    }
    height += 2;
    width += 2;
    vector<vector<int>> grid(height, vector<int>(width, 0));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || j == 0) grid[i][j] = 4;
            else if (i == height - 1 || j == width - 1) grid[i][j] = 4;
            else grid[i][j] = smallgrid[i - 1][j - 1];
        }
    }
    return grid;
}

void GameLevel::Load(unsigned int levelHeight, unsigned int levelWidth, unsigned int gridHeight, unsigned int gridWidth,
                     unsigned int blocks, unsigned int coins, unsigned int monsters, unsigned int monsterDistance)
{
    // clear old data
    this->Boxes.clear();
    // load from file
    unsigned int tileCode;
    vector<vector<int>> grid = generateGrid(gridHeight, gridWidth, blocks, coins, monsters, monsterDistance);
//    if (tileData.size() > 0)
    this->init(grid, levelHeight, levelWidth);
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &box: this->Boxes)
            box.Draw(renderer);
    for (GameObject &monster : this->Monsters)
            monster.Draw(renderer);
    for (GameObject &coins : this->Coins)
        if (!coins.Destroyed)
            coins.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (GameObject &tile : this->Boxes)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<int>> grid, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    unsigned int height = grid.size();
    unsigned int width = grid[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;
    // initialize level tiles based on tileData
//    glm::vec2 pos(unit_width, unit_height);
//    glm::vec2 size(unit_width, unit_height);
//    GameObject obj(pos, size, ResourceManager::GetTexture("cowboy"), glm::vec3(1.0f,1.0f,1.0f));
//    obj.IsSolid = true;
//    this->Boxes.push_back(obj);
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (grid[y][x] == 4) // solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("box"), glm::vec3(1.0f,1.0f,1.0f));
                obj.IsSolid = true;
                this->Boxes.push_back(obj);
            }
            if (grid[y][x] == 3) // solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("alien"), glm::vec3(1.0f,1.0f,1.0f));
                obj.IsSolid = true;
                this->Monsters.push_back(obj);
            }
            if (grid[y][x] == 2) // solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::GetTexture("coin"), glm::vec3(1.0f,1.0f,1.0f));
                obj.IsSolid = false;
                this->Coins.push_back(obj);
            }
//            else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
//            {
//                glm::vec3 color = glm::vec3(1.0f); // original: white
//                if (tileData[y][x] == 2)
//                    color = glm::vec3(0.2f, 0.6f, 1.0f);
//                else if (tileData[y][x] == 3)
//                    color = glm::vec3(0.0f, 0.7f, 0.0f);
//                else if (tileData[y][x] == 4)
//                    color = glm::vec3(0.8f, 0.8f, 0.4f);
//                else if (tileData[y][x] == 5)
//                    color = glm::vec3(1.0f, 0.5f, 0.0f);
//
//                glm::vec2 pos(unit_width * x, unit_height * y);
//                glm::vec2 size(unit_width, unit_height);
//                this->Boxes.emplace_back(pos, size, ResourceManager::GetTexture("block"), color);
//            }
        }
    }
}