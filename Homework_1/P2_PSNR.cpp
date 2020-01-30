// EE569 Homework Assignment #1
// Problem 2 PSNR
// Name: XU Kangyan
// Date: Jan 28, 2020

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

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
        cout << "input_image.raw original_image.raw [Width] [Height]" << endl;
        return 0;
    }

    // size
    Width = atoi(argv[3]);
    Height = atoi(argv[4]);

    // Allocate image data array
    unsigned char Imagedata_1[Height][Width];
    unsigned char Imagedata_2[Height][Width];

    // Read input_image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata_1, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    // Read original_image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[2],"rb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fread(Imagedata_2, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    int i,j;
    int Sum = 0;
    // Array Index starts from 0
    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Sum +=  (Imagedata_2[i][j] - Imagedata_1[i][j])*(Imagedata_2[i][j] - Imagedata_1[i][j]);
        }
    }
    double MSE;
    MSE = Sum / (Height*Width);
    double PSNR;
    PSNR = 10 * log10(255*255/MSE);
    cout << "PSNR = " << PSNR << endl;

    return 0;
}