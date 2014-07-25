#include "lumina/lumina.hpp"
#include "MandelSet.hpp"

#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  if(argc != 2) {
    cout << "Usage:" << endl
         << "\t ./test N" << endl << endl
         << "Choose N to start a specific game/program:" << endl
         << "  1: MandelbrotSet" << endl;
    return 0;
  }

  switch(argv[1][0]) {
    case '1':
      {
        MandelSet mandel;
        mandel.init();
        mandel.start();
        break;
      }
    case '2':
      {
        break;
      }
    default:
      cout << "Invalid number!" << endl;
  }  
}