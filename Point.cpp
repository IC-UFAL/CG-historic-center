#include "Point.h"

Point::Point() {
    x = 0;
    y = 0;
    z = 0;
}

Point::Point(double x, double y, double z) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    this->z = static_cast<float>(z);
}

Point::Point(float x, float y) {
    this->x = x;
    this->y = y;
    this->z = 0;
}
