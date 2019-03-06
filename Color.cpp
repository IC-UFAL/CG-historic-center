#include "Color.h"

class Color {
public:
    double R, G, B;

    Color() {
        R = G = B = 0;
    }

    Color(double r, double g, double b) {
        R = r;
        G = g;
        B = b;
    }
};
