#include "Helper.h"
int WIDTH = 500;
int HEIGHT = 500;
// Image image = Image();

int main(int argc, char * argv[]) {
  if (argc < 3 || argc > 4) {
    cout << "Invalid usage. Correct usages are:\n./compose A.img B.img\n./compose A.img B.img out.img\n";
    exit(1);
  }
  Image A = Image(argv[1]);
  Image B = Image(argv[2]);
  if (A.width > B.width || A.height > B.height) {
    cout << "Warning, image A has a larger dimension than image B, the image may appear skewed.\n";
  }
  B.convertToFourChannels();  //Convert the background to a 4 channel image if it isn't already
  B.composite(A); // Composite will overlay A onto B
  image = B;  // The drawImage function relies on a global image named image
  if (argc == 4) {
    image.writeImage(argv[3]);
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("COMPOSE");

  glutDisplayFunc(drawImage);	  // display callback
  glutKeyboardFunc(handleKey);
  glutReshapeFunc(handleReshape); // window resize callback

  glutMainLoop();
  return 1;
}
