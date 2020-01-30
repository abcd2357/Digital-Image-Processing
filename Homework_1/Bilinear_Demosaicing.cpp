// EE569 Homework Assignment #1
// Problem 1 (a) Bilinear Demosaicing
// Name: XU Kangyan
// Date: Jan 28, 2020
// Even Pattern

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int Width;
    int Height;

    // Check for proper syntax
    if (argc < 4){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "input_image.raw [Width] [Height]" << endl;
        return 0;
    }

    // size
    Width = atoi(argv[2]);
    Height = atoi(argv[3]);

    // Allocate image data array
    unsigned char Imagedata[Height][Width];
    unsigned char Outdata[Height][Width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Array Index starts from 0
    // First Extend Image Boundary
    unsigned char Image_Extend[Height+2][Width+2];
    int i,j;

    // Four Corner
    Image_Extend[0][0] = Imagedata[1][1];
    Image_Extend[0][Width + 1] = Imagedata[1][Width - 2];
    Image_Extend[Height + 1][0] = Imagedata[Height - 2][1];
    Image_Extend[Height + 1][Width + 1] = Imagedata[Height - 2][Width - 2];

    // Edge Pixel
    // Edge Left
    for (i=1;i<Height+1;i++) {
        Image_Extend[i][0] = Imagedata[i-1][1];
    }
    // Edge Up
    for (i=1;i<Width+1;i++) {
        Image_Extend[0][i] = Imagedata[1][i-1];
    }
    // Edge Right
    for (i=1;i<Height+1;i++) {
        Image_Extend[i][Width+1] = Imagedata[i-1][Width-2];
    }
    // Edge Down
    for (i=1;i<Width+1;i++) {
        Image_Extend[Height+1][i] = Imagedata[Height-2][i-1];
    }

    // Copy Imagedata To Image_Extend
    for (i=1;i<Height+1;i++) {
        for(j=1;j<Width+1;j++) {
            Image_Extend[i][j] = Imagedata[i-1][j-1];
        }
    }

    /* ------------------------------------------------------------------------------- */

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {

            // Red Pixel
            if ((i%2==0)&&(j%2==1)){
                Outdata[i][j][0] = Image_Extend[i+1][j+1]; // R
                Outdata[i][j][1] = 0.25*(Image_Extend[i+1][j]+Image_Extend[i+1][j+2]+Image_Extend[i][j+1]+Image_Extend[i+2][j+1]); // G
                Outdata[i][j][2] = 0.25*(Image_Extend[i][j]+Image_Extend[i][j+2]+Image_Extend[i+2][j]+Image_Extend[i+2][j+2]); // B
            }

            // Blue Pixel
            if ((i%2==1)&&(j%2==0)){
                Outdata[i][j][0] = 0.25*(Image_Extend[i][j]+Image_Extend[i][j+2]+Image_Extend[i+2][j]+Image_Extend[i+2][j+2]); // R
                Outdata[i][j][1] = 0.25*(Image_Extend[i+1][j]+Image_Extend[i+1][j+2]+Image_Extend[i][j+1]+Image_Extend[i+2][j+1]); // G
                Outdata[i][j][2] = Image_Extend[i+1][j+1]; // B
            }

            // Type 1 Green Pixel B Row R Column
            if ((i%2==1)&&(j%2==1)) {
                Outdata[i][j][0] = 0.5*(Image_Extend[i][j+1]+Image_Extend[i+2][j+1]); // R
                Outdata[i][j][1] = Image_Extend[i+1][j+1]; // G
                Outdata[i][j][2] = 0.5*(Image_Extend[i+1][j]+Image_Extend[i+1][j+2]); // B
            }

            // Type 2 Green Pixel R Row B Column
            if ((i%2==0)&&(j%2==0)) {
                Outdata[i][j][0] = 0.5*(Image_Extend[i+1][j]+Image_Extend[i+1][j+2]); // R
                Outdata[i][j][1] = Image_Extend[i+1][j+1]; // G
                Outdata[i][j][2] = 0.5*(Image_Extend[i][j+1]+Image_Extend[i+2][j+1]); // B
            }

        }
    }

    /* ------------------------------------------------------------------------------- */

    // Write image data from image data matrix
    if (!(file = fopen("Bilinear_Result.raw","wb"))) {
        cout << "Cannot write file: " << "Bilinear_Result.raw" << endl;
        exit(1);
    }
    fwrite(Outdata, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    return 0;
}