#include "Helper.h"

int main (int argc, char * argv[]) {
  Image img = Image();
  if (argc != 3) {
    printf("Incorrect usage.  Correct usage is ./alphamask input.img output.img\n");
    exit(1);
  }
  img = Image(argv[1]);
  img.convertToFourChannels();
  img.chromaKey();
  img.writeImage(argv[2]);
}
