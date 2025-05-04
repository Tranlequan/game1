#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include "Wall.h"
#include "const.h"
#include "PlayerTank.h"
#include "Bullet.h"
#include "EnemyTank.h"
#include "menu.h"
#include <fstream>


using namespace std;

SDL_Texture* loadTexture(const string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (!newTexture) {
        cerr << "Failed to load image " << path << "! SDL_image Error: "<< IMG_GetError() << endl;
    }
    return newTexture;
}

class Game
{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    SDL_Texture* backgroundTexture;
    SDL_Texture* wallTexture;
    SDL_Texture* bulletTexture;
    SDL_Texture* playerTexture;
    SDL_Texture* enemyTexture;
    TTF_Font* font;
    vector<Wall> walls;

    PlayerTank* player;

    int enemyNumber = 10;
    vector<EnemyTank> enemies;
    Mix_Music* bgMusic;
    int score = 0;
    int highScore = 0;
Game()
{
    running = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        running = false;
        return;
    }

    window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
        running = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
        running = false;
        return;
    }

    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        running = false;
        return;
    }

    // Tải nhạc nền
    bgMusic = Mix_LoadMUS("background.mp3");
    if (!bgMusic) {
        cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << endl;
        running = false;
        return;
    }

    Mix_PlayMusic(bgMusic, -1); // -1 = lặp vô hạn

    // Khởi tạo SDL_ttf
    if (TTF_Init() < 0) {
        cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
        running = false;
        return;
    }

    // Tải font
    font = TTF_OpenFont("impact.ttf", 36);
    if (!font) {
        cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
        running = false;
        return;
    }

    // Tải texture
    backgroundTexture = loadTexture("background1.png", renderer);
    wallTexture = loadTexture("wall.png", renderer);
    bulletTexture = loadTexture("bullet.png", renderer);
    playerTexture = loadTexture("player.png", renderer);
    enemyTexture = loadTexture("enemy.png", renderer);

    // Tạo tường, player và enemy
    generateWalls();
    player = new PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, playerTexture, bulletTexture);
    spawnEnemies();
}


    void render() {
        if (backgroundTexture) SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        for (auto& wall : walls) wall.render(renderer);
        player->render(renderer);
        for (auto& enemy : enemies) enemy.render(renderer);
        renderScore(renderer, score, font);
        SDL_RenderPresent(renderer);
    }

    void run() {
        while (running) {
            handleEvents();
            update();
            render();
            SDL_Delay(16);
        }
    }
    void renderScore(SDL_Renderer* renderer, int score, TTF_Font* font) {
    // Tạo một bề mặt chứa văn bản (score)
    SDL_Color textColor = {255, 255, 255, 255}; // Màu trắng cho văn bản
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), textColor);
    if (!textSurface) {
        cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
        return;
    }

    // Chuyển bề mặt thành texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);  // Hủy bề mặt sau khi đã tạo texture

    // Tạo vị trí và kích thước để vẽ text
    SDL_Rect renderQuad = {10, 10, textSurface->w, textSurface->h};

    // Vẽ text lên màn hình
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Giải phóng texture
    SDL_DestroyTexture(textTexture);
}


    ~Game() {
        delete player;
        Mix_HaltMusic();
        Mix_FreeMusic(bgMusic);
        Mix_CloseAudio();
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(wallTexture);
        SDL_DestroyTexture(bulletTexture);
        SDL_DestroyTexture(playerTexture);
        SDL_DestroyTexture(enemyTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        IMG_Quit();
        SDL_Quit();
    }

    void generateWalls()
    {
        for (int i = 1; i < MAP_HEIGHT - 1; i += 2)
        {
            for (int j = 1; j < MAP_WIDTH - 1; j += 2)
            {
                walls.emplace_back(j * TILE_SIZE , i * TILE_SIZE , wallTexture);
            }
        }
    }

    void handleEvents()
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if(event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP: player->move(0 , -10 , walls); break;
                    case SDLK_DOWN: player->move(0 , 10 , walls); break;
                    case SDLK_LEFT: player->move(-10 , 0 , walls); break;
                    case SDLK_RIGHT: player->move(10 , 0 , walls); break;
                    case SDLK_SPACE: player->shoot(); break;
                }
            }
        }
    }

    void update() {
        player->updateBullets();
        for (auto& enemy : enemies) {
            enemy.move(walls);
            enemy.updateBullets();
            if (rand() % 100 < 2) enemy.shoot();
        }

        for (auto& bullet : player->Bullets) {
            for (auto& wall : walls)
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    score += 100;
                    break;
                }
            for (auto& enemy : enemies)
                if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                    enemy.active = false;
                    bullet.active = false;
                    score += 200;
                    break;
                }
        }

        enemies.erase(remove_if(enemies.begin(), enemies.end(), [](EnemyTank& e) { return !e.active; }), enemies.end());
        if (enemies.empty()) running = false;

        for (auto& enemy : enemies)
            for (auto& bullet : enemy.Bullets)
                if (SDL_HasIntersection(&bullet.rect, &player->rect)) {
                    running = false;
                    return;
                }
        if (score > highScore) {
            highScore = score;
            ofstream outfile("highscore.txt");
            if (outfile.is_open()) {
                outfile << highScore;
                outfile.close();
            }
        }
    }
    void spawnEnemies()
    {
        enemies.clear();
        for (int i = 0; i < enemyNumber; ++i)
        {
            int ex, ey;
            bool valid = false;
            while (!valid)
            {
                ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
                ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
                SDL_Rect tempRect = {ex, ey, TILE_SIZE, TILE_SIZE};

                valid = true;
                for (const auto& wall : walls)
                {
                    if (wall.active && SDL_HasIntersection(&tempRect, &wall.rect))
                    {
                        valid = false;
                        break;
                    }
                }

                if (valid && SDL_HasIntersection(&tempRect, &player->rect))
                {
                    valid = false;
                }

                if (valid)
                {
                    for (const auto& enemy : enemies)
                    {
                        if (SDL_HasIntersection(&tempRect, &enemy.rect))
                        {
                            valid = false;
                            break;
                        }
                    }
                }
            }

            enemies.emplace_back(ex, ey, enemyTexture, bulletTexture, walls);
        }
    }

};

int main(int argc, char* argv[]) {
    Game game;
    if (!game.running) return 1;
    Menu menu(game.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!menu.show()) {
        return 0;
    }

    game.run();
    return 0;
}
