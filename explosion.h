#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <SDL.h>
#include <vector>

class Explosion {
public:
    SDL_Rect rect;
    int frame;
    bool active;
    Uint32 startTime;        // Thời gian bắt đầu hiệu ứng nổ
    int frameDelay = 100;    // Độ trễ giữa các frame (ms), có thể điều chỉnh

    Explosion(SDL_Rect pos);
    void update();
    void render(SDL_Renderer* renderer, const std::vector<SDL_Texture*>& frames);
};

#endif
