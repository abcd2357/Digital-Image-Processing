// EE569 Homework Assignment #3
// Problem 2 (c) Counting games
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

    int shrink_Conditional_pattern_num = 58;
    int shrink_Unconditional_pattern_num = 37;
    const unsigned char M = 255;
    const unsigned char I = 255;
    const unsigned char A = 16;
    const unsigned char B = 32;
    const unsigned char C = 48;
    const unsigned char D = 64;

    const unsigned char shrink_Conditional_pattern[58][9] {
            // S1
            {0,0,I,0,I,0,0,0,0},{I,0,0,0,I,0,0,0,0},{0,0,0,0,I,0,I,0,0},{0,0,0,0,I,0,0,0,I},
            // S2
            {0,0,0,0,I,I,0,0,0},{0,I,0,0,I,0,0,0,0},{0,0,0,I,I,0,0,0,0},{0,0,0,0,I,0,0,I,0},
            // S3
            {0,0,I,0,I,I,0,0,0},{0,I,I,0,I,0,0,0,0},{I,I,0,0,I,0,0,0,0},{I,0,0,I,I,0,0,0,0},
            {0,0,0,I,I,0,I,0,0},{0,0,0,0,I,0,I,I,0},{0,0,0,0,I,0,0,I,I},{0,0,0,0,I,I,0,0,I},
            // STK4
            {0,0,I,0,I,I,0,0,I},{I,I,I,0,I,0,0,0,0},{I,0,0,I,I,0,I,0,0},{0,0,0,0,I,0,I,I,I},
            // ST5
            {I,I,0,0,I,I,0,0,0},{0,I,0,0,I,I,0,0,I},{0,I,I,I,I,0,0,0,0},{0,0,I,0,I,I,0,I,0},
            // ST5
            {0,I,I,0,I,I,0,0,0},{I,I,0,I,I,0,0,0,0},{0,0,0,I,I,0,I,I,0},{0,0,0,0,I,I,0,I,I},
            // ST6
            {I,I,0,0,I,I,0,0,I},{0,I,I,I,I,0,I,0,0},
            // STK6
            {I,I,I,0,I,I,0,0,0},{0,I,I,0,I,I,0,0,I},{I,I,I,I,I,0,0,0,0},{I,I,0,I,I,0,I,0,0},
            {I,0,0,I,I,0,I,I,0},{0,0,0,I,I,0,I,I,I},{0,0,0,0,I,I,I,I,I},{0,0,I,0,I,I,0,I,I},
            // STK7
            {I,I,I,0,I,I,0,0,I},{I,I,I,I,I,0,I,0,0},{I,0,0,I,I,0,I,I,I},{0,0,I,0,I,I,I,I,I},
            // STK8
            {0,I,I,0,I,I,0,I,I},{I,I,I,I,I,I,0,0,0},{I,I,0,I,I,0,I,I,0},{0,0,0,I,I,I,I,I,I},
            // STK9
            {I,I,I,0,I,I,0,I,I},{0,I,I,0,I,I,I,I,I},{I,I,I,I,I,I,I,0,0},{I,I,I,I,I,I,0,0,I},
            {I,I,I,I,I,0,I,I,0},{I,I,0,I,I,0,I,I,I},{I,0,0,I,I,I,I,I,I},{0,0,I,I,I,I,I,I,I},
            // STK10
            {I,I,I,0,I,I,I,I,I},{I,I,I,I,I,I,I,0,I},{I,I,I,I,I,0,I,I,I},{I,0,I,I,I,I,I,I,I}

    };

    const unsigned char shrink_Unconditional_pattern[37][9] {
            // Spur
            {0,0,M,0,M,0,0,0,0},{M,0,0,0,M,0,0,0,0},
            // Single 4-connection
            {0,0,0,0,M,0,0,M,0},{0,0,0,0,M,M,0,0,0},
            // L Cluster
            {0,0,M,0,M,M,0,0,0},{0,M,M,0,M,0,0,0,0},{M,M,0,0,M,0,0,0,0},{M,0,0,M,M,0,0,0,0},
            {0,0,0,M,M,0,M,0,0},{0,0,0,0,M,0,M,M,0},{0,0,0,0,M,0,0,M,M},{0,0,0,0,M,M,0,0,M},
            // 4-connected Offset
            {0,M,M,M,M,0,0,0,0},{M,M,0,0,M,M,0,0,0},{0,M,0,0,M,M,0,0,M},{0,0,M,0,M,M,0,M,0},
            // Spur corner Cluster
            {0,A,M,0,M,B,M,0,0},{M,B,0,A,M,0,0,0,M},{0,0,M,A,M,0,M,B,0},{M,0,0,0,M,B,0,A,M},
            // Corner Clutter
            {M,M,D,M,M,D,D,D,D},
            // Tee Branch
            {D,M,0,M,M,M,D,0,0},{0,M,D,M,M,M,0,0,D},{0,0,D,M,M,M,0,M,D},{D,0,0,M,M,M,D,M,0},
            {D,M,D,M,M,0,0,M,0},{0,M,0,M,M,0,D,M,D},{0,M,0,0,M,M,D,M,D},{D,M,D,0,M,M,0,M,0},
            // Vee Branch
            {M,D,M,D,M,D,A,B,C},{M,D,C,D,M,B,M,D,A},{C,B,A,D,M,D,M,D,M},{A,D,M,B,M,D,C,D,M},
            // Diagonal Branch
            {D,M,0,0,M,M,M,0,D},{0,M,D,M,M,0,D,0,M},{D,0,M,M,M,0,0,M,D},{M,0,D,0,M,M,D,M,0}
    };

    /* ------------------------------------------------------------------------------- */

    int i,j,k,m,p,q,count;
    int out_num = 0;
    unsigned char Test[Height][Width];
    unsigned char Test_copy[Height][Width];
    unsigned char ImageOutput[Height][Width];
    bool Keep, ABC_ALL_0, ABC_Flag, First_match, match;
    int star_count_now = 0, star_count_last = 0, star_count_add = 0;

    int T = 50;
    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            ImageOutput[i][j] = (ImageInput[i][j] < T) ? 0:255;
        }
    }

    if (!(file = fopen("stars_01.raw","wb"))) {
        cout << "Cannot write file: " << "stars_01.raw" << endl;
        exit(1);
    }
    fwrite(ImageOutput, sizeof(unsigned char), Width*Height, file);
    fclose(file);

    while(1){

        First_match = 0;
        for (i=0;i<Height;i++) {
            for (j=0;j<Width;j++) {
                Test[i][j] = 0;
                Test_copy[i][j] = 0;
            }
        }

        for (i=1;i<Height-1;i++) {
            for (j=1;j<Width-1;j++) {

                // Conditional Mark Patterns Match
                for (k=0;k<shrink_Conditional_pattern_num;k++) {
                    if (
                            (ImageOutput[i-1][j-1] == shrink_Conditional_pattern[k][0]) &&
                            (ImageOutput[i-1][j] == shrink_Conditional_pattern[k][1]) &&
                            (ImageOutput[i-1][j+1] == shrink_Conditional_pattern[k][2]) &&
                            (ImageOutput[i][j-1] == shrink_Conditional_pattern[k][3]) &&
                            (ImageOutput[i][j] == shrink_Conditional_pattern[k][4]) &&
                            (ImageOutput[i][j+1] == shrink_Conditional_pattern[k][5]) &&
                            (ImageOutput[i+1][j-1] == shrink_Conditional_pattern[k][6]) &&
                            (ImageOutput[i+1][j] == shrink_Conditional_pattern[k][7]) &&
                            (ImageOutput[i+1][j+1] == shrink_Conditional_pattern[k][8])
                            ) {Test[i][j] = M;
                            Test_copy[i][j] = M;
                        First_match = 1;
                        break;}
                }
            }
        }

        for (i = 1; i < Height - 1; i++) {
            for (j = 1; j < Width - 1; j++) {
                match = 0;

                for (k = 0; k < shrink_Unconditional_pattern_num; k++) {
                    Keep = 1;
                    ABC_ALL_0 = 1;
                    ABC_Flag = 0;

                    for (m = 0; m < 9; m++) {
                        p = m / 3 - 1;
                        q = m % 3 - 1;
                        switch (shrink_Unconditional_pattern[k][m]) {
                            case 0:
                                if (Test[i + p][j + q] == M) { Keep = 0; };
                                break;
                            case M:
                                if (Test[i + p][j + q] == 0) { Keep = 0; };
                                break;
                            case A:
                                ABC_Flag = 1;
                                if (Test[i + p][j + q] == M) { ABC_ALL_0 = 0; };
                                break;
                            case B:
                                if (Test[i + p][j + q] == M) { ABC_ALL_0 = 0; };
                                break;
                            case C:
                                if (Test[i + p][j + q] == M) { ABC_ALL_0 = 0; };
                                break;
                            case D:
                                break;
                        }
                        if (Keep == 0) break;
                    }

                    if (ABC_Flag == 0 && Keep == 1) {
                        match = 1;
                        break;
                    }

                    if (ABC_Flag == 1 && ABC_ALL_0 == 0 && Keep == 1) {
                        match = 1;
                        break;
                    }
                }

                if (First_match == 1 && match == 1) {
                    Test_copy[i][j] = 0;
                    break;
                }
            }
        }

        count = 0;
        // Erase if not match
        for (i=0;i<Height;i++) {
            for (j=0;j<Width;j++) {
                if(Test_copy[i][j] == M){
                    ImageOutput[i][j] = 0;
                    count++;
                }
            }
        }

        // cout << count <<endl;

        for (i=1;i<Height-1;i++) {
            for (j=1;j<Width-1;j++) {
                if (
                        (ImageOutput[i-1][j-1] == 0) &&
                        (ImageOutput[i-1][j] == 0) &&
                        (ImageOutput[i-1][j+1] == 0) &&
                        (ImageOutput[i][j-1] == 0) &&
                        (ImageOutput[i][j] == 255) &&
                        (ImageOutput[i][j+1] == 0) &&
                        (ImageOutput[i+1][j-1] == 0) &&
                        (ImageOutput[i+1][j] == 0) &&
                        (ImageOutput[i+1][j+1] == 0)
                        ){star_count_now++;
                }
            }
        }
        star_count_add = star_count_now - star_count_last;
        star_count_last = star_count_now;

        cout << "star_count_add = " << star_count_add << "  star_count_now = " << star_count_now << endl;
        star_count_now = 0;

        if (First_match == 1) {

            out_num++;
            if (out_num == 10) {

                out_num = 0;
                char filename[32];
                snprintf(filename, sizeof(char) * 32, "stars_shrink %i.raw", count);

                if (!(file = fopen(filename,"wb"))) {
                    cout << "Cannot write file: " << filename << endl;
                    exit(1);
                }
                fwrite(ImageOutput, sizeof(unsigned char), Width*Height, file);
                fclose(file);
            }

        }else{

            if (!(file = fopen("stars_shrink_final.raw","wb"))) {
                cout << "Cannot write file: " << "stars_shrink_final.raw" << endl;
                exit(1);
            }
            fwrite(ImageOutput, sizeof(unsigned char), Width*Height, file);
            fclose(file);
            break;
        }

    }

    int star_count = 0;
    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            if (ImageOutput[i][j] == 255) star_count++;
        }
    }
    cout << "star count = " << star_count << endl;

    /* ------------------------------------------------------------------------------- */


    return 0;
}