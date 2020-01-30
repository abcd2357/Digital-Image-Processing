// EE569 Homework Assignment #1
// Problem 1
// Name: XU Kangyan
// Date: Jan 28, 2020

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<cmath>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int Width;
    int Height;

    // Check for proper syntax
    if (argc < 5){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "image_1.raw image_2.raw [Width] [Height]" << endl;
        return 0;
    }

    // size
    Width = atoi(argv[3]);
    Height = atoi(argv[4]);

    // Allocate image data array
    unsigned char Image_1[Height][Width][3];
    unsigned char Image_2[Height][Width][3];
    unsigned char Sub_Image[Height][Width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Image_1, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    // Read image (filename specified by second argument) into image data matrix
    if (!(file = fopen(argv[2],"rb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fread(Image_2, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    int i,j;

    // Array Index starts from 0
    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Sub_Image[i][j][0] = 255-15*abs(Image_1[i][j][0] - Image_2[i][j][0]); // To Enlarge Difference
            Sub_Image[i][j][1] = 255-15*abs(Image_1[i][j][1] - Image_2[i][j][1]);
            Sub_Image[i][j][2] = 255-15*abs(Image_1[i][j][2] - Image_2[i][j][2]);
        }
    }

    /* ------------------------------------------------------------------------------- */

    // Write image data (filename specified by second argument) from image data matrix
    if (!(file = fopen("Sub_Image.raw","wb"))) {
        cout << "Cannot write file: " << "Sub_Image.raw" << endl;
        exit(1);
    }
    fwrite(Sub_Image, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    return 0;
}