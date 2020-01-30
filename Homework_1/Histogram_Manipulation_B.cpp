// EE569 Homework Assignment #1
// Problem 1 (c) Histogram Manipulation Method B
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

    // Obtain the Input_Image Histogram
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

    /* ------------------------------------------------------------------------------- */

    // Obtain the Input_Image Cumulative Histogram
    int n = 0;

    int CH_R[256] = {0};
    int CH_G[256] = {0};
    int CH_B[256] = {0};

    CH_R[0] = Histogram_Value_R[0];
    CH_G[0] = Histogram_Value_G[0];
    CH_B[0] = Histogram_Value_B[0];

    for (n=0;n<255;n++) {
        CH_R[n+1] = CH_R[n] + Histogram_Value_R[n+1];
        CH_G[n+1] = CH_G[n] + Histogram_Value_G[n+1];
        CH_B[n+1] = CH_B[n] + Histogram_Value_B[n+1];
    }

    // R
    if (!(file = fopen("Cumulative_Histogram_R.txt","wb"))) {
        cout << "Cannot write file: " << "Cumulative_Histogram_R.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", CH_R[n]);
    }
    fclose(file);

    // G
    if (!(file = fopen("Cumulative_Histogram_G.txt","wb"))) {
        cout << "Cannot write file: " << "Cumulative_Histogram_G.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", CH_G[n]);
    }
    fclose(file);

    // B
    if (!(file = fopen("Cumulative_Histogram_B.txt","wb"))) {
        cout << "Cannot write file: " << "Cumulative_Histogram_B.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", CH_B[n]);
    }
    fclose(file);

    /* ------------------------------------------------------------------------------- */
    // Assign Pixels to Corresponding "Bucket"

    int num = Width*Height/256+1/2; // # of pixels in each bucket
    int count = 1;
    unsigned char Outdata[Height][Width][3];

    int intensity;

    // R
    int new_intensity = 0;
    for (intensity=0;intensity<256;intensity++){
        for (i=0;i<Height;i++) {
            for (j=0;j<Width;j++) {
                if (Imagedata[i][j][0]==intensity){
                    Outdata[i][j][0] = new_intensity;
                    count++;
                    if(count>num){
                        new_intensity++;
                        count = 1;
                    }
                }
            }
        }
    }

    // G
    new_intensity = 0;
    for (intensity=0;intensity<256;intensity++){
        for (i=0;i<Height;i++) {
            for (j=0;j<Width;j++) {
                if (Imagedata[i][j][1]==intensity){
                    Outdata[i][j][1] = new_intensity;
                    count++;
                    if(count>num){
                        new_intensity++;
                        count = 1;
                    }
                }
            }
        }
    }

    // B
    new_intensity = 0;
    for (intensity=0;intensity<256;intensity++){
        for (i=0;i<Height;i++) {
            for (j=0;j<Width;j++) {
                if (Imagedata[i][j][2]==intensity){
                    Outdata[i][j][2] = new_intensity;
                    count++;
                    if(count>num){
                        new_intensity++;
                        count = 1;
                    }
                }
            }
        }
    }

    /* ------------------------------------------------------------------------------- */

    // Write image data from image data matrix
    if (!(file = fopen("Method_B_Enhanced.raw","wb"))) {
        cout << "Cannot write file: " << "Method_B_Enhanced.raw" << endl;
        exit(1);
    }
    fwrite(Outdata, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    // Obtain the Output_Image Histogram
    int Out_Histogram_Value_R[256] = {0};
    int Out_Histogram_Value_G[256] = {0};
    int Out_Histogram_Value_B[256] = {0};

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            int k;

            // R
            for (k=0;k<256;k++) {
                if (Outdata[i][j][0]==k) {
                    Out_Histogram_Value_R[k]+=1;
                    break;
                }
            }

            // G
            for (k=0;k<256;k++) {
                if (Outdata[i][j][1]==k) {
                    Out_Histogram_Value_G[k]+=1;
                    break;
                }
            }

            // B
            for (k=0;k<256;k++) {
                if (Outdata[i][j][2]==k) {
                    Out_Histogram_Value_B[k]+=1;
                    break;
                }
            }
        }
    }

    /* ------------------------------------------------------------------------------- */

    // Obtain the Output_Image Cumulative Histogram

    int Out_CH_R[256] = {0};
    int Out_CH_G[256] = {0};
    int Out_CH_B[256] = {0};

    Out_CH_R[0] = Out_Histogram_Value_R[0];
    Out_CH_G[0] = Out_Histogram_Value_G[0];
    Out_CH_B[0] = Out_Histogram_Value_B[0];

    for (n=0;n<255;n++) {
        Out_CH_R[n+1] = Out_CH_R[n] + Out_Histogram_Value_R[n+1];
        Out_CH_G[n+1] = Out_CH_G[n] + Out_Histogram_Value_G[n+1];
        Out_CH_B[n+1] = Out_CH_B[n] + Out_Histogram_Value_B[n+1];
    }

    // R
    if (!(file = fopen("Out_Cumulative_Histogram_R.txt","wb"))) {
        cout << "Cannot write file: " << "Out_Cumulative_Histogram_R.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", Out_CH_R[n]);
    }
    fclose(file);

    // G
    if (!(file = fopen("Out_Cumulative_Histogram_G.txt","wb"))) {
        cout << "Cannot write file: " << "Out_Cumulative_Histogram_G.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", Out_CH_G[n]);
    }
    fclose(file);

    // B
    if (!(file = fopen("Out_Cumulative_Histogram_B.txt","wb"))) {
        cout << "Cannot write file: " << "Out_Cumulative_Histogram_B.txt" << endl;
        exit(1);
    }
    for (n=0;n<256;n++) {
        fprintf(file, "%d \n", Out_CH_B[n]);
    }
    fclose(file);

    return 0;
}