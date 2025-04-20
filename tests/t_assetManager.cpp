#include <SDL3/SDL.h>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include "assetManager.h"
#include "texture.h"
#include "app.h"

TEST_CASE("AssetManager init default", "[assetM]") {
  App *app = new App();
  app->init();
  AssetManager *manager = app->getAssetManager();
  std::shared_ptr<Texture> texture = manager->loadTexture("test", "honey.png");
  REQUIRE(texture.get()->isLoaded());
}

TEST_CASE("AssetManager init custom path", "[assetM]") {
  App *app = new App();
  app->init();
  AssetManager *manager = app->getAssetManager();
  std::shared_ptr<Texture> texture = manager->loadTexture("test", "honey.png");
  REQUIRE(texture.get()->isLoaded());
}

TEST_CASE("AssetManager delete texture", "[assetM]") {
  App *app = new App();
  app->init();
  AssetManager *manager = app->getAssetManager();
  std::shared_ptr<Texture> texture = manager->loadTexture("test", "honey.png");
  REQUIRE(2 == texture.use_count());  
  manager->removeTexture("test");
  REQUIRE(1 == texture.use_count());  
}
