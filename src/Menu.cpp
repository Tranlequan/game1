#include "menu.h"
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

Menu::Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight),
      backgroundTexture(nullptr), titleFont(nullptr), buttonFont(nullptr),
      titleTexture1(nullptr), titleTexture2(nullptr),
      playButtonTexture(nullptr),
      bestScoreLabelTexture(nullptr), bestScoreValueTexture(nullptr),
      bestScore(0) {}

Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(titleTexture1);
    SDL_DestroyTexture(titleTexture2);
    SDL_DestroyTexture(playButtonTexture);
    SDL_DestroyTexture(bestScoreLabelTexture);
    SDL_DestroyTexture(bestScoreValueTexture);
    TTF_CloseFont(titleFont);
    TTF_CloseFont(buttonFont);
}

void Menu::setBestScore(int score) {
    bestScore = score;
}

void Menu::saveHighScore(int score) {
    int highscore;
    std::ifstream file("highscore.txt");

    // Nếu file tồn tại, đọc điểm cao nhất
    if (file.is_open()) {
        file >> highscore;
        file.close();
    } else {
        highscore = 0; // Nếu không có file, mặc định điểm cao nhất là 0
    }

    // Lưu điểm nếu điểm hiện tại lớn hơn điểm cao nhất
    if (score > highscore) {
        std::ofstream file_out("highscore.txt");
        if (file_out.is_open()) {
            file_out << score;
            file_out.close();
        }
    }
}

bool Menu::loadMedia() {
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Load background image
    SDL_Surface* bgSurface = IMG_Load("background1.png");
    if (!bgSurface) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
        return false;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    titleFont = TTF_OpenFont("pixel.ttf", 100);
    buttonFont = TTF_OpenFont("stencilla.ttf", 50);
    if (!titleFont || !buttonFont) {
        std::cerr << "Font load error\n";
        return false;
    }

    // Màu chữ
    SDL_Color titleColor = {255, 255, 180};       // Vàng cổ
    SDL_Color buttonTextColor = {255, 255, 255};  // Trắng
    SDL_Color bestScoreColor = {221, 221, 221};   // Xám trắng

    // Đọc điểm cao nhất từ file
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> bestScore;
        file.close();
    } else {
        bestScore = 0;  // Nếu không có file, mặc định là 0
    }

    // Tạo texture tiêu đề từng dòng
    titleTexture1 = loadTextTexture("BATTLE", titleColor, titleFont);
    titleTexture2 = loadTextTexture("CITY", titleColor, titleFont);
    if (!titleTexture1 || !titleTexture2) return false;

    int w1, h1, w2, h2;
    SDL_QueryTexture(titleTexture1, nullptr, nullptr, &w1, &h1);
    SDL_QueryTexture(titleTexture2, nullptr, nullptr, &w2, &h2);
    titleRect1 = { (screenWidth - w1) / 2, 50, w1, h1 };
    titleRect2 = { (screenWidth - w2) / 2, titleRect1.y + h1 + 10, w2, h2 };

    playButtonTexture = loadTextTexture("PLAY GAME", buttonTextColor, buttonFont);
    if (!playButtonTexture) return false;

    SDL_QueryTexture(playButtonTexture, nullptr, nullptr, &w1, &h1);
    playButtonRect = { (screenWidth - w1) / 2, titleRect2.y + h2 + 50, w1, h1 };
    playButtonOutlineRect = { playButtonRect.x - 20, playButtonRect.y - 10, w1 + 40, h1 + 20 };

    bestScoreLabelTexture = loadTextTexture("HIGH SCORE", bestScoreColor, buttonFont);
    if (!bestScoreLabelTexture) return false;

    SDL_QueryTexture(bestScoreLabelTexture, nullptr, nullptr, &w1, &h1);
    bestScoreLabelRect = { (screenWidth - w1) / 2, playButtonRect.y + h1 + 60, w1, h1 };

    // Hiển thị điểm cao nhất
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(6) << bestScore;
    bestScoreValueTexture = loadTextTexture(ss.str(), bestScoreColor, buttonFont);
    if (!bestScoreValueTexture) return false;

    SDL_QueryTexture(bestScoreValueTexture, nullptr, nullptr, &w1, &h1);
    bestScoreValueRect = { (screenWidth - w1) / 2, bestScoreLabelRect.y + h1 + 10, w1, h1 };

    return true;
}

SDL_Texture* Menu::loadTextTexture(const std::string& text, SDL_Color color, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
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
    // Dùng hình nền
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 26, 26, 26, 255); // fallback
        SDL_RenderClear(renderer);
    }

    // Tiêu đề 2 dòng
    SDL_RenderCopy(renderer, titleTexture1, nullptr, &titleRect1);
    SDL_RenderCopy(renderer, titleTexture2, nullptr, &titleRect2);

    // Nút PLAY
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);  // nền nâu đậm
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
