#include "lumina/lumina.hpp"
// using namespace lumina;

int main() {
  lumina::Window win("Test Game");
  win.setVersionHint(3, 3);
  win.open();
}