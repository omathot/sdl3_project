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
  manager->setBasePath("tests/");
  std::shared_ptr<Texture> texture = manager->loadTexture("test", "honey.png");
  REQUIRE(texture->isLoaded());
  spdlog::debug("path in custom: {}", texture->getPath());
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

TEST_CASE("AssetManager load animation", "[assetM]") {
  App *app = new App();
  app->init();
  AssetManager *manager = app->getAssetManager();
  manager->setBasePath("tests");
  std::shared_ptr<Animation> animation = manager->loadAnimation("test", "jump.png", 48, 48, 3, 0.2);
  REQUIRE(animation->isLoaded());
}
