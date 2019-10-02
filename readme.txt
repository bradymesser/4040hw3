alphamask:
  - To run, compile the program with `make alpha` then run:
    ./alphamask input.img output.img
    (ex: ./alphamask hand.jpg out.png)
  - the thresholds.txt file contains 4 doubles, each on a different line
    line 1 is hue value (the desired color), line 2 is the threshold for hue
    (range of acceptible values), line 3 is minimum desired saturation, and line 4 is
    minimum desired value, change these values to change the screening behavior of the
    program (blue screen green screen etc...) The recommended thresholds for green
    screening are the following:
120.0
50.0
0.3
0.8
    The program will break if you put anything other than numbers in this file,
    please don't do that.

compose:
  - To run, compile the program with `make compose` then run:
    ./compose A.img B.img (ex: ./compose hand.jpg dhouse.png)
    ----- OR -----
    ./compose A.img B.img output.img (ex: ./compose hand.jpg dhouse.png out.png)
  - This program takes in 2 or 3 command line arguments after the program name.
    The first argument will be image A and the second will be image B with the
    optional third argument being the name of an output file if you would like
    the resulting image to be saved to a file.  This program performs A over B
    to create a composite image.
  - If image A is not a 4 channel image, you will be asked if you would like it
    converted to a 4 channel image and chromakeyed for you (this is what alphamask
    does, but alphamask is not needed for this to work in compose)
  - Image B will be converted to a 4 channel image if it is not one already
  - Currently, the program does not work well if A has a larger dimension than B,
    a warning is displayed whenever this is detected but the program will still run.
  - The composited image will be displayed in a window, with the following keyboard
    functions:
      - q or Q: close the window / quit the program
      - w or W: write the displayed image to a file
