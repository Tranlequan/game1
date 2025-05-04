#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iomanip>

class Menu {
public:
    Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~Menu();

    bool show();                          // Hiện menu, trả về true nếu chọn Play
    void setBestScore(int score);        // Cập nhật điểm cao nhất

private:
    SDL_Renderer* renderer;
    int screenWidth, screenHeight;

    TTF_Font* titleFont;
    TTF_Font* buttonFont;

    SDL_Texture* backgroundTexture;
    SDL_Texture* titleTexture;
    SDL_Texture* playButtonTexture;
    SDL_Texture* bestScoreLabelTexture;
    SDL_Texture* bestScoreValueTexture;

    SDL_Rect titleRect;
    SDL_Rect playButtonRect;
    SDL_Rect playButtonOutlineRect;
    SDL_Rect bestScoreLabelRect;
    SDL_Rect bestScoreValueRect;

    int bestScore = 0;

    bool loadMedia();
    void handleEvent(SDL_Event& e, bool& quit, bool& playGame);
    void render();
    SDL_Texture* loadTextTexture(const std::string& text, SDL_Color color, TTF_Font* font);
};

#endif
