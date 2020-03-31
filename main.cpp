/*
 * pan.cpp
 * Copyright (C) 2020-03-13 carryhjr@gmail.com
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define WIDTH         7920                    //数字图像水平像素个数
#define HEIGHT        6004                    //数字图像竖直像素个数

unsigned short * dealtRaw(unsigned short * raw) {
    unsigned short *output = (unsigned short *)malloc(3 * sizeof(unsigned short) * WIDTH * HEIGHT); // rgb uint16 thus trebble
    unsigned short *output_start = output;

    int currentTempIndex = 0;
    int nearestBluesAvg = 0;
    int nearestRedsAvg = 0;
    int nearestGreensAvg = 0;

    for (int j = 0; j < HEIGHT / 2; j++)
    {
        for (int i = 0; i < WIDTH; i++) // handle B G B G B G ...
        {
            if (currentTempIndex % 2 == 0) // even, B position
            {
                // red
                if (j == 0) // if in the first row, only take right-bottom
                {
                    nearestRedsAvg = *(raw+currentTempIndex+WIDTH+1);
                } else {
                    nearestRedsAvg = (*(raw+currentTempIndex+WIDTH-1) + *(raw+currentTempIndex+WIDTH-1)) / 2;
                }
                // gren
                nearestGreensAvg = (*(raw+currentTempIndex+1) + *(raw+currentTempIndex+WIDTH)) / 2;
                // blue
                nearestBluesAvg = *(raw+currentTempIndex);

                *output = nearestRedsAvg;
                output++;
                *output = nearestGreensAvg;
                output++;
                *output = nearestBluesAvg;
                output++;

                currentTempIndex++;
            } else { // odd, G
                if (i == 0) {
                    nearestRedsAvg = *(raw+currentTempIndex+WIDTH);
                } else {
                    nearestRedsAvg = (*(raw+currentTempIndex-WIDTH) + *(raw+currentTempIndex+WIDTH)) / 2;
                }
                nearestGreensAvg = *(raw + currentTempIndex);
                if (j == WIDTH - 1) {
                    nearestBluesAvg = *(raw+currentTempIndex-1);
                } else {
                    nearestBluesAvg = (*(raw+currentTempIndex-1) + *(raw+currentTempIndex+1)) / 2;
                }

                *output = nearestRedsAvg;
                output++;
                *output = nearestGreensAvg;
                output++;
                *output = nearestBluesAvg;
                output++;

                currentTempIndex++;

            }
        }

        for (int i = 0; i < WIDTH; i++) // handle G R G R G R ...
        {
            if (currentTempIndex % 2 == 0) // even, G position
            {
                if (i == 0) {
                    nearestRedsAvg = *(raw + currentTempIndex + 1);
                } else {
                    nearestRedsAvg = (*(raw+currentTempIndex-1) + *(raw+currentTempIndex+1)) / 2;
                }
                nearestGreensAvg = *(raw + currentTempIndex);
                if (j == HEIGHT/2 - 1) {
                    nearestBluesAvg = *(raw + currentTempIndex - WIDTH);
                } else {
                    nearestBluesAvg = (*(raw+currentTempIndex - WIDTH) + *(raw+currentTempIndex+WIDTH)) / 2;
                }
                *output = nearestRedsAvg;
                output++;
                *output = nearestGreensAvg;
                output++;
                *output = nearestBluesAvg;
                output++;
                currentTempIndex++;
            } else { // R position
                nearestRedsAvg = *(raw + currentTempIndex);
                nearestGreensAvg = (*(raw+currentTempIndex-WIDTH) + *(raw+currentTempIndex-1)) / 2;
                if (i == WIDTH - 1) {
                    nearestRedsAvg = *(raw+currentTempIndex-WIDTH-1);
                } else {
                    nearestRedsAvg = (*(raw+currentTempIndex-WIDTH-1) + *(raw+currentTempIndex-WIDTH+1)) / 2;
                }
                *output = nearestRedsAvg;
                output++;
                *output = nearestGreensAvg;
                output++;
                *output = nearestBluesAvg;
                output++;
                currentTempIndex++;
            }

        }


    }

    // unsigned char *outputByte = (unsigned char *)malloc(sizeof(unsigned char) * WIDTH * HEIGHT); // gray uint16
    // for (int i = 0; i < HEIGHT * WIDTH; i++)
    //     *(outputByte + i) = *(output_start + i) / 257;

    return output_start;

}

int main() {
    char filepath[] = "/Users/carry/Downloads/yuantu/GQ/GQ-20ms-bayer02.raw";
    FILE *fp = fopen(filepath, "rb");
    if (fp == nullptr) {
        printf("read data failed");
        return -1;
    }
    unsigned short *raw = (unsigned short *)malloc(sizeof(unsigned short) * WIDTH * HEIGHT); // gray uint16
    fread(raw, 1, WIDTH * HEIGHT * 2, fp);
    unsigned short * output  = dealtRaw(raw);


    Mat mat16uc3_rgb(HEIGHT, WIDTH, CV_16UC3, output);
    imshow("image", mat16uc3_rgb);
    waitKey(0);

    // unsigned char *outputByte = (unsigned char *)malloc(sizeof(unsigned char) * WIDTH * HEIGHT);
    // unsigned char *outputByte_start = outputByte;
    // for (int i = 0; i < HEIGHT * WIDTH; i++) {
    //     *outputByte = *output / 257;
    //     outputByte++;
    //     output++;
    // }
    //
    // Mat mat8uc3_rgb(HEIGHT, WIDTH, CV_8UC3, outputByte_start);
    // imshow("image", mat8uc3_rgb);
    // waitKey(0);
    // free(raw);
    // raw = nullptr;
    //
    return 0;
}


