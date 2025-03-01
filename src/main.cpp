#include <SDL3/SDL.h>
#include <flecs.h>
#include "app.h"
#include <iostream>

int main(int argc, char **argv) {
  App *app = new App();

  if (!app->init()) {
    std::cout << "Received error, exitting." << std::endl;
    return 1;
  }
  app->run();
  std::cout << "Finished app run" << std::endl;
  delete(app);

  return 0;
}
