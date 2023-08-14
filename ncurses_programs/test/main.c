/*
#include <stdio.h>

int main() {
    printf("\x1b[2J");
    fflush(stdout);
    printf("\x1b[H");
    fflush(stdout);

    printf("    __  _\n");
    printf("  / __ \\(_)_______  ____  ____ _\n");
    printf(" / /_/ / / ___/ _ \\/ __ \\/ __ `/ \n");
    printf("/ ____/ / /  /  __/ /_/ / /_/ /  \n");
    printf("/_/   /_/_/   \\___/ .___/\\__,_/   \n");
    printf("                /_/             \n");

    return 0;
}
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

float A, B, C;
float x, y, z, ooz;
float K1 = 40.0f;
int xp, yp;
int idx;

float cubewidth = 10;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
int distanceFromCam = 60;

float incrementSpeed = 0.6;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1.0f / z;

    xp = (int)(width / 2.0f + K1 * ooz * x + 2);
    yp = (int)(height / 2.0f + K1 * ooz * y);

    idx = xp + yp * width;

    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main(void) {

    while (1) {
        printf("\x1b[2J");

        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);

        for (float cubeX = -cubewidth; cubeX < cubewidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubewidth; cubeY < cubewidth; cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubewidth, '.');
                calculateForSurface(cubewidth, cubeY, cubeX, '$');
            }
        }
        printf("\x1b[H");

        for (int k = 0; k < width * height; ++k) {
            putchar(k % width ? buffer[k] : 10);
        }

        A += 0.005;
        B += 0.005;

        // usleep(1000);
    }

    return 0;
}
