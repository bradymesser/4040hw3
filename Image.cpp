// //default constructor
// Image() {
//   //Nothing to do without a specified file, set all values to defaults
//   width = 0;
//   height = 0;
//   channels = 0;
//   pixels = NULL;
//   ext = "";
// }
//
// // overloaded constructor, reads in an image
// Image(string file) {
//   ImageInput *in = ImageInput::open(file);
//   if (!in) {
//       cout << "Could not open file " << file << " exiting. " << endl;
//       exit(1);
//   }
//   const ImageSpec &spec = in->spec();
//   width = spec.width;
//   height = spec.height;
//   channels = spec.nchannels;
//   ext = file.substr(file.find("."), file.length());
//   pixels = new unsigned char[width*height*channels];
//   in->read_image (TypeDesc::UINT8, &pixels[0]);
//   in->close ();
//   ImageInput::destroy (in);
// }
//
// //writes the object to a file
// void writeImage(string filename) {
//   const int xres = this->width, yres = this->height;
//   const int channels = this->channels;  // RGB
//   ImageOutput *out = ImageOutput::create (filename);
//   if (!out) {
//       cout << "EXITING, could not open output file" << endl;
//       exit(1);
//   }
//   ImageSpec spec (xres, yres, channels, TypeDesc::UINT8);
//   out->open (filename, spec);
//   out->write_image (TypeDesc::UINT8, pixels);
//   out->close ();
//   // ImageOutput::destroy (out);
//   ImageOutput::destroy(out);
// }
//
// // Inverts the colors of the image, the switch isn't necessary and can be replaced with a single for loop
// void invert() {
//   switch (channels) {
//     case 1:
//       // set unpack alignment to 1 so that rows aren't skipped in the pixmap
//       for (int i = 0; i < width * height; i++) {
//         pixels[i] = 255 - pixels[i];
//       }
//       break;
//     case 2:
//       // I'm not sure if 2 channels is a case but I implemented it anyways
//       break;
//     case 3:
//       for (int i = 0; i < width * height * channels; i++) {
//         pixels[i] = 255 - pixels[i];
//       }
//       break;
//     case 4:
//       for (int i = 0; i < width * height * channels; i++) {
//         pixels[i] = 255 - pixels[i];
//       }
//       break;
//     default:
//       break;
//   }
// }
//
// // This just draws the pixels, it cannot be used as the callback function for drawing
// void draw() {
//   switch (channels) {
//     case 1:
//       // set unpack alignment to 1 so that rows aren't skipped in the pixmap
//       glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//       glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
//       break;
//     case 2:
//       // I'm not sure if 2 channels is a case but I implemented it anyways
//       glDrawPixels(width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pixels);
//       break;
//     case 3:
//       glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
//       break;
//     case 4:
//       glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//       break;
//     default:
//       break;
//   }
// }
//
// // Converts to a 4 channel image
// void convertToFourChannels() {
//   if (channels == 4) {
//     return;
//   }
//   double h,s,v;
//   unsigned char * pixels2 = new unsigned char[width*height*4];
//   int j = 0;
//   for (int i = 0; i < (width * height * channels) - channels; i+=channels) {
//     RGBtoHSV(pixels[i],pixels[i+1],pixels[i+2],h,s,v);
//     pixels2[j] = pixels[i];
//     pixels2[j+1] = pixels[i+1];
//     pixels2[j+2] = pixels[i+2];
//     pixels2[j+3] = 255;
//     j += 4;
//   }
//   pixels = pixels2;
//   channels = 4;
// }
//
// // Chromakey the image based on hsv values and the thresholds defined in thresholds.txt
// void chromaKey() {
//   if (channels != 4) {
//     printf("Can't chromakey without 4 channels, would you like the image to be converted to 4 channels for you? (y/n)\n");
//     char temp;
//     cin >> temp;
//     if (temp != 'y' && temp != 'Y') {
//       return;
//     }
//     // if the user agreed, convert the image to 4 channels and continue execution
//     convertToFourChannels();
//   }
//   ifstream in;
//   in.open("thresholds.txt");
//   // the threshold values for h s and v
//   double hV = 120.0; //The desired color, 120 hue is green
//   double hT = 50.0; //threshold (hue's within the range of hT are valid)
//   double sT = 0.3; // minimum desired saturation
//   double vT = 0.8; // minimum desired value
//   if (in != NULL) {
//     // If there is a thresholds.txt file, read in the values and use them
//     in >> hV >> hT >> sT >> vT;
//   } else {
//     cout << "Could not find thresholds.txt, using default values.\n";
//   }
//   double h,s,v;
//
//   for (int i = 0; i < (width * height * channels) - channels; i+=channels) {
//     RGBtoHSV(pixels[i],pixels[i+1],pixels[i+2],h,s,v);
//     if (abs(h - hV) <= hT && (s >= sT || v >= vT)) {
//      pixels[i+3] = 0; //TODO: Make alpha be other values than 0 and 255 (non binary)
//     } else {
//      pixels[i+3] = 255;
//    }
//   }
// }
//
// // Uses the current image object as the background (B), and overlays the parameter image (A)
// // The result is an image with A composited onto B (A over B)
// void composite(Image A) {
//   if (A.channels != 4) {
//     cout << "Can't perform A over B if A isn't 4 channels.  \nWould you like the image to be converted to 4 channels and chromakeyed for you? (y/n)\n";
//     char temp;
//     cin >> temp;
//     if (temp != 'y' && temp != 'Y') {
//       return;
//     }
//     A.convertToFourChannels();
//     A.chromaKey();
//   }
//
//   double r, g, b, a;
//   double newR, newG, newB, newA;
//   int j = 0;
//   int currRow = 0;
//   int w = A.width * A.channels;
//   for (int i = 0; i < A.width * A.height * A.channels; i+=A.channels) {
//     if (i > w + (w * currRow)) {
//       currRow++;
//       j = (width * channels) * currRow;
//     }
//     // first premultiply
//     a = A.pixels[i+3] / 255;
//     // if the pixel is transparent on A, do nothing to B
//     // TODO: Once alpha is non binary (0 and 255) this will need to change
//     if (a == 0) {
//       j+= channels;
//       continue;
//     }
//     r = A.pixels[i] * a;
//     g = A.pixels[i+1] * a;
//     b = A.pixels[i+2] * a;
//     // Perform over operation
//     newR = r + (1-a) * pixels[i];
//     newG = g + (1-a) * pixels[i+1];
//     newB = b + (1-a) * pixels[i+2];
//     newA = a + (1-a) * pixels[i+3];
//     pixels[j] = newR;
//     pixels[j+1] = newG;
//     pixels[j+2] = newB;
//     pixels[j+3] = newA;
//     j+= channels;
//   }
// }
