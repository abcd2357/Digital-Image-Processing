// EE569 Homework Assignment #1
// Problem 1 (b) MHC Demosaicing
// Name: XU Kangyan
// Date: Jan 28, 2020
// Even Pattern

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

// To Avoid Overflow
float bound(float A) {
    if (A>255) {return 255;}
    if (A<0) {return 0;}
    return A; }

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
    fread(Imagedata, sizeof(unsigned char), Width * Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Array Index starts from 0
    // First Extend Image Boundary
    unsigned char Image_Extend[Height+4][Width+4];
    int i,j;

    // Copy Imagedata To Image_Extend
    for (i=2;i<Height+2;i++) {
        for (j=2;j<Width+2;j++) {
            Image_Extend[i][j] = Imagedata[i-2][j-2];
        }
    }

    // Left & Right Edge
    for (i=2;i<Height+2;i++) {
        Image_Extend[i][0] = Imagedata[i-2][2];
        Image_Extend[i][1] = Imagedata[i-2][1];
        Image_Extend[i][Width+3] = Imagedata[i-2][Width-3];
        Image_Extend[i][Width+2] = Imagedata[i-2][Width-2];
    }

    // Up & Down Edge
    for (j=2;j<Width+2;j++) {
        Image_Extend[0][j] = Imagedata[2][j-2];
        Image_Extend[1][j] = Imagedata[1][j-2];
        Image_Extend[Height+3][j] = Imagedata[Height-3][j-2];
        Image_Extend[Height+2][j] = Imagedata[Height-2][j-2];
    }

    // Four Corner
    for (i=0;i<2;i++) {
        // Left UP
        Image_Extend[i][0] = Image_Extend[i][4];
        Image_Extend[i][1] = Image_Extend[i][3];
        // Right UP
        Image_Extend[i][Width+3] = Image_Extend[i][Width-1];
        Image_Extend[i][Width+2] = Image_Extend[i][Width];
        // Left Down
        Image_Extend[Height+3][i] = Image_Extend[Height-1][i];
        Image_Extend[Height+2][i] = Image_Extend[Height][i];
        // Right Down
        Image_Extend[Height+3][Width+2+i] = Image_Extend[Height-1][Width-i];
        Image_Extend[Height+2][Width+2+i] = Image_Extend[Height][Width-i];
    }

    /* ------------------------------------------------------------------------------- */

    for (i=2;i<Height+2;i++) {
        for (j=2;j<Width+2;j++) {

            // Red Pixel
            if ((i % 2 == 0) && (j % 2 == 1)) {
                Outdata[i-2][j-2][0] = Image_Extend[i][j]; // R
                Outdata[i-2][j-2][1] = bound(0.125*(4 * Image_Extend[i][j]
                        +2*(Image_Extend[i][j + 1] + Image_Extend[i][j - 1] + Image_Extend[i + 1][j] + Image_Extend[i - 1][j])
                        -(Image_Extend[i][j + 2] + Image_Extend[i][j - 2] + Image_Extend[i + 2][j] + Image_Extend[i - 2][j]))); // G
                Outdata[i-2][j-2][2] = bound(0.125*(6 * Image_Extend[i][j]
                        +2*(Image_Extend[i - 1][j - 1] + Image_Extend[i - 1][j + 1] + Image_Extend[i + 1][j - 1] + Image_Extend[i + 1][j + 1])
                        -1.5*(Image_Extend[i - 2][j] + Image_Extend[i + 2][j] + Image_Extend[i][j - 2] + Image_Extend[i][j + 2]))); // B
            }

            // Blue Pixel
            if ((i%2==1)&&(j%2==0)){
                Outdata[i-2][j-2][0] = bound(0.125*(6 * Image_Extend[i][j]
                        +2*(Image_Extend[i - 1][j - 1] + Image_Extend[i - 1][j + 1] + Image_Extend[i + 1][j - 1] + Image_Extend[i + 1][j + 1])
                        -1.5*(Image_Extend[i - 2][j] + Image_Extend[i + 2][j] + Image_Extend[i][j - 2] + Image_Extend[i][j + 2]))); // R
                Outdata[i-2][j-2][1] = bound(0.125*(4 * Image_Extend[i][j]
                        +2*(Image_Extend[i][j + 1] + Image_Extend[i][j - 1] + Image_Extend[i + 1][j] + Image_Extend[i - 1][j])
                        -(Image_Extend[i][j + 2] + Image_Extend[i][j - 2] + Image_Extend[i + 2][j] + Image_Extend[i - 2][j]))); // G
                Outdata[i-2][j-2][2] = Image_Extend[i][j]; // B
            }

            // Type 1 Green Pixel (B Row, R column)
            if ((i%2==1)&&(j%2==1)) {
                Outdata[i-2][j-2][0] =  bound(0.125*(5 * Image_Extend[i][j]
                        +4*(Image_Extend[i - 1][j] + Image_Extend[i + 1][j])
                        -(Image_Extend[i - 1][j - 1] + Image_Extend[i - 1][j + 1] + Image_Extend[i + 1][j - 1] + Image_Extend[i + 1][j + 1] + Image_Extend[i - 2][j] + Image_Extend[i + 2][j])
                        +0.5*(Image_Extend[i][j - 2] + Image_Extend[i][j + 2]))); // R
                Outdata[i-2][j-2][1] = Image_Extend[i][j]; // G
                Outdata[i-2][j-2][2] = bound(0.125*(5 * Image_Extend[i][j]
                        +4*(Image_Extend[i][j - 1] + Image_Extend[i][j + 1])
                        -(Image_Extend[i - 1][j - 1] + Image_Extend[i - 1][j + 1] + Image_Extend[i + 1][j - 1] + Image_Extend[i + 1][j + 1] + Image_Extend[i][j - 2] + Image_Extend[i][j + 2])
                        +0.5*(Image_Extend[i - 2][j] + Image_Extend[i + 2][j]))); // B
            }

            // Type 2 Green Pixel (R Row, B column)
            if ((i%2==0)&&(j%2==0)) {
                Outdata[i-2][j-2][0] = bound(0.125*(5 * Image_Extend[i][j]
                        +4*(Image_Extend[i][j - 1] + Image_Extend[i][j + 1])
                        -(Image_Extend[i - 1][j - 1] + Image_Extend[i - 1][j + 1] + Image_Extend[i + 1][j - 1] + Image_Extend[i + 1][j + 1] + Image_Extend[i][j - 2] + Image_Extend[i][j + 2])
                        +0.5*(Image_Extend[i - 2][j] + Image_Extend[i + 2][j]))); // R
                Outdata[i-2][j-2][1] = Image_Extend[i][j]; // G
                Outdata[i-2][j-2][2] = bound(0.125*(5 * Image_Extend[i][j]
                        +4*(Image_Extend[i - 1][j] + Image_Extend[i + 1][j])
                        -(Image_Extend[i - 1][j - 1] + Image_Extend[i - 1][j + 1] + Image_Extend[i + 1][j - 1] + Image_Extend[i + 1][j + 1] + Image_Extend[i - 2][j] + Image_Extend[i + 2][j])
                        +0.5*(Image_Extend[i][j - 2] + Image_Extend[i][j + 2]))); // B
            }

        }
    }

    /* ------------------------------------------------------------------------------- */

    // Write image data from image data matrix
    if (!(file = fopen("MHC_Result.raw","wb"))) {
        cout << "Cannot write file: " << "MHC_Result.raw" << endl;
        exit(1);
    }
    fwrite(Outdata, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    return 0;
}