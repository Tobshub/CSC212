#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return -1;
  }
  const int WINDOW_WIDTH = 800;
  const int WINDOW_HEIGHT = 600;
  SDL_Window *win =
      SDL_CreateWindow("Window.cpp", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

  float size = 10;

  const int MAP_WIDTH = 8;
  const int MAP_HEIGHT = 16;
  int map[MAP_WIDTH][MAP_HEIGHT] = {
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1},
      {1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
      {1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
      {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  };

  struct Player {
    float angle, x, y;
  };
  Player player = {1, 2, 0};

  SDL_Event e;
  bool quit = false;
  bool mode2d = true;
  while (!quit) {
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      Player ray = {
          .angle = (player.angle - .5f) + ((float)x / WINDOW_WIDTH),
          .x = (float)cos(ray.angle),
          .y = (float)sin(ray.angle),
      };
      float distance = 0.f;
      while (true) {
        int testX = (int)(player.x + ray.x * distance);
        int testY = (int)(player.y + ray.y * distance);

        if (testX < 0 || testY < 0 || testX >= MAP_WIDTH || testY >= MAP_HEIGHT)
          break;
        if (map[testY][testX] == 1)
          break;

        distance += 0.01f;
      }

      int wallHeight = (WINDOW_HEIGHT / (distance + 0.0001f));
      SDL_RenderLine(ren, x, WINDOW_HEIGHT / 2.f - wallHeight / 2.f, x,
                     WINDOW_HEIGHT / 2.f + wallHeight / 2.f);
    }

    if (mode2d) {
      for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
          if (map[i][j] == 1) {
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_FRect r = {j * size, i * size, size, size};
            SDL_RenderFillRect(ren, &r);
          } else {
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_FRect r = {j * size, i * size, size, size};
            SDL_RenderFillRect(ren, &r);
          }
        }
      }
    }

    SDL_RenderPresent(ren);

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
          mode2d = !mode2d;
        }
        const float speed = 0.05f;

        if (e.key.key == SDLK_W) {
          player.x += cos(player.angle) * speed;
          player.y += sin(player.angle) * speed;
        }
        if (e.key.key == SDLK_S) {
          player.x -= cos(player.angle) * speed;
          player.y -= sin(player.angle) * speed;
        }
        if (e.key.key == SDLK_A)
          player.angle -= 0.05f;
        if (e.key.key == SDLK_D)
          player.angle += 0.05f;
      }
    }
  }

  return 0;
}
