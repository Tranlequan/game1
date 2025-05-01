#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Wall.h"
#include "const.h"
#include "PlayerTank.h"
#include "Bullet.h"

using namespace std;

SDL_Texture* loadTexture(const string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (!newTexture) {
        cerr << "Failed to load image " << path << "! SDL_image Error: "<< IMG_GetError() << endl;
    }
    return newTexture;
}

class Game {
public:
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    vector<Wall> walls;
    PlayerTank* player;
    SDL_Texture* wallTexture;
    SDL_Texture* playerTexture;

    Game() {
        running = true;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
            running = false;
            return;
        }

        window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
            running = false;
            return;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
            running = false;
            return;
        }

        wallTexture = loadTexture("wall.png", renderer);
        playerTexture = loadTexture("Player.png", renderer);

        if (!wallTexture || !playerTexture) {
            running = false;
            return;
        }

        player = new PlayerTank(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, playerTexture);
        generateWalls();
    }

    ~Game() {
        delete player;
        SDL_DestroyTexture(wallTexture);
        SDL_DestroyTexture(playerTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void generateWalls() {
        for (int i = 1; i < MAP_HEIGHT - 2; i += 3) {
            for (int j = 1; j < MAP_WIDTH - 2; j += 3) {
                walls.emplace_back(j * TILE_SIZE, i * TILE_SIZE, wallTexture);
            }
        }
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);

        for (const auto& wall : walls) {
            wall.render(renderer);
        }

        player->render(renderer);
        SDL_RenderPresent(renderer);
    }


void update () {
player -> updateBullets();
for (auto& bullet : player -> Bullets) {
for (auto& wall : walls) {
if
(wall.active && SDL_HasIntersection (&bullet.rect, &wall. rect)) {
wall. active = false;
bullet. active = false;
break;
}}}}
    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: player->move(0, -5, walls); break;
                    case SDLK_DOWN: player->move(0, 5, walls); break;
                    case SDLK_LEFT: player->move(-5, 0, walls); break;
                    case SDLK_RIGHT: player->move(5, 0, walls); break;
                    case SDLK_SPACE: player->shoot(); break ;
                }
            }
        }
    }
void run(){
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
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
