#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include "Wall.h"
#include "const.h"
#include "PlayerTank.h"
#include "Bullet.h"
#include "EnemyTank.h"

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

    vector<Wall> walls;

    PlayerTank* player;

    int enemyNumber = 10;
    vector<EnemyTank> enemies;
    Mix_Music* bgMusic;

    Game()
    {
        running = true;
        if (SDL_Init (SDL_INIT_VIDEO) < 0) {
            cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
            running = false;
        }

        window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (!window) {
            cerr << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
            running = false;
        }

        renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
            running = false;
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
    running = false;
    return;
}

Mix_Music* bgMusic = Mix_LoadMUS("background.mp3");
if (!bgMusic) {
    cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << endl;
    running = false;
    return;
}

Mix_PlayMusic(bgMusic, -1); // -1 = lặp vô hạn


        backgroundTexture = loadTexture("background1.png", renderer);
        wallTexture = loadTexture("wall.png", renderer);
        bulletTexture = loadTexture("bullet.png", renderer);
        playerTexture = loadTexture("player.png", renderer);
        enemyTexture = loadTexture("enemy.png", renderer);

        generateWalls();
        player = new PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, renderer, playerTexture, bulletTexture);
        spawnEnemies();
    }

    void render() {
        if (backgroundTexture) SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        for (auto& wall : walls) wall.render(renderer);
        player->render(renderer);
        for (auto& enemy : enemies) enemy.render(renderer);
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
                    break;
                }
            for (auto& enemy : enemies)
                if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                    enemy.active = false;
                    bullet.active = false;
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
    if (game.running) {
        game.run();
    }
    return 0;
}
