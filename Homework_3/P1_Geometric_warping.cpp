// EE569 Homework Assignment #3
// Problem 1 (a) Geometric warping
// Name: XU Kangyan
// ID: 4876010998
// Email: kangyanx@usc.edu
// Date: Mar 3, 2020

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int Width, Height;

    // Check for proper syntax
    if (argc < 4) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "input_image.raw [Width] [Height]" << endl;
        return 0;
    }

    // size
    Width = atoi(argv[2]);
    Height = atoi(argv[3]);

    // Allocate image data array
    unsigned char ImageInput[Height][Width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(ImageInput, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    int i,j,k;
    unsigned char ImageWrapped[Height][Width][3];
    double x_mid = (Width-1)/2.0;
    double y_mid = (Height-1)/2.0;
    int y_new, y_0;

    /*
    cout << "Input (255,255): " << (int)ImageInput[255][255][0] <<" "<< (int)ImageInput[255][255][1] <<" "<< (int)ImageInput[255][255][2] << endl;
    cout << "Input (255,256): " << (int)ImageInput[255][256][0] <<" "<< (int)ImageInput[255][256][1] <<" "<< (int)ImageInput[255][256][2] << endl;
    cout << "Input (256,255): " << (int)ImageInput[256][255][0] <<" "<< (int)ImageInput[256][255][1] <<" "<< (int)ImageInput[256][255][2] << endl;
    cout << "Input (256,256): " << (int)ImageInput[256][256][0] <<" "<< (int)ImageInput[256][256][1] <<" "<< (int)ImageInput[256][256][2] << endl;
    */

    for (i=0;i<Width;i++) {
        for (j=0;j<Height;j++) {
            for (k=0;k<3;k++) {
                ImageWrapped[j][i][k] = 0; // black background
            }
        }
    }

    // VERTICAL squeezing
    // int h_new = h_old * sqrt(256^2-(x-255)^2)/256.0 + 0.5

    for (i=0;i<Width;i++) {
        for (j = 0; j < Height; j++) {
            y_new = y_mid - (y_mid - j) * sqrt(256 * 256 - (i - x_mid) * (i - x_mid)) / 256.0 + 0.5;
            for (k = 0; k < 3; k++) {
                ImageWrapped[y_new][i][k] = ImageInput[j][i][k];
            }
        }
        // Up Boundary Condition
        y_0 = y_mid - (y_mid - 0) * sqrt(256 * 256 - (i - x_mid) * (i - x_mid)) / 256.0 + 0.5;
        for (k = 0; k < 3; k++) {
            ImageWrapped[y_0][i][k] = ImageInput[0][i][k];
        }
    }

    /* ------------------------------------------------------------------------------- */

    /*
    cout << "Otput (255,255): " << (int)ImageWrapped[255][255][0] <<" "<< (int)ImageWrapped[255][255][1] <<" "<< (int)ImageWrapped[255][255][2] << endl;
    cout << "Otput (255,256): " << (int)ImageWrapped[255][256][0] <<" "<< (int)ImageWrapped[255][256][1] <<" "<< (int)ImageWrapped[255][256][2] << endl;
    cout << "Otput (256,255): " << (int)ImageWrapped[256][255][0] <<" "<< (int)ImageWrapped[256][255][1] <<" "<< (int)ImageWrapped[256][255][2] << endl;
    cout << "Otput (256,256): " << (int)ImageWrapped[256][256][0] <<" "<< (int)ImageWrapped[256][256][1] <<" "<< (int)ImageWrapped[256][256][2] << endl;
    */

    // Output

    if (!(file = fopen("Warpped.raw","wb"))) {
        cout << "Cannot write file: " << "Warpped.raw" << endl;
        exit(1);
    }
    fwrite(ImageWrapped, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */
    /* REVERSE */

    /*
    unsigned char ImageReverse[Height][Width][3];
    int y_old;

    for (i=0;i<Width;i++) {
        for (j=0;j<Height;j++) {
            if( (int)ImageWrapped[j][i][0]+(int)ImageWrapped[j][i][1]+(int)ImageWrapped[j][i][2]>0 ) {
                y_old = y_mid - (y_mid - j) * 256.0/sqrt(256*256-(i-x_mid)*(i-x_mid)) + 0.5;
                for (k=0;k<3;k++) {
                    ImageReverse[y_old][i][k] = ImageWrapped[j][i][k];
                }
            }
        }
    }

    // Output

    if (!(file = fopen("Reverse.raw","wb"))) {
        cout << "Cannot write file: " << "Reverse.raw" << endl;
        exit(1);
    }
    fwrite(ImageReverse, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);
    */

    /* ------------------------------------------------------------------------------- */
    /* Bilinear Interpolation */
    int pixel_num;
    double dist_y; // Equally divide color pixel interval
    int neighbor_y;
    double delta;
    unsigned char ImageRecovered[Height][Width][3];

    for (i=0;i<Width;i++) {
        pixel_num = sqrt(256*256 - (255.5-i)*(255.5-i))+0.5; // From 256-pixel_num to 255+pixel_num, (2*pixel_num-1)/511
        dist_y = (2*pixel_num-1)/511.0;
        for (j=0;j<Height;j++) {
            neighbor_y = 256 - pixel_num + dist_y * j;
            delta = 256 - pixel_num + dist_y*j - neighbor_y;
            for (k=0;k<3;k++){
                ImageRecovered[j][i][k] = (1-delta) * ImageWrapped[neighbor_y][i][k] + delta * ImageWrapped[1+neighbor_y][i][k];
            }
        }
    }

    // Output

    if (!(file = fopen("Recover.raw","wb"))) {
        cout << "Cannot write file: " << "Recover.raw" << endl;
        exit(1);
    }
    fwrite(ImageRecovered, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);


    return 0;
}