#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class Menu {
public:
    Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight);
    ~Menu();

    bool loadMedia();
    void handleEvent(SDL_Event& e, bool& quit, bool& playGame);
    void render();
    bool show();

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    TTF_Font* titleFont;
    TTF_Font* buttonFont;

    SDL_Texture* titleTexture;
    SDL_Texture* playButtonTexture;

    SDL_Rect titleRect;
    SDL_Rect playButtonRect;
    SDL_Rect playButtonOutlineRect;

    int screenWidth, screenHeight;

    SDL_Texture* loadTextTexture(const std::string& text, SDL_Color color, TTF_Font* font);
    SDL_Texture* loadTexture(const std::string& path);
};

#endif
