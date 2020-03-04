// EE569 Homework Assignment #3
// Problem 1 (b) Homographic Transformation and Image Stitching
// Name: XU Kangyan
// ID: 4876010998
// Email: kangyanx@usc.edu
// Date: Mar 3, 2020

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int Width = 480;
    int Height = 720;

    // Check for proper syntax
    if (argc < 4) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "input_image.raw input_image.raw input_image.raw" << endl;
        return 0;
    }

    // Allocate image data array
    unsigned char left[Height][Width][3];
    unsigned char middle[Height][Width][3];
    unsigned char right[Height][Width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }
    fread(left, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    if (!(file = fopen(argv[2], "rb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fread(middle, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    if (!(file = fopen(argv[3], "rb"))) {
        cout << "Cannot open file: " << argv[3] << endl;
        exit(1);
    }
    fread(right, sizeof(unsigned char), Width*Height*3, file);
    fclose(file);

    /* ------------------------------------------------------------------------------- */

    unsigned char canvas[1280][1280][3];
    int i,j,k;

    // Calculate Homographic Transformation Matrix between Left & Middle

    double X_lm[8][8] = {
            -0.05222690415,      -0.0277047512,    0.06263841702,     0.03646069309,    0.05178984874,     0.02849557712,    -0.06220136161,     -0.03725151901,
            -0.01023820124,   -0.0009886733023,    0.01256182512,    0.001301138335,   0.006840564485,     0.00101689476,   -0.009164188366,    -0.001329359793,
            8.575116757,        2.545375533,     -8.377588359,      -3.349828174,     -5.568492291,      -2.618032708,       6.370963893,        3.422485348,
            -0.02818712023,     -0.02205787181,    0.03709551233,      0.0229346196,    0.02899171527,     0.02075964812,    -0.03790010737,     -0.02163639591,
            -0.03500229254,     -0.01514338408,    0.04606458425,     0.01901726538,    0.03600142514,     0.01188576468,    -0.04706371685,     -0.01575964598,
            14.62388628,        9.224338873,     -19.24568914,      -9.231993733,     -15.04132184,      -6.236246307,        19.6631247,        7.243901167,
            -0.00004503105251,  -0.00003052851739, 0.00005926288142,  0.00004017689584, 0.00004631645382,  0.00003139994709, -0.00006054828273,  -0.00004104832553,
            -0.0000179272697, -0.000003420666282, 0.00002359308963, 0.000004501749991,  0.0000184389996, 0.000003518308435, -0.00002410481953, -0.000004599392144
    };

    double Y_lm[8] = {18+400.5,999.5-523,23+400.5,999.5-203,268+400.5,999.5-579,217+400.5,999.5-316};

    double H_lm[3][3];
    H_lm[2][2] = 1;
    for (i=0;i<3;i++) {
        for (j=0;j<3;j++) {
            k = 0;
            if ((i!=2) || (j!=2)){
                while(k<8){
                    H_lm[i][j] += X_lm[3*i+j][k]*Y_lm[k];
                    k++;
                }
            }
        }
    }

    // Warp Left

    int x_new;
    int y_new;
    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            x_new = 0.5+(H_lm[0][0]*(j+0.5)+H_lm[0][1]*(999.5-i)+H_lm[0][2])/(H_lm[2][0]*(j+0.5)+H_lm[2][1]*(999.5-i)+H_lm[2][2]);
            y_new = 0.5+(H_lm[1][0]*(j+0.5)+H_lm[1][1]*(999.5-i)+H_lm[1][2])/(H_lm[2][0]*(j+0.5)+H_lm[2][1]*(999.5-i)+H_lm[2][2]);
            if ((y_new>=0)&&(y_new<1280)){
                for (k=0;k<3;k++){canvas[1279-y_new][x_new][k] = left[i][j][k];}
            }
        }
    }

    // Filter Left

    int p,q,temp,count;
    bool flag_1,flag_2,flag_3,flag_4;

    for (i=2;i<1278;i++) {
        for (j=2;j<1278;j++) {
            if ((int)canvas[i][j][0] == 0 && (int)canvas[i][j][1] == 0 && (int)canvas[i][j][2] == 0){

                // To judge if edge, in 5*5 neighbor NO two black lines.
                flag_1 = 0;
                flag_2 = 0;
                flag_3 = 0;
                flag_4 = 0;

                for (k=0;k<3;k++) {
                    count = 0;
                    temp = 0;

                    for (p = -2; p < 3; p++) {
                        for (q = -2; q < 3; q++) {
                            if ((int)canvas[i + p][j + q][k] > 0) {
                                if (p > 0) flag_1 = 1;
                                if (p < 0) flag_2 = 1;
                                if (q > 0) flag_3 = 1;
                                if (q < 0) flag_4 = 1;
                            }
                        }
                    }

                    for (p = -1; p < 2; p++) {
                        for (q = -1; q < 2; q++) {
                            if ((int)canvas[i + p][j + q][k] > 0) {
                                temp += (int)canvas[i + p][j + q][k];
                                count++;
                            }
                        }
                    }

                    if (flag_1 == 1 && flag_2 == 1 && flag_3 == 1 && flag_4 == 1) canvas[i][j][k] = (int)(temp*1.0 / count);
                }
            }
        }
    }

    /* ------------------------------------------------------------------------------- */

    // Calculate Homographic Transformation Matrix between Right & Middle

    double X_rm[8][8] = {
            0.01235229536,      0.01314892717,      0.003024153649,      0.007318404425,    -0.009301805184,    -0.007999246619,    -0.006074643826,     -0.01246808497,
            0.01040453125,   -0.0008787822302,      0.007528759209,    -0.0004891109123,     -0.00808724653,    0.0005346136376,    -0.009846043932,    0.0008332795048,
            -11.59273965,        -7.25331175,         -3.68216111,        -4.037034211,        10.39583293,        4.412605588,        5.879067835,        6.877740372,
            -0.004214801228,     0.008044672495,     -0.002345865907,     0.0006266252536,     0.002564105348,    -0.006681231109,     0.003996561787,     -0.00199006664,
            0.02040519815,     0.004612894485,       0.01135708568,       0.00430526123,     -0.01241365248,    -0.004563861675,     -0.01934863135,     -0.00435429404,
            -2.02209969,       -6.338925497,         -1.12545633,       -0.7580034006,        1.230159229,        7.199636416,        1.917396791,        0.897292481,
            -0.0000005127988403,  0.000007926041519, -0.0000002854125857,   0.000004411460843, 0.0000003119649488, -0.000004821865694, 0.0000004862464773, -0.000007515636668,
            0.00001551234779, -0.000001506854254,   0.000008633832498, -0.0000008386820231, -0.000009437050948, 0.0000009167058758,  -0.00001470912934,  0.000001428830401
    };

    double Y_rm[8] = {426+400.5,999.5-559,262+400.5,999.5-37,254+400.5,999.5-565,450+400.5,999.5-176};

    double H_rm[3][3];
    H_rm[2][2] = 1;
    for (i=0;i<3;i++) {
        for (j=0;j<3;j++) {
            k = 0;
            if ((i!=2) || (j!=2)){
                while(k<8){
                    H_rm[i][j] += X_rm[3*i+j][k]*Y_rm[k];
                    k++;
                }
            }
        }
    }

    // Warp Right

    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            x_new = 0.5+(H_rm[0][0]*(j+800.5)+H_rm[0][1]*(999.5-i)+H_rm[0][2])/(H_rm[2][0]*(j+800.5)+H_rm[2][1]*(999.5-i)+H_rm[2][2]);
            y_new = 0.5+(H_rm[1][0]*(j+800.5)+H_rm[1][1]*(999.5-i)+H_rm[1][2])/(H_rm[2][0]*(j+800.5)+H_rm[2][1]*(999.5-i)+H_rm[2][2]);
            if ((y_new>=0)&&(y_new<1280)){
                if((int)canvas[1279-y_new][x_new][0] == 0 && (int)canvas[1279-y_new][x_new][1] == 0 && (int)canvas[1279-y_new][x_new][2] == 0){
                    for (k=0;k<3;k++){canvas[1279-y_new][x_new][k] = right[i][j][k];}
                }else{
                    for (k=0;k<3;k++){canvas[1279-y_new][x_new][k] = (int)(right[i][j][k]*0.5+canvas[1279-y_new][x_new][k]*0.5);}
                }
            }
        }
    }

    // Filter Right

    for (i=2;i<1278;i++) {
        for (j=2;j<1278;j++) {
            if ((int)canvas[i][j][0] == 0 && (int)canvas[i][j][1] == 0 && (int)canvas[i][j][2] == 0){

                flag_1 = 0;
                flag_2 = 0;
                flag_3 = 0;
                flag_4 = 0;

                for (k=0;k<3;k++) {
                    count = 0;
                    temp = 0;

                    for (p = -2; p < 3; p++) {
                        for (q = -2; q < 3; q++) {
                            if ((int)canvas[i + p][j + q][k] > 0) {
                                if (p > 0) flag_1 = 1;
                                if (p < 0) flag_2 = 1;
                                if (q > 0) flag_3 = 1;
                                if (q < 0) flag_4 = 1;
                            }
                        }
                    }

                    for (p = -1; p < 2; p++) {
                        for (q = -1; q < 2; q++) {
                            if ((int)canvas[i + p][j + q][k] > 0) {
                                temp += (int)canvas[i + p][j + q][k];
                                count++;
                            }
                        }
                    }

                    if (flag_1 == 1 && flag_2 == 1 && flag_3 == 1 && flag_4 == 1) canvas[i][j][k] = (int)(temp*1.0 / count);
                }
            }
        }
    }

    /* ------------------------------------------------------------------------------- */

    // Set the middle on the canvas (width:400+480+400 height:280+720+280)
    for (i=0;i<Height;i++) {
        for (j=0;j<Width;j++) {
            for (k=0;k<3;k++) {
                canvas[i+280][j+400][k] = (int)(0.5* middle[i][j][k] + 0.5*canvas[i+280][j+400][k]);
            }
        }
    }

    /* ------------------------------------------------------------------------------- */

    // Output

    if (!(file = fopen("Stitched.raw","wb"))) {
        cout << "Cannot write file: " << "Stitched.raw" << endl;
        exit(1);
    }
    fwrite(canvas, sizeof(unsigned char), 1280*1280*3, file);
    fclose(file);

    return 0;
}