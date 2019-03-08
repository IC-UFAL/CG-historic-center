#include "Cube.h"

Cube::Cube(Point *pos, float width, float height, float depth, Point *rotPoint, float rotAngle, int rotAxis[3],
           Color *color) {
    position = pos;
    this->width = width;
    this->height = height;
    this->depth = depth;
    rotationPoint = rotPoint;
    rotationAngle = rotAngle;
    rotationAxis = rotAxis;
    this->color = color;
}
