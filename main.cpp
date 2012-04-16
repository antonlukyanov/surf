#include <iostream>
#include <fstream>
#include "bmp/bitmap.h"

using namespace std;

int main(int argc, char **argv) {
  Bitmap b("100-24.bmp");

  RGBTriple p;

  b.at(5, 5, p);
  cout << p.r << endl;
  cout << p.g << endl;
  cout << p.b << endl;

  return 0;
}