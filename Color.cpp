#include "Color.h"

Color::Color() {
    R = G = B = 0;
}

Color::Color(double r, double g, double b) {
    R = r;
    G = g;
    B = b;
}

Color::Color(int r, int g, int b) {
    R = (r * 1.0) / 255;
    G = (g * 1.0) / 255;
    B = (b * 1.0) / 255;
}
