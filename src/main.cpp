#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_video.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <flecs.h>

int main(int argc, char **argv) {
  SDL_Log("Starting init");
  int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (result < 0) {
    SDL_Log("SDL_Init() error: %s\n", SDL_GetError());
  }

  SDL_Window *window = SDL_CreateWindow("Hello world", 800, 600, SDL_WINDOW_RESIZABLE);
  if (window == nullptr) {
    SDL_Log("Failed to create window: %s\n", SDL_GetError());
  }

  SDL_Event event;
  while (true) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          return 0 ;
        default: break ;
      }
    }
  }
  flecs::world ecs;
  
  std::cout << "Hello World!" << std::endl;

  SDL_DestroyWindow(window);
  return 0;
  
}
