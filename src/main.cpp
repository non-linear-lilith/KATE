#include "kate_window.hpp"
#include "first_app.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#ifndef ENGINE_DIR
#define ENGINE_DIR "../include"
#endif

int main() {
  kate::FirstApp app{};

  try {
    app.run();
  } catch(const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}