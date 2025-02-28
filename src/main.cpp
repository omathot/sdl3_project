#include <iostream>
#include <SDL3/SDL.h>
#include <flecs.h>

struct Vec2 {
  int x;
  int y;
};

struct Enemy {};

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
  bool is_playing = true;
  while (is_playing) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          is_playing = false ;
        default: break ;
      }
    }
  }
  flecs::world ecs;
  auto e = ecs.entity("Bob").add<Enemy>();
  if (e.has<Enemy>()) {
    std::cout << "E is enemy tagged" << std::endl;
  }
  e.add<Vec2>();
  e.set<Vec2>({19, 91});
  e.remove<Vec2>();
  const Vec2 *vec = e.get<Vec2>();
  std::cout << vec->x << " " << vec->y << std::endl;
  std::cout << "Entity name: " << e.name() << std::endl;
  ecs.lookup("Bob");
  e.is_alive();
  e.destruct();
  if (!e.is_alive()) {
    SDL_Log("E is dead\n");
  }
  
  std::cout << "Hello World!" << std::endl;

  SDL_DestroyWindow(window);
  return 0;
}
