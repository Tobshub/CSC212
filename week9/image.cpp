#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Two Image", 800, 600, 0);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return 1;
    }

}
