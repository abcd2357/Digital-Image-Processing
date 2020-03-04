// EE569 Homework Assignment #2
// Problem 2 (b) Error Diffusion
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
    unsigned char ImageInput[Height][Width];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(ImageInput, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */
    // Floyd - Steinberg Error Diffusion

    // copy ImageInput to Imagedata
    int i,j,m,n;
    unsigned char Imagedata[Height][Width];

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Imagedata[i][j] = ImageInput[i][j];
        }
    }
    /* ---------------------------------- */

    unsigned char Binarize[Height][Width];
    // CAUTION!
    int error[Height][Width];
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
                Binarize[i][j] = (Imagedata[i][j] < T ? 0 : 255);
                error[i][j] = Imagedata[i][j] - Binarize[i][j];
                for (m = -1; m < 2; m++) {
                    for (n = -1; n < 2; n++) {
                        Imagedata[i+m][j+n] += (int)(FS_filter_LR[m+1][n+1]*error[i][j]);
                    }
                }
            }else{
                Binarize[i][Width-1-j] = (Imagedata[i][Width-1-j] < T ? 0 : 255);
                error[i][Width-1-j] = Imagedata[i][Width-1-j] - Binarize[i][Width-1-j];
                for (m = -1; m < 2; m++) {
                    for (n = -1; n < 2; n++) {
                        Imagedata[i+m][Width-1-j+n] += (int)(FS_filter_LR[m+1][n+1]*error[i][Width-1-j]);
                    }
                }
            }
        }
    }

    if (!(file = fopen("Floyd_Steinberg.raw","wb"))) {
        cout << "Cannot write file: " << "Floyd_Steinberg.raw" << endl;
        exit(1);
    }
    fwrite(Binarize, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */
    // JJN Error Diffusion

    // copy ImageInput to Imagedata
    unsigned char Imagedata_JJN[Height][Width];

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Imagedata_JJN[i][j] = ImageInput[i][j];
        }
    }
    /* ---------------------------------- */

    unsigned char Binarize_JJN[Height][Width];
    int error_JJN[Height][Width];

    // CAUTION !
    double JJN_filter_LR[5][5] = {{0,0,0,0,0},
                               {0,0,0,0,0},
                               {0,0,0,7.0/48,5.0/48},
                               {3.0/48,5.0/48,7.0/48,5.0/48,3.0/48},
                               {1.0/48,3.0/48,5.0/48,3.0/48,1.0/48}};
    double JJN_filter_RL[5][5] = {{0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {5.0/48,7.0/48,0,0,0},
                                  {3.0/48,5.0/48,7.0/48,5.0/48,3.0/48},
                                  {1.0/48,3.0/48,5.0/48,3.0/48,1.0/48}};

    for (i = 0; i < Height; i++) {
        for (j = 0; j < Width; j++) {
            if (i%2 == 0) {
                Binarize_JJN[i][j] = (Imagedata_JJN[i][j] < T ? 0 : 255);
                error_JJN[i][j] = Imagedata_JJN[i][j] - Binarize_JJN[i][j];
                for (m = -2; m < 3; m++) {
                    for (n = -2; n < 3; n++) {
                        Imagedata_JJN[i+m][j+n] += (int)(JJN_filter_LR[m+2][n+2]*error_JJN[i][j]);
                    }
                }
            }else{
                Binarize_JJN[i][Width-1-j] = (Imagedata_JJN[i][Width-1-j] < T ? 0 : 255);
                error_JJN[i][Width-1-j] = Imagedata_JJN[i][Width-1-j] - Binarize_JJN[i][Width-1-j];
                for (m = -2; m < 3; m++) {
                    for (n = -2; n < 3; n++) {
                        Imagedata_JJN[i+m][Width-1-j+n] += (int)(JJN_filter_LR[m+2][n+2]*error_JJN[i][Width-1-j]);
                    }
                }
            }
        }
    }

    if (!(file = fopen("JJN.raw","wb"))) {
        cout << "Cannot write file: " << "JJN.raw" << endl;
        exit(1);
    }
    fwrite(Binarize_JJN, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */
    // Stucki Error Diffusion

    // copy ImageInput to Imagedata
    unsigned char Imagedata_STU[Height][Width];

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Imagedata_STU[i][j] = ImageInput[i][j];
        }
    }
    /* ---------------------------------- */

    unsigned char Binarize_STU[Height][Width];
    int error_STU[Height][Width];

    // CAUTION !
    double STU_filter_LR[5][5] = {{0,0,0,0,0},
                               {0,0,0,0,0},
                               {0,0,0,8.0/48,4.0/48},
                               {2.0/48,4.0/48,8.0/48,4.0/48,2.0/48},
                               {1.0/48,2.0/48,4.0/48,2.0/48,1.0/48}};
    double STU_filter_RL[5][5] = {{0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {4.0/48,8.0/48,0,0,0},
                                  {2.0/48,4.0/48,8.0/48,4.0/48,2.0/48},
                                  {1.0/48,2.0/48,4.0/48,2.0/48,1.0/48}};

    for (i = 0; i < Height; i++) {
        for (j = 0; j < Width; j++) {
            if  (i%2 == 0) {
                Binarize_STU[i][j] = (Imagedata_STU[i][j] < T ? 0 : 255);
                error_STU[i][j] = Imagedata_STU[i][j] - Binarize_STU[i][j];
                for (m = -2; m < 3; m++) {
                    for (n = -2; n < 3; n++) {
                        Imagedata_STU[i+m][j+n] += (int)(STU_filter_LR[m+2][n+2]*error_STU[i][j]);
                    }
                }
            }else{
                Binarize_STU[i][Width-1-j] = (Imagedata_STU[i][Width-1-j] < T ? 0 : 255);
                error_STU[i][Width-1-j] = Imagedata_STU[i][Width-1-j] - Binarize_STU[i][Width-1-j];
                for (m = -2; m < 3; m++) {
                    for (n = -2; n < 3; n++) {
                        Imagedata_STU[i+m][Width-1-j+n] += (int)(STU_filter_LR[m+2][n+2]*error_STU[i][Width-1-j]);
                    }
                }
            }
        }
    }

    if (!(file = fopen("Stucki.raw","wb"))) {
        cout << "Cannot write file: " << "Stucki.raw" << endl;
        exit(1);
    }
    fwrite(Binarize_STU, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    return 0;
}