#include "Perlin.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <random>
using namespace std;

double interp1(double a, double b, double x) {

    double ft = x * 3.1415927;
    double f = (1.0 - cos(ft)) * 0.5;
    //return (b-x > b-1/2) ? b-x : a+x;
    return a * (1.0 - f) + b * f;
}

double smoothNoise(double x, double y) {
    double corners = (rand2(x - 1, y - 1) + rand2(x + 1, y - 1) + rand2(x - 1, y + 1) + rand2(x + 1, y + 1)) / 16;
    double sides = (rand2(x - 1, y) + rand2(x + 1, y) + rand2(x, y - 1) + rand2(x, y + 1)) / 8;
    double center = rand2(x, y) / 4;

    return corners + sides + center;
}



double lininterp1(double a, double b, double x) {
    return a * (1 - x) + b * x;
}
double rand2(double x, double y) {

    int n = (int)x + (int)y * 57;
    //n=pow((n<<13),n);
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);

}

double noise(double x, double y) {
    double floorX = (double)floor(x);
    double floorY = (double)floor(y);

    double s, t, u, v;

    s = smoothNoise(floorX, floorY);
    t = smoothNoise(floorX + 1, floorY);
    u = smoothNoise(floorY, floorY + 1);
    v = smoothNoise(floorX + 1, floorY + 1);

    double int1 = interp1(s, t, x - floorX);
    double int2 = interp1(u, v, x - floorX);

    return interp1(int1, int2, y - floorY);
}
