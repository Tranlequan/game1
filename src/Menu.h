#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Menu {
public:
    Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~Menu();

    bool loadMedia();
    bool show();
    void setBestScore(int score);
    void saveHighScore(int score);

private:
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
    int bestScore;

    SDL_Texture* backgroundTexture;
    SDL_Texture* titleTexture1;
    SDL_Texture* titleTexture2;
    SDL_Texture* playButtonTexture;
    SDL_Texture* bestScoreLabelTexture;
    SDL_Texture* bestScoreValueTexture;
    TTF_Font* titleFont;
    TTF_Font* buttonFont;

    SDL_Rect titleRect1;
    SDL_Rect titleRect2;
    SDL_Rect playButtonRect;
    SDL_Rect playButtonOutlineRect;
    SDL_Rect bestScoreLabelRect;
    SDL_Rect bestScoreValueRect;

    SDL_Texture* loadTextTexture(const std::string& text, SDL_Color color, TTF_Font* font);
    void handleEvent(SDL_Event& e, bool& quit, bool& playGame);
    void render();
};

#endif
