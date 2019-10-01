/*
Brady Messer (wmesser@clemson.edu)
CPSC 4040 HW1 Fall 2019
This file contains the Image object and some helper functions to read and write images
*/

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <OpenImageIO/imageio.h>
#include <string>
#include <math.h>
#include "rgbToHsv.cpp"
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
      // ImageOutput::destroy (out);
      ImageOutput::destroy(out);
    }

    // Inverts the colors of the image, the switch isn't necessary and can be replaced with a single for loop
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

    // This should display the image on screen but I have not tested it yet
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

    // Converts to a 4 channel image
    void convertToFourChannels() {
      if (channels == 4) {
        printf("Image already 4 channels, returning.\n");
        return;
      }
      double h,s,v;
      unsigned char * pixels2 = new unsigned char[width*height*4];
      int j = 0;
      for (int i = 0; i < (width * height * channels) - channels; i+=channels) {
        RGBtoHSV(pixels[i],pixels[i+1],pixels[i+2],h,s,v);
        pixels2[j] = pixels[i];
        pixels2[j+1] = pixels[i+1];
        pixels2[j+2] = pixels[i+2];
        pixels2[j+3] = 255;
        j += 4;
      }
      pixels = pixels2;
      channels = 4;
    }

    // Chromakey the image based on hsv values and the thresholds defined in thresholds.txt
    void chromaKey() {
      if (channels != 4) {
        printf("Can't chromakey without 4 channels\n");
        return;
      }
      ifstream in;
      in.open("thresholds.txt");
      double hT = 40.0;
      double sT = 40.0;
      double vT = 40.0;  // the threshold values for h s and v
      in >> hT >> sT >> vT;
      double h,s,v;

      for (int i = 0; i < (width * height * channels) - channels; i+=channels) {
        RGBtoHSV(pixels[i],pixels[i+1],pixels[i+2],h,s,v);
        if (abs(h - 120) <= hT && (abs(1.0 - s) <= sT || abs(1.0 - v) <= vT)) {
         pixels[i+3] = 0;
        } else {
         pixels[i+3] = 255;
       }
      }
    }
};
