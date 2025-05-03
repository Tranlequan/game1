#include "Bullet.h"

// Constructor của Bullet
Bullet::Bullet::Bullet(int startX, int startY, int dirX, int dirY, SDL_Texture* tex) {
    x = startX;
    y = startY;
    dx = dirX ;
    dy = dirY;
    active = true;
    rect = {x, y, 10, 10};
    texture = tex;
}


void Bullet::move() {
        x += dx ; // cập nhật tọa độ
        y += dy;
        rect.x = x; // đồng bộ lại hình vẽ
        rect.y = y;
        // nếu đạn ra khỏi màn hình => tắt
        if (x < TILE_SIZE || x > SCREEN_WIDTH - TILE_SIZE ||
            y < TILE_SIZE || y > SCREEN_HEIGHT - TILE_SIZE) {
            active = false;
        }
    }

// Hàm render Bullet lên màn hình
void Bullet::render(SDL_Renderer* renderer) {
    if (active && texture) {
        double angle = 0.0;
        // xác định góc xoay theo hướng di chuyển
        if (dx == 0 && dy < 0) angle = 0.0;         // UP
        else if (dx == 0 && dy > 0) angle = 180.0;  // DOWN
        else if (dx < 0 && dy == 0) angle = 270.0;  // LEFT
        else if (dx > 0 && dy == 0) angle = 90.0;   // RIGHT
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }
}

