// EE569 Homework Assignment #1
// Problem 1 (c) Histogram Manipulation Method A
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
    unsigned char Imagedata[Height][Width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Obtain the Histogram
    int Histogram_Value_R[256] = {0};
    int Histogram_Value_G[256] = {0};
    int Histogram_Value_B[256] = {0};

    int i,j;
    // Array Index starts from 0
    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            int k;

            // R
            for (k=0;k<256;k++) {
                if (Imagedata[i][j][0]==k) {
                    Histogram_Value_R[k]+=1;
                    break;
                }
            }

            // G
            for (k=0;k<256;k++) {
                if (Imagedata[i][j][1]==k) {
                    Histogram_Value_G[k]+=1;
                    break;
                }
            }

            // B
            for (k=0;k<256;k++) {
                if (Imagedata[i][j][2]==k) {
                    Histogram_Value_B[k]+=1;
                    break;
                }
            }
        }
    }

    int n = 0;

    // R
    if (!(file = fopen("Histogram_Value_R.txt","wb"))) {
        cout << "Cannot write file: " << "Histogram_Value.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", Histogram_Value_R[n]);
    }
    fclose(file);

    // G
    if (!(file = fopen("Histogram_Value_G.txt","wb"))) {
        cout << "Cannot write file: " << "Histogram_Value.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", Histogram_Value_G[n]);
    }
    fclose(file);

    // B
    if (!(file = fopen("Histogram_Value_B.txt","wb"))) {
        cout << "Cannot write file: " << "Histogram_Value.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", Histogram_Value_B[n]);
    }
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Calculate the Normalized Probability Histogram & the CDF
    float CDF_R[256] = {0};
    float CDF_G[256] = {0};
    float CDF_B[256] = {0};

    CDF_R[0] = (float) Histogram_Value_R[0] / (Width*Height);
    CDF_G[0] = (float) Histogram_Value_G[0] / (Width*Height);
    CDF_B[0] = (float) Histogram_Value_B[0] / (Width*Height);

    for (n=0;n<255;n++) {
        CDF_R[n+1] = CDF_R[n] + (float) Histogram_Value_R[n+1] / (Width*Height);
        CDF_G[n+1] = CDF_G[n] + (float) Histogram_Value_G[n+1] / (Width*Height);
        CDF_B[n+1] = CDF_B[n] + (float) Histogram_Value_B[n+1] / (Width*Height);
    }

    /* ------------------------------------------------------------------------------- */

    // Create the Mapping-Table
    int New_Intensity_R[256] = {0};
    int New_Intensity_G[256] = {0};
    int New_Intensity_B[256] = {0};

    // Mapping Rule: n to CDF[n]*255
    for (n=0;n<256;n++) {
        New_Intensity_R[n] = CDF_R[n]*255;
        New_Intensity_G[n] = CDF_G[n]*255;
        New_Intensity_B[n] = CDF_B[n]*255;
    }

    // R
    if (!(file = fopen("New_Intensity_R.txt","wb"))) {
        cout << "Cannot write file: " << "New_Intensity_R.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", New_Intensity_R[n]);
    }
    fclose(file);

    // G
    if (!(file = fopen("New_Intensity_G.txt","wb"))) {
        cout << "Cannot write file: " << "New_Intensity_G.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", New_Intensity_G[n]);
    }
    fclose(file);

    // B
    if (!(file = fopen("New_Intensity_B.txt","wb"))) {
        cout << "Cannot write file: " << "New_Intensity_B.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", New_Intensity_B[n]);
    }
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Enhance Image
    unsigned char Outdata[Height][Width][3];

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            Outdata[i][j][0] = New_Intensity_R[Imagedata[i][j][0]];
            Outdata[i][j][1] = New_Intensity_G[Imagedata[i][j][1]];
            Outdata[i][j][2] = New_Intensity_B[Imagedata[i][j][2]];
        }
    }

    /* ------------------------------------------------------------------------------- */

    // Write image data from image data matrix
    if (!(file = fopen("Method_A_Enhanced.raw","wb"))) {
        cout << "Cannot write file: " << "Method_A_Enhanced.raw" << endl;
        exit(1);
    }
    fwrite(Outdata, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    return 0;
}