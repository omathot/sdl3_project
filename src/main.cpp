#include <iostream>
#include <SDL3/SDL.h>
#include <flecs.h>

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  flecs::world ecs;
  
  std::cout << "Hello World!" << std::endl;
  return 0;
  
}
