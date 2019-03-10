#include "Cylinder.h"

Cylinder::Cylinder(Point *pos, float base, float top, float height, float rotAngle, int rotAxis[3], Color *color) {
    position = pos;
    this->base = base;
    this->top = top;
    this->height = height;
    rotationAngle = rotAngle;
    rotationAxis = rotAxis;
    this->color = color;
}