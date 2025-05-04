#include "menu.h"
#include <SDL_image.h>
#include <iostream>
#include <sstream>
#include <iomanip>

Menu::Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight),
      backgroundTexture(nullptr), titleFont(nullptr), buttonFont(nullptr),
      titleTexture(nullptr), playButtonTexture(nullptr),
      bestScoreLabelTexture(nullptr), bestScoreValueTexture(nullptr) {}

Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(bestScoreLabelTexture);
    SDL_DestroyTexture(bestScoreValueTexture);
    TTF_CloseFont(titleFont);
    TTF_CloseFont(buttonFont);
}

void Menu::setBestScore(int score) {
    bestScore = score;
}

bool Menu::loadMedia() {
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return false;
    }

    titleFont = TTF_OpenFont("stencilla.ttf", 90);
    buttonFont = TTF_OpenFont("impact.ttf", 50);
    if (!titleFont || !buttonFont) {
        std::cerr << "Font load error\n";
        return false;
    }

    // Màu chữ phù hợp tone tối
    SDL_Color titleColor = { 224, 224, 224 };     // xám nhạt
    SDL_Color buttonTextColor = { 255, 255, 255 }; // trắng sáng
    SDL_Color bestScoreColor = { 221, 221, 221 };  // xám trắng

    // Tạo texture chữ
    titleTexture = loadTextTexture("BATTLE CITY", titleColor, titleFont);
    playButtonTexture = loadTextTexture("PLAY GAME", buttonTextColor, buttonFont);
    bestScoreLabelTexture = loadTextTexture("HIGH SCORE", bestScoreColor, buttonFont);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(6) << bestScore;
    bestScoreValueTexture = loadTextTexture(ss.str(), bestScoreColor, buttonFont);

    // Layout
    int w, h;

    SDL_QueryTexture(titleTexture, nullptr, nullptr, &w, &h);
    titleRect = { (screenWidth - w) / 2, 60, w, h };

    SDL_QueryTexture(playButtonTexture, nullptr, nullptr, &w, &h);
    playButtonRect = { (screenWidth - w) / 2, 230, w, h };
    playButtonOutlineRect = { playButtonRect.x - 20, playButtonRect.y - 10, w + 40, h + 20 };

    SDL_QueryTexture(bestScoreLabelTexture, nullptr, nullptr, &w, &h);
    bestScoreLabelRect = { (screenWidth - w) / 2, 350, w, h };

    SDL_QueryTexture(bestScoreValueTexture, nullptr, nullptr, &w, &h);
    bestScoreValueRect = { (screenWidth - w) / 2, bestScoreLabelRect.y + bestScoreLabelRect.h + 10, w, h };

    return true;
}

SDL_Texture* Menu::loadTextTexture(const std::string& text, SDL_Color color, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Menu::handleEvent(SDL_Event& e, bool& quit, bool& playGame) {
    if (e.type == SDL_QUIT) {
        quit = true;
    } else if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= playButtonRect.x && x <= playButtonRect.x + playButtonRect.w &&
            y >= playButtonRect.y && y <= playButtonRect.y + playButtonRect.h) {
            playGame = true;
        }
    }
}

void Menu::render() {
    // Nền xám đậm city tone
    SDL_SetRenderDrawColor(renderer, 26, 26, 26, 255); // #1A1A1A
    SDL_RenderClear(renderer);

    // Tiêu đề
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);

    // Nút PLAY
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // nền nâu đậm
    SDL_RenderFillRect(renderer, &playButtonOutlineRect);

    SDL_SetRenderDrawColor(renderer, 205, 133, 63, 255); // viền nâu nhạt
    SDL_RenderDrawRect(renderer, &playButtonOutlineRect);

    SDL_RenderCopy(renderer, playButtonTexture, nullptr, &playButtonRect);

    // HIGH SCORE
    SDL_RenderCopy(renderer, bestScoreLabelTexture, nullptr, &bestScoreLabelRect);
    SDL_RenderCopy(renderer, bestScoreValueTexture, nullptr, &bestScoreValueRect);
}

bool Menu::show() {
    bool quit = false;
    bool playGame = false;
    SDL_Event e;

    if (!loadMedia()) return false;

    while (!quit && !playGame) {
        while (SDL_PollEvent(&e)) {
            handleEvent(e, quit, playGame);
        }

        render();
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return playGame;
}
