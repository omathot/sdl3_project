#include <SDL3/SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include "assetManager.h"
#include "texture.h"

TEST_CASE("AssetManager init default", "[assetM]") {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_CreateWindowAndRenderer("test", 800, 800, SDL_WINDOW_HIDDEN, &window, &renderer);
  AssetManager *manager = new AssetManager();
  std::shared_ptr<Texture> texture = manager->loadTexture("test", "honey.png", renderer);
  REQUIRE(texture.get()->isLoaded());
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
}

TEST_CASE("AssetManager init custom path", "[assetM]") {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_CreateWindowAndRenderer("test", 800, 800, SDL_WINDOW_HIDDEN, &window, &renderer);
  AssetManager *manager = new AssetManager("tests/");
  std::shared_ptr<Texture> texture = manager->loadTexture("test", "honey.png", renderer);
  REQUIRE(texture.get()->isLoaded());
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
}

TEST_CASE("AssetManager delete texture", "[assetM]") {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_CreateWindowAndRenderer("test", 800, 800, SDL_WINDOW_HIDDEN, &window, &renderer);
  AssetManager *manager = new AssetManager();
  std::shared_ptr<Texture> texture = manager->loadTexture("test", "honey.png", renderer);
  REQUIRE(2 == texture.use_count());  
  manager->removeTexture("test");
  REQUIRE(1 == texture.use_count());  
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
}
