// EE569 Homework Assignment #2
// Problem 2 (c) Color Halftoning with Error Diffusion - MBVQ-based Error Diffusion
// Name: XU Kangyan
// ID: 4876010998
// Email: kangyanx@usc.edu
// Date: Feb 16, 2020

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int Determine_MBVQ(unsigned char R, unsigned char G, unsigned char B) {
    if ((R + G) > 255) {
        if ((G + B) > 255) {
            if ((R + G + B) > 510) { return 1; } // CMYW
            else { return 2; }; // MYGC
        } else {
            return 3;
        }; // RGMY
    } else {
        if ((G + B) <= 255) {
            if ((R + G + B) <= 255) { return 4; } // KRGB
            else { return 5; }; // RGBM
        } else {
            return 6;
        }; // CMGB
    }
}

char getNearestVertex(int mbvq, unsigned char R, unsigned char G, unsigned char B){
    char vertex;

    // No.1 for CMYW
    if (mbvq == 1) {
        vertex = 'W';
        if ((B < 0.5) && (B <= R) && (B <= G)) {
            vertex = 'Y';
        }
        if ((G < 0.5) && (G <= B) && (G <= R)) {
            vertex = 'M';
        }
        if ((R < 0.5) && (R <= B) && (R <= G)) {
            vertex = 'C';
        }
    }

    // No.2 for MYGC
    if (mbvq == 2) {
        vertex = 'M';
        if ((G >= B) && (R >= B)) {
            if (R >= 0.5) {
                vertex = 'Y';
            } else {
                vertex = 'G';
            }
        }
        if ((G >= R) && (B >= R)) {
            if (B >= 0.5) {
                vertex = 'C';
            } else {
                vertex = 'G';
            }
        }
    }

    // No.3 for RGMY
    if (mbvq == 3) {
        if (B > 0.5) {
            if (R > 0.5){
                if (B >= G) {
                    vertex = 'M';
                }else{
                    vertex = 'Y';
                }
            }else{
                if(G>B+R){
                    vertex = 'G';
                }else{
                    vertex = 'M';
                }
            }
        }else{
            if(R >= 0.5){
                if (G >= 0.5){
                    vertex = 'Y';
                }else{
                    vertex = 'R';
                }
            }else{
                if (R >= G){
                    vertex = 'R';
                }else{
                    vertex = 'G';
                }
            }
        }
    }

    // No.4 for KRGB
    if (mbvq == 4) {
        vertex = 'B';
        if ((B > 0.5) && (B >= R) && (B >= G)) {
            vertex = 'B';
        }
        if ((G > 0.5) && (G >= B) && (G >= R)) {
            vertex = 'G';
        }
        if ((R > 0.5) && (R >= B) && (R >= G)) {
            vertex = 'R';
        }
    }

    // No.5 for RGBM
    if (mbvq == 5) {
        vertex = 'G';
        if ((R > G) && (R >= B)) {
            if (B < 0.5) {
                vertex = 'R';
            } else {
                vertex = 'M';
            }
        }
        if ((B > G) && (B >= R)) {
            if (R < 0.5) {
                vertex = 'B';
            } else {
                vertex = 'M';
            }
        }
    }

    // No.6 for CMGB
    if (mbvq == 6) {
        if (B > 0.5) {
            if (R > 0.5){
                if (G >= R) {
                    vertex = 'C';
                }else{
                    vertex = 'M';
                }
            }else{
                if(G > 0.5){
                    vertex = 'C';
                }else{
                    vertex = 'B';
                }
            }
        }else{
            if(R > 0.5){
                if (R-G+B >= 0.5){
                    vertex = 'M';
                }else{
                    vertex = 'G';
                }
            }else{
                if (G >= B){
                    vertex = 'G';
                }else{
                    vertex = 'B';
                }
            }
        }
    }

}

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
    // Using Floyd - Steinberg Error Diffusion
    int i,j,m,n;

    // R G B For clear, not using k=0:2 .
    unsigned char R[Height][Width];
    unsigned char G[Height][Width];
    unsigned char B[Height][Width];

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            R[i][j] = ImageInput[i][j][0];
            G[i][j] = ImageInput[i][j][1];
            B[i][j] = ImageInput[i][j][2];
        }
    }
    /* ---------------------------------- */

    unsigned char Binarize[Height][Width][3];
    // CAUTION!
    int error[Height][Width][3];
    int T = 128;
    int MBVQ;
    char V;
    int Vector[3];

    // CAUTION !
    double FS_filter_LR[3][3] = {{0,0,0},
                                 {0,0,7.0/16},
                                 {3.0/16,5.0/16,1.0/16}};
    double FS_filter_RL[3][3] = {{0,0,0},
                                 {7.0/16,0,0},
                                 {1.0/16,5.0/16,3.0/16}};

    for (i = 0; i < Height; i++) {
        for (j = 0; j < Width; j++) {
            if (i % 2 == 0) {

                // Determine MBVQ
                MBVQ = Determine_MBVQ(R[i][j], G[i][j], B[i][j]);

                // Find the closet vertex in MBVQ
                V = getNearestVertex(MBVQ, R[i][j], G[i][j], B[i][j]);

                switch(V){
                    case'W': Vector[0] = 0;
                             Vector[1] = 0;
                             Vector[2] = 0;
                    case'Y': Vector[0] = 0;
                             Vector[1] = 0;
                             Vector[2] = 255;
                    case'C': Vector[0] = 0;
                             Vector[1] = 255;
                             Vector[2] = 0;
                    case'M': Vector[0] = 255;
                             Vector[1] = 0;
                             Vector[2] = 0;
                    case'G': Vector[0] = 0;
                             Vector[1] = 255;
                             Vector[2] = 255;
                    case'R': Vector[0] = 255;
                             Vector[1] = 0;
                             Vector[2] = 255;
                    case'B': Vector[0] = 255;
                             Vector[1] = 255;
                             Vector[2] = 0;
                    case'K': Vector[0] = 255;
                             Vector[1] = 255;
                             Vector[2] = 255;
                }

                Binarize[i][j][0] = (R[i][j] < T ? 0 : 255);
                Binarize[i][j][1] = (G[i][j] < T ? 0 : 255);
                Binarize[i][j][2] = (B[i][j] < T ? 0 : 255);

                error[i][j][0] = R[i][j] - Binarize[i][j][0];
                error[i][j][1] = G[i][j] - Binarize[i][j][1];
                error[i][j][2] = B[i][j] - Binarize[i][j][2];

                for (m = -1; m < 2; m++) {
                    for (n = -1; n < 2; n++) {
                        R[i+m][j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][j][0] - Vector[0]);
                        G[i+m][j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][j][1] - Vector[1]);
                        B[i+m][j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][j][2] - Vector[2]);
                    }
                }

            }else{

                // Determine MBVQ
                MBVQ = Determine_MBVQ(R[i][Width-1-j], G[i][Width-1-j], B[i][Width-1-j]);

                // Find the closet vertex in MBVQ
                V = getNearestVertex(MBVQ, R[i][Width-1-j], G[i][Width-1-j], B[i][Width-1-j]);

                switch(V){
                    case'W': Vector[0] = 0;
                        Vector[1] = 0;
                        Vector[2] = 0;
                    case'Y': Vector[0] = 0;
                        Vector[1] = 0;
                        Vector[2] = 255;
                    case'C': Vector[0] = 0;
                        Vector[1] = 255;
                        Vector[2] = 0;
                    case'M': Vector[0] = 255;
                        Vector[1] = 0;
                        Vector[2] = 0;
                    case'G': Vector[0] = 0;
                        Vector[1] = 255;
                        Vector[2] = 255;
                    case'R': Vector[0] = 255;
                        Vector[1] = 0;
                        Vector[2] = 255;
                    case'B': Vector[0] = 255;
                        Vector[1] = 255;
                        Vector[2] = 0;
                    case'K': Vector[0] = 255;
                        Vector[1] = 255;
                        Vector[2] = 255;
                }

                Binarize[i][Width-1-j][0] = (R[i][Width-1-j] < T ? 0 : 255);
                Binarize[i][Width-1-j][1] = (G[i][Width-1-j] < T ? 0 : 255);
                Binarize[i][Width-1-j][2] = (B[i][Width-1-j] < T ? 0 : 255);

                error[i][Width-1-j][0] = R[i][Width-1-j] - Binarize[i][Width-1-j][0];
                error[i][Width-1-j][1] = G[i][Width-1-j] - Binarize[i][Width-1-j][1];
                error[i][Width-1-j][2] = B[i][Width-1-j] - Binarize[i][Width-1-j][2];

                for (m = -1; m < 2; m++) {
                    for (n = -1; n < 2; n++) {
                        R[i+m][Width-1-j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][Width-1-j][0] - Vector[0]);
                        G[i+m][Width-1-j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][Width-1-j][1] - Vector[1]);
                        B[i+m][Width-1-j+n] += (int)(FS_filter_LR[m+1][n+1] * error[i][Width-1-j][2] - Vector[2]);
                    }
                }
            }
        }
    }

    if (!(file = fopen("MBVQ.raw","wb"))) {
        cout << "Cannot write file: " << "MBVQ.raw" << endl;
        exit(1);
    }
    fwrite(Binarize, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    return 0;
}