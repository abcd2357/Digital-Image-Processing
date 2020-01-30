// EE569 Homework Assignment #1
// Problem 2 (b) Bilateral Filtering
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

    int i,j,t,q;

    // Even Pattern
    unsigned char Image_Extend[Height + 4][Width + 4];

    // Copy Imagedata To Image_Extend
    for (i=2;i<Height+2;i++) {
        for (j=2;j<Width+2;j++) {
            Image_Extend[i][j] = Imagedata[i - 2][j - 2];
        }
    }

    // Left & Right Edge
    for (i=2;i<Height+2;i++) {
        Image_Extend[i][0] = Imagedata[i - 2][2];
        Image_Extend[i][1] = Imagedata[i - 2][1];
        Image_Extend[i][Width + 3] = Imagedata[i - 2][Width - 3];
        Image_Extend[i][Width + 2] = Imagedata[i - 2][Width - 2];
    }

    // Up & Down Edge
    for (j=2;j<Width+2;j++) {
        Image_Extend[0][j] = Imagedata[2][j - 2];
        Image_Extend[1][j] = Imagedata[1][j - 2];
        Image_Extend[Height + 3][j] = Imagedata[Height - 3][j - 2];
        Image_Extend[Height + 2][j] = Imagedata[Height - 2][j - 2];
    }

    // Four Corner
    for (i=0;i<2;i++) {
        // Left UP
        Image_Extend[i][0] = Image_Extend[i][4];
        Image_Extend[i][1] = Image_Extend[i][3];
        // Right UP
        Image_Extend[i][Width + 3] = Image_Extend[i][Width - 1];
        Image_Extend[i][Width + 2] = Image_Extend[i][Width];
        // Left Down
        Image_Extend[Height + 3][i] = Image_Extend[Height - 1][i];
        Image_Extend[Height + 2][i] = Image_Extend[Height][i];
        // Right Down
        Image_Extend[Height + 3][Width + 2 + i] = Image_Extend[Height - 1][Width - i];
        Image_Extend[Height + 2][Width + 2 + i] = Image_Extend[Height][Width - i];
    }

    /* ------------------------------------------------------------------------------- */

    // Bilateral Filtering
    unsigned char Outdata[Height][Width];
    double Bilateral_Filter[5][5];
    double Pseudo_Intesity = 0;
    double Sum = 0;
    double Two_delta_C_Square = 4; // Maintain Two_delta_C_Square equals 2000
    double Two_delta_S_Square = 2000; // Maintain Two_delta_C_Square equals 4

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            for (t=0;t<5;t++) {
                for (q=0;q<5;q++) {
                    Bilateral_Filter[t][q] = exp((-1)*((t-2)*(t-2)+(q-2)*(q-2))/Two_delta_C_Square
                            -(Image_Extend[i+t][j+q]-Image_Extend[i+2][j+2])*(Image_Extend[i+t][j+q]-Image_Extend[i+2][j+2])/Two_delta_S_Square);
                    Sum += Bilateral_Filter[t][q];
                    Pseudo_Intesity += Bilateral_Filter[t][q]*Image_Extend[i+t][j+q];
                }
            }
            Outdata[i][j] = Pseudo_Intesity / Sum;
            Pseudo_Intesity = 0;
            Sum = 0;
        }
    }

    // Write image data from image data matrix
    if (!(file = fopen("5*5_Bilateral_Filtered.raw","wb"))) {
        cout << "Cannot write file: " << "5*5_Bilateral_Filtered.raw" << endl;
        exit(1);
    }
    fwrite(Outdata, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    return 0;
}