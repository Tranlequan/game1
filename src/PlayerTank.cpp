#include "PlayerTank.h"
#include <algorithm>

PlayerTank::PlayerTank(int startX, int startY, SDL_Renderer* renderer, SDL_Texture* tex, SDL_Texture* bulletTex) {
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = -1;
        texture = tex;
        bulletTexture = bulletTex;
    }


    void PlayerTank ::move(int dx, int dy, const std::vector<Wall>& walls) {
        int newX = x + dx;
        int newY = y + dy;
        this ->dirX = dx;
        this ->dirY = dy;

        SDL_Rect newRect = {newX, newY, TILE_SIZE, TILE_SIZE};
        for(size_t i = 0; i < walls.size(); i++){
            if(walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)){
                return;
            }
        }

        if(newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
           newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2){
               x = newX;
               y = newY;
               rect.x = x;
               rect.y = y;
           }
    }
    void PlayerTank::render(SDL_Renderer* renderer) {
    if (texture) {
                double angle = 0.0;
        if (dirX == 0 && dirY < 0) angle = 0.0;
        else if (dirX == 0 && dirY > 0) angle = 180.0;
        else if (dirX < 0 && dirY == 0) angle = 270.0;
        else if (dirX > 0 && dirY == 0) angle = 90.0;

        SDL_RenderCopyEx(renderer, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    }
    for (auto& bullet : Bullets) bullet.render(renderer);
}
void PlayerTank::shoot() {
       Bullets.emplace_back(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5, dirX, dirY, bulletTexture);

    }
void PlayerTank::updateBullets() {
        for (auto& bullet : Bullets) bullet.move();
        Bullets.erase(remove_if(Bullets.begin(), Bullets.end(),
                                [](Bullet& b) { return !b.active; }), Bullets.end());
    }
