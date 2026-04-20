#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return -1;
  }
  const int WINDOW_WIDTH = 800;
  const int WINDOW_HEIGHT = 600;
  SDL_Window *win =
      SDL_CreateWindow("Dino.cpp", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!win) {
    SDL_Quit();
    return -1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, NULL);
  if (!ren) {
    SDL_Log("CreateRenderer failed: %s", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return -1;
  }
  SDL_Log("Using renderer: %s", SDL_GetRendererName(ren));

  float size = 100;

  struct Player {
    float x, y;
  };
  Player player = {WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f};

  struct Obstacle {
    float x, y, size;
  };
  Obstacle* obstacles[3] = {};

  SDL_Event e;
  bool quit = false;

  int lastTime = SDL_GetTicks();
  bool is_jumping = false;

  int frame_counter = 0;
  while (!quit) {
    int currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.f;
    lastTime = currentTime;

    frame_counter++;

    if (frame_counter/1000 % 60 == 0) {
      is_jumping = false;
    }
    if (is_jumping) {
      frame_counter = 0;
      player.y -= 100 * deltaTime;
    } else {
      if (player.y < WINDOW_HEIGHT/2.f) {
        player.y += 100 * deltaTime;
      }
    }

    while (SDL_PollEvent(&e) != 0) {
      // Check the type of event
      switch (e.type) {
      case SDL_EVENT_QUIT:
        // Handle window close event (e.g., clicking the 'X' button)
        quit = true;
        break;
      case SDL_EVENT_KEY_DOWN:
        if (e.key.key == SDLK_ESCAPE) {
          quit = true;
          break;
        }
        if (e.key.key == SDLK_SPACE) {
            is_jumping = true;
        }
      }
    }

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    SDL_FRect rect = {player.x, player.y, size, size};
    SDL_RenderFillRect(ren, &rect);
    SDL_RenderPresent(ren);
  }

  return 0;
}
