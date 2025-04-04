#include <kate_window.hpp>
#include <first_app.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>


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