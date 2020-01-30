// EE569 Homework Assignment #1
// Problem 2 (a) Basic Denoising Method with Uniform Weight Filter & Gaussian Weight Filter
// Name: XU Kangyan
// Date: Jan 28, 2020

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

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Obtain the Input_Image Histogram
    int Histogram_Value[256] = {0};

    int i,j,k;
    // Array Index starts from 0
    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            for (k=0;k<256;k++) {
                if (Imagedata[i][j]==k) {
                    Histogram_Value[k]+=1;
                    break;
                }
            }
        }
    }

    if (!(file = fopen("Histogram_Value.txt","wb"))) {
        cout << "Cannot write file: " << "Histogram_Value.txt" << endl;
        exit(1);
    }

    int n;
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", Histogram_Value[n]);
    }
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Even Pattern
    unsigned char Image_Extend[Height+2][Width+2];

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

    // Basic Denoising Method using 3*3 Uniform Weight Filter
    int t,q;
    int Itensity = 0;
    int Uniform_Filter_3[3][3] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; // CAUTION!
    unsigned char Outdata_1[Height][Width];

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            for (t=0;t<3;t++) {
                for (q=0;q<3;q++) {
                    Itensity += Uniform_Filter_3[t][q]*Image_Extend[i+t][j+q];
                }
            }
            Outdata_1[i][j] = Itensity / 9;
            Itensity = 0;
        }
    }

    // Write image data from image data matrix
    if (!(file = fopen("3*3_Uniform_Filtered.raw","wb"))) {
        cout << "Cannot write file: " << "3*3_Uniform_Filtered.raw" << endl;
        exit(1);
    }
    fwrite(Outdata_1, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Even Pattern
    unsigned char Image_Extend_2[Height+4][Width+4];

    // Copy Imagedata To Image_Extend
    for (i=2;i<Height+2;i++) {
        for (j=2;j<Width+2;j++) {
            Image_Extend_2[i][j] = Imagedata[i-2][j-2];
        }
    }

    // Left & Right Edge
    for (i=2;i<Height+2;i++) {
        Image_Extend_2[i][0] = Imagedata[i-2][2];
        Image_Extend_2[i][1] = Imagedata[i-2][1];
        Image_Extend_2[i][Width+3] = Imagedata[i-2][Width-3];
        Image_Extend_2[i][Width+2] = Imagedata[i-2][Width-2];
    }

    // Up & Down Edge
    for (j=2;j<Width+2;j++) {
        Image_Extend_2[0][j] = Imagedata[2][j-2];
        Image_Extend_2[1][j] = Imagedata[1][j-2];
        Image_Extend_2[Height+3][j] = Imagedata[Height-3][j-2];
        Image_Extend_2[Height+2][j] = Imagedata[Height-2][j-2];
    }

    // Four Corner
    for (i=0;i<2;i++) {
        // Left UP
        Image_Extend_2[i][0] = Image_Extend[i][4];
        Image_Extend_2[i][1] = Image_Extend[i][3];
        // Right UP
        Image_Extend_2[i][Width+3] = Image_Extend[i][Width-1];
        Image_Extend_2[i][Width+2] = Image_Extend[i][Width];
        // Left Down
        Image_Extend_2[Height+3][i] = Image_Extend[Height-1][i];
        Image_Extend_2[Height+2][i] = Image_Extend[Height][i];
        // Right Down
        Image_Extend_2[Height+3][Width+2+i] = Image_Extend[Height-1][Width-i];
        Image_Extend_2[Height+2][Width+2+i] = Image_Extend[Height][Width-i];
    }

    /* ------------------------------------------------------------------------------- */

    // Basic Denoising Method using 5*5 Uniform Weight Filter
    unsigned char Outdata_2[Height][Width];
    int Uniform_Filter_5[5][5] = {1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 1,
                                  1, 1, 1, 1, 1};

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            for (t=0;t<5;t++) {
                for (q=0;q<5;q++) {
                    Itensity += Uniform_Filter_5[t][q]*Image_Extend_2[i+t][j+q];
                }
            }
            Outdata_2[i][j] = Itensity/25;
            Itensity = 0;
        }
    }

    // Write image data from image data matrix
    if (!(file = fopen("5*5_Uniform_Filtered.raw","wb"))) {
        cout << "Cannot write file: " << "5*5_Uniform_Filtered.raw" << endl;
        exit(1);
    }
    fwrite(Outdata_2, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Basic Denoising Method using 5*5 Gaussian Weight Filter
    unsigned char Outdata_3[Height][Width];
    int Gaussian_Filter_5[5][5] = {1, 4, 7, 4, 1,
                                  4, 16, 26, 16, 4,
                                  7, 26, 41, 26, 7,
                                  4, 16, 26, 16, 4,
                                  1, 4, 7, 4, 1};

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            for (t=0;t<5;t++) {
                for (q=0;q<5;q++) {
                    Itensity += Gaussian_Filter_5[t][q]*Image_Extend_2[i+t][j+q];
                }
            }
            Outdata_3[i][j] = Itensity/273;
            Itensity = 0;
        }
    }

    // Write image data from image data matrix
    if (!(file = fopen("5*5_Gaussian_Filtered.raw","wb"))) {
        cout << "Cannot write file: " << "5*5_Gaussian_Filtered.raw" << endl;
        exit(1);
    }
    fwrite(Outdata_3, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    return 0;
}