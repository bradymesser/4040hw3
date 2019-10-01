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
  B.composite(A);
  image = B;
  if (argv == 4) {
    image.write(argv[3]);
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
