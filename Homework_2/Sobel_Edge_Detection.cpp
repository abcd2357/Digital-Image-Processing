// EE569 Homework Assignment #2
// Problem 1 (a) Sobel Edge Detector
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

    // Convert RGB images to grayscale image
    int i,j;
    unsigned char Grayscale[Height][Width];
    double R_coeff = 0.2989;
    double G_coeff = 0.5870;
    double B_coeff = 0.1140;

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Grayscale[i][j] = R_coeff*ImageInput[i][j][0] + G_coeff*ImageInput[i][j][1] + B_coeff*ImageInput[i][j][2];
        }
    }

    /* ------------------------------------------------------------------------------- */

    // Extend Image Boundary
    unsigned char Image_Extend[Height+2][Width+2];

    // Copy Grayscale To Image_Extend
    for (i=1;i<Height+1;i++) {
        for (j=1;j<Width+1;j++) {
            Image_Extend[i][j] = Grayscale[i-1][j-1];
        }
    }

    /* ------------------------------------------------------------------------------- */

    int Gx_filter[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
    int Gy_filter[3][3] = {{1,2,1}, {0,0,0}, {-1,-2,-1}};
    int Gx[Height][Width];
    int Gy[Height][Width];
    int Gradient[Height][Width];
    unsigned char Gx_out[Height][Width];
    unsigned char Gy_out[Height][Width];
    unsigned char Gra_out[Height][Width];
    unsigned char Edge_map[Height][Width];
    int m,n;

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            for (m=0;m<3;m++) {
                for (n=0;n<3;n++) {
                    Gx[i][j] += Image_Extend[i+m][j+n] * Gx_filter[m][n];
                    Gy[i][j] += Image_Extend[i+m][j+n] * Gy_filter[m][n];
                }
            }
            Gradient[i][j] = sqrt(Gx[i][j]*Gx[i][j]+Gy[i][j]*Gy[i][j]);
        }
    }

    int Gx_min = Gx[0][0], Gx_max = Gx[0][0], Gy_min = Gy[0][0], Gy_max = Gy[0][0], Gra_max = Gradient[0][0], Gra_min = Gradient[0][0];

    for (i=0;i<Height;i++) {
        for(j=0;j<Width;j++) {
            if (Gx[i][j] < Gx_min) {Gx_min = Gx[i][j];};
            if (Gy[i][j] < Gy_min) {Gy_min = Gy[i][j];};
            if (Gradient[i][j] < Gra_min) {Gra_min = Gradient[i][j];};
            if (Gx[i][j] > Gx_max) {Gx_max = Gx[i][j];};
            if (Gy[i][j] > Gy_max) {Gy_max = Gy[i][j];};
            if (Gradient[i][j] > Gra_max) {Gra_max = Gradient[i][j];};
        }
    }

    // CAUTION!
    double Gx_Normalized = 255.0/(Gx_max-Gx_min);
    double Gy_Normalized = 255.0/(Gy_max-Gy_min);
    double Gra_Normalized = 255.0/(Gra_max-Gra_min);

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Gx_out[i][j] = Gx_Normalized*(Gx[i][j]-Gx_min);
            Gy_out[i][j] = Gy_Normalized*(Gy[i][j]-Gy_min);
            Gra_out[i][j] = Gra_Normalized*(Gradient[i][j]-Gra_min);
        }
    }

    // Threshold Percent
    double Threshold = 0.37;

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Edge_map[i][j] = (Gradient[i][j] > (Threshold*255) ? 0 : 255);
        }
    }

        /* ------------------------------------------------------------------------------- */

    // Output

    if (!(file = fopen("Sobel_Gx.raw","wb"))) {
        cout << "Cannot write file: " << "Sobel_Gx.raw" << endl;
        exit(1);
    }
    fwrite(Gx_out, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    if (!(file = fopen("Sobel_Gy.raw","wb"))) {
        cout << "Cannot write file: " << "Sobel_Gy.raw" << endl;
        exit(1);
    }
    fwrite(Gy_out, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    if (!(file = fopen("Sobel_Gradient.raw","wb"))) {
        cout << "Cannot write file: " << "Sobel_Gradient.raw" << endl;
        exit(1);
    }
    fwrite(Gra_out, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    if (!(file = fopen("Sobel_Edge_Map.raw","wb"))) {
        cout << "Cannot write file: " << "Sobel_Edge_Map.raw" << endl;
        exit(1);
    }
    fwrite(Edge_map, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    return 0;
}