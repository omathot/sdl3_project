#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include "app.h"

void spdlog_ref() {
  spdlog::info("Info!");
  spdlog::warn("Warn!");
  spdlog::warn("Easy padding in numbers like {:08d}", 12);
  spdlog::info("{:<30}", "left aligned");
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("Debug");
  // change log pattern
  spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
  spdlog::debug("Changed pattern");

  SPDLOG_TRACE("Some trace message with param {}", 42);
  SPDLOG_DEBUG("Some debug message");
}

void set_logging_level(char *str) {
  std::string level = str;
  if (level == "debug") {
    spdlog::set_level(spdlog::level::debug);
  } else if (level == "info") {
    spdlog::set_level(spdlog::level::info);
  } else if (level == "warn") {
    spdlog::set_level(spdlog::level::warn);
  } else if (level == "error") {
    spdlog::set_level(spdlog::level::err);
  }
}

int main(int argc, char **argv) {
  if (argc > 0) {
    set_logging_level(argv[1]);
  }
  App *app = new App();

  if (!app->init()) {
    spdlog::error("Received error, exitting");
    return 1;
  }
  app->run();
  spdlog::info("Finished app run");
  delete(app);

  return 0;
}
