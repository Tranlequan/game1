#include "Explosion.h"
#include <SDL.h>

Explosion::Explosion(SDL_Rect pos) {
    rect = pos;
    frame = 0;
    active = true;
    startTime = SDL_GetTicks();  // Lưu thời gian bắt đầu
}

void Explosion::update() {
    Uint32 currentTime = SDL_GetTicks();
    frame = (currentTime - startTime) / frameDelay;

    if (frame >= 3) {  // Nếu bạn có nhiều frame hơn, hãy thay số 3 bằng frames.size()
        active = false;
    }
}

void Explosion::render(SDL_Renderer* renderer, const std::vector<SDL_Texture*>& frames) {
    if (frame < frames.size()) {
        SDL_RenderCopy(renderer, frames[frame], nullptr, &rect);
    }
}
