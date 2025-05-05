#include "Explosion.h"

Explosion::Explosion(SDL_Rect pos) {
    rect = pos;
    frame = 0;
    active = true;
}

void Explosion::update() {
    frame++;
    if (frame >= 3) {  // Số lượng frame ảnh nổ, nếu bạn có nhiều hơn thì sửa lại
        active = false;
    }
}

void Explosion::render(SDL_Renderer* renderer, const std::vector<SDL_Texture*>& frames) {
    if (frame < frames.size()) {
        SDL_RenderCopy(renderer, frames[frame], nullptr, &rect);
    }
}
