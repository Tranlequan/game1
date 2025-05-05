#pragma once
#include <SDL.h>
#include <vector>

class Explosion {
public:
    SDL_Rect rect;                    // Vị trí và kích thước vụ nổ
    int frame;                        // Frame hiện tại
    bool active;                      // Còn hiển thị hay không

    Explosion(SDL_Rect pos);         // Hàm khởi tạo
    void update();                   // Cập nhật frame
    void render(SDL_Renderer* renderer, const std::vector<SDL_Texture*>& frames); // Vẽ
};
