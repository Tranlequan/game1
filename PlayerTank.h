#ifndef PLAYERTANK_H
#define PLAYERTANK_H
#include <SDL.h>
#include <vector>
#include "Wall.h"
#include "const.h"

class PlayerTank
{
    public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;

    PlayerTank (int startX, int startY) ;
    void move(int dx, int dy, const std::vector<Wall>& walls) ;
    void render(SDL_Renderer* renderer);
};

#endif // PLAYERTANK_H
