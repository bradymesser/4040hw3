#include "Helper.h"

int main(int argc, char * argv[]) {
  if (argc < 3 || argc > 4) {
    cout << "Invalid usage. Correct usages are:\n./compose A.img B.img\n./compose A.img B.img out.img\n";
    exit(1);
  }
  Image A = Image(argv[1]);
  Image B = Image(argv[2]);
  return 1;
}
