// EE569 Homework Assignment #2
// Problem 2 (c) Color Halftoning with Error Diffusion - Separable Error Diffusion
// Name: XU Kangyan
// ID: 4876010998
// Email: kangyanx@usc.edu
// Date: Feb 16, 2020

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

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
    unsigned char ImageInput[Height][Width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(ImageInput, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */
    // Floyd - Steinberg Error Diffusion
    int i,j,m,n;

    // Separate Image into CMY Three Channels. For clear, not using k=0:2 .
    unsigned char C[Height][Width]; // Cyan
    unsigned char M[Height][Width]; // Magenta
    unsigned char Y[Height][Width]; // Yellow

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            C[i][j] = 255 - ImageInput[i][j][0]; // 255-R
            M[i][j] = 255 - ImageInput[i][j][1]; // 255-G
            Y[i][j] = 255 - ImageInput[i][j][2]; // 255-B
        }
    }
    /* ---------------------------------- */

    unsigned char Binarize[Height][Width][3];
    // CAUTION!
    int error[Height][Width][3];
    int T = 128;

    // CAUTION !
    double FS_filter_LR[3][3] = {{0,0,0},
                                 {0,0,7.0/16},
                                 {3.0/16,5.0/16,1.0/16}};
    double FS_filter_RL[3][3] = {{0,0,0},
                                 {7.0/16,0,0},
                                 {1.0/16,5.0/16,3.0/16}};

    for (i = 0; i < Height; i++) {
        for (j = 0; j < Width; j++) {
            if (i%2 == 0) {

                Binarize[i][j][0] = (C[i][j] < T ? 0 : 255);
                Binarize[i][j][1] = (M[i][j] < T ? 0 : 255);
                Binarize[i][j][2] = (Y[i][j] < T ? 0 : 255);

                error[i][j][0] = C[i][j] - Binarize[i][j][0];
                error[i][j][1] = M[i][j] - Binarize[i][j][1];
                error[i][j][2] = Y[i][j] - Binarize[i][j][2];

                for (m = -1; m < 2; m++) {
                    for (n = -1; n < 2; n++) {
                        C[i+m][j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][j][0]);
                        M[i+m][j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][j][1]);
                        Y[i+m][j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][j][2]);
                    }
                }

            }else{

                Binarize[i][Width-1-j][0] = (C[i][Width-1-j] < T ? 0 : 255);
                Binarize[i][Width-1-j][1] = (M[i][Width-1-j] < T ? 0 : 255);
                Binarize[i][Width-1-j][2] = (Y[i][Width-1-j] < T ? 0 : 255);

                error[i][Width-1-j][0] = C[i][Width-1-j] - Binarize[i][Width-1-j][0];
                error[i][Width-1-j][1] = M[i][Width-1-j] - Binarize[i][Width-1-j][1];
                error[i][Width-1-j][2] = Y[i][Width-1-j] - Binarize[i][Width-1-j][2];

                for (m = -1; m < 2; m++) {
                    for (n = -1; n < 2; n++) {
                        C[i+m][Width-1-j+n] += (int)(FS_filter_RL[m+1][n+1] * error[i][Width-1-j][0]);
                        M[i+m][Width-1-j+n] += (int)(FS_filter_RL[m+1][n+1] * error[i][Width-1-j][1]);
                        Y[i+m][Width-1-j+n] += (int)(FS_filter_RL[m+1][n+1] * error[i][Width-1-j][2]);
                    }
                }
            }
        }
    }

    // Turn Back to RGB for Output on Screen
    unsigned char ImageOut[Height][Width][3];

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            ImageOut[i][j][0] = 255 - Binarize[i][j][0];
            ImageOut[i][j][1] = 255 - Binarize[i][j][1];
            ImageOut[i][j][2] = 255 - Binarize[i][j][2];
        }
    }

    if (!(file = fopen("Separable_Error_Diffusion.raw","wb"))) {
        cout << "Cannot write file: " << "Separable_Error_Diffusion.raw" << endl;
        exit(1);
    }
    fwrite(ImageOut, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    return 0;
}