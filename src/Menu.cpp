#include "menu.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

Menu::Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight),
      backgroundTexture(nullptr), titleFont(nullptr), buttonFont(nullptr),
      titleTexture(nullptr), playButtonTexture(nullptr) {}

Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(playButtonTexture);
    TTF_CloseFont(titleFont);
    TTF_CloseFont(buttonFont);
}

bool Menu::loadMedia() {
    backgroundTexture = loadTexture("menu1.png");
    if (!backgroundTexture) return false;

    if (TTF_Init() < 0) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    titleFont = TTF_OpenFont("stencilla.ttf", 90);
    buttonFont = TTF_OpenFont("arial.ttf", 50);
    if (!titleFont || !buttonFont) {
        std::cerr << "Failed to load fonts.\n";
        return false;
    }

    SDL_Color white = {255, 255, 255};
    SDL_Color green = {0, 255, 0};

    titleTexture = loadTextTexture("BATTLE TANK", white, titleFont);
    playButtonTexture = loadTextTexture("PLAY GAME", green, buttonFont);

    if (!titleTexture || !playButtonTexture) return false;

    int titleW, titleH;
    SDL_QueryTexture(titleTexture, nullptr, nullptr, &titleW, &titleH);
    titleRect = { (screenWidth - titleW) / 2, 100, titleW, titleH };

    int buttonW, buttonH;
    SDL_QueryTexture(playButtonTexture, nullptr, nullptr, &buttonW, &buttonH);
    playButtonRect = { (screenWidth - buttonW) / 2, 350, buttonW, buttonH };

    // Tạo khung nền to hơn chữ một chút
    playButtonOutlineRect = {
        playButtonRect.x - 10,
        playButtonRect.y - 5,
        playButtonRect.w + 20,
        playButtonRect.h + 10
    };

    return true;
}

SDL_Texture* Menu::loadTextTexture(const std::string& text, SDL_Color color, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) return nullptr;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* Menu::loadTexture(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << "\n";
        return nullptr;
    }
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
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);

    // Vẽ nền vàng nhạt cho nút PLAY GAME
    SDL_SetRenderDrawColor(renderer, 255, 255, 102, 255); // Vàng nhạt
    SDL_RenderFillRect(renderer, &playButtonOutlineRect);

    // Vẽ viền đen quanh nút
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Đen
    SDL_RenderDrawRect(renderer, &playButtonOutlineRect);

    // Vẽ chữ PLAY GAME màu vàng
    SDL_RenderCopy(renderer, playButtonTexture, nullptr, &playButtonRect);

    // Reset lại màu render nếu cần tiếp tục vẽ thành phần khác
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}


bool Menu::show() {
    bool quit = false;
    bool playGame = false;
    SDL_Event e;

    if (!loadMedia()) return false;

    while (!quit && !playGame) {
        while (SDL_PollEvent(&e) != 0) {
            handleEvent(e, quit, playGame);
        }

        SDL_RenderClear(renderer);
        render();
        SDL_RenderPresent(renderer);
    }

    return playGame;
}
