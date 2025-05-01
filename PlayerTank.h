#ifndef PLAYERTANK_H
#define PLAYERTANK_H
#include <SDL.h>
#include <vector>
#include <algorithm>
#include "Wall.h"
#include "const.h"
#include "Bullet.h"

class PlayerTank
{
    public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    SDL_Texture* texture;
    std::vector<Bullet> Bullets;
    PlayerTank(int startX, int startY, SDL_Texture* tex);
    void move(int dx, int dy, const std::vector<Wall>& walls) ;
    void shoot();
    void updateBullets();
    void render(SDL_Renderer* renderer);
};

#endif // PLAYERTANK_H
