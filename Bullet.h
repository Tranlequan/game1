#ifndef BULLET_H
#define BULLET_H



#include <SDL.h>
#include "Wall.h"
#include "const.h"

class Bullet {
public:
    int x, y;
    int dx, dy;
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture; // Thêm texture vào class
    Bullet(int startX, int startY, int dirX, int dirY, SDL_Texture* tex); // Thay đổi constructor
    void move();
    void render(SDL_Renderer* renderer);
};

#endif // BULLET_H
