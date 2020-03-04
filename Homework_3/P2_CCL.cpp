// EE569 Homework Assignment #3
// Connected-component labeling
// Name: XU Kangyan
// ID: 4876010998
// Email: kangyanx@usc.edu
// Date: Mar 3, 2020

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int Width, Height;

    // Check for proper syntax
    if (argc < 4) {
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
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(ImageInput, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    int i,j;
    int new_label = 1;
    int label[Height][Width];
    int tmp = Width*Height+1;

    // labelling
    for (i=1; i<Height-1; i++) {
        for (j=1; j<Width-1; j++) {
            if (ImageInput[i][j] == 255){

                if ((label[i-1][j-1]==0)&&(label[i-1][j]==0)&&(label[i-1][j+1]==0)&(label[i][j-1]==0)){
                    new_label++;
                    label[i][j] = new_label;
                }

                if(label[i-1][j-1]!=0 && label[i-1][j-1]<tmp)tmp = label[i-1][j-1];

                if(label[i-1][j]!=0 && label[i-1][j]<tmp)tmp = label[i-1][j];

                if(label[i-1][j+1]!=0 && label[i-1][j+1]<tmp)tmp = label[i-1][j+1];

                if(label[i][j-1]!=0 && label[i][j-1]<tmp)tmp = label[i][j-1];

                if(tmp!=Width*Height+1)label[i][j] = tmp;

                tmp=Width*Height+1;

            }
        }
    }

    int count = 200;

    while(count>0) {
        for (i=1; i<Height-1; i++) {
            for (j = 1; j < Width - 1; j++) {
                if (ImageInput[i][j] == 255) {
                    if(label[i-1][j-1] > label[i][j])label[i-1][j-1] = label[i][j];
                    if(label[i-1][j] > label[i][j])label[i-1][j] = label[i][j];
                    if(label[i-1][j+1] > label[i][j])label[i-1][j+1] = label[i][j];
                    if(label[i][j-1] > label[i][j])label[i][j-1] = label[i][j];
                }
            }
        }
        count--;
    }

    int k;
    int label_count = 0;
    int label_num = 0;
    for (k=1; k<new_label+1; k++){
        for (i=1; i<Height-1; i++) {
            for (j = 1; j < Width - 1; j++) {
                if (ImageInput[i][j] == 255) {
                    if (label[i][j] == k) label_count ++;
                }
            }
        }
        if (label_count!=0) {cout << "The label " << k << " has " << label_count << " counts." << endl;
        label_num++;}
        label_count = 0;
    }

    cout << "Total label number is " << label_num-1 << endl;

    /* ------------------------------------------------------------------------------- */

    return 0;
}