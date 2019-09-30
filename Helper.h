/*
Brady Messer (wmesser@clemson.edu)
CPSC 4040 HW1 Fall 2019
This file contains the Image object and some helper functions to read and write images
*/

#include <GL/glut.h>
#include <iostream>
#include <OpenImageIO/imageio.h>
#include <string>
using namespace std;
OIIO_NAMESPACE_USING;

// The struct that represents an image, can be converted to a class later and
// the read and write image functions can be encapsulated within the class
class Image {
  private:
  public:
    int width;
    int height;
    int channels;
    unsigned char * pixels;
    string ext;

    //default constructor
    Image() {
      //Nothing to do without a specified file, set all values to defaults
      width = 0;
      height = 0;
      channels = 0;
      pixels = NULL;
      ext = "";
    }

    // overloaded constructor, reads in an image
    Image(string file) {
      ImageInput *in = ImageInput::open(file);
      if (!in) {
          cout << "Could not open file " << file << " exiting. " << endl;
          exit(1);
      }
      const ImageSpec &spec = in->spec();
      width = spec.width;
      height = spec.height;
      channels = spec.nchannels;
      ext = file.substr(file.find("."), file.length());
      pixels = new unsigned char[width*height*channels];
      in->read_image (TypeDesc::UINT8, &pixels[0]);
      in->close ();
      ImageInput::destroy (in);
    }

    //writes the object to a file
    void writeImage(string filename) {
      const int xres = this->width, yres = this->height;
      const int channels = this->channels;  // RGB
      ImageOutput *out = ImageOutput::create (filename);
      if (!out) {
          cout << "EXITING" << endl;
          exit(1);
      }
      ImageSpec spec (xres, yres, channels, TypeDesc::UINT8);
      out->open (filename, spec);
      out->write_image (TypeDesc::UINT8, pixels);
      out->close ();
      ImageOutput::destroy (out);
    }

    void invert() {
      switch (channels) {
        case 1:
    			// set unpack alignment to 1 so that rows aren't skipped in the pixmap
    			for (int i = 0; i < width * height; i++) {
    				pixels[i] = 255 - pixels[i];
    			}
    			break;
        case 2:
    			// I'm not sure if 2 channels is a case but I implemented it anyways
          break;
        case 3:
    			for (int i = 0; i < width * height * channels; i++) {
    				pixels[i] = 255 - pixels[i];
    			}
          break;
        case 4:
    			for (int i = 0; i < width * height * channels; i++) {
    				pixels[i] = 255 - pixels[i];
    			}
          break;
        default:
          break;
      }
    }

    void draw() {
      switch (channels) {
        case 1:
    			// set unpack alignment to 1 so that rows aren't skipped in the pixmap
    			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    			glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
    			break;
        case 2:
    			// I'm not sure if 2 channels is a case but I implemented it anyways
          glDrawPixels(width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pixels);
          break;
        case 3:
          glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
          break;
        case 4:
          glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
          break;
        default:
          break;
      }
    }
};
