#include "Bullet.h"

Bullet::Bullet (int startX, int startY, int dirX, int dirY) {
x = startX;
y = startY;
dx = dirX;
dy = dirY;
active = true;
rect = {x, y, 10, 10};
}
void Bullet::move() {
        x += dx; // cập nhật tọa độ
        y += dy;
        rect.x = x; // đồng bộ lại hình vẽ
        rect.y = y;
        // nếu đạn ra khỏi màn hình => tắt
        if (x < TILE_SIZE || x > SCREEN_WIDTH - TILE_SIZE ||
            y < TILE_SIZE || y > SCREEN_HEIGHT - TILE_SIZE) {
            active = false;
        }
    }
void Bullet::render (SDL_Renderer* renderer) {
    if (active){
    SDL_SetRenderDrawColor (renderer,255, 255, 255, 255);
    SDL_RenderFillRect (renderer, &rect);
}
}
