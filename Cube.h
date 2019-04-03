#include "Point.h"
#include "Color.h"

#ifndef HISTORIC_CENTER_CUBE_H
#define HISTORIC_CENTER_CUBE_H


class Cube {
public:
    Point *position, *rotationPoint;
    int *rotationAxis;
    float width, height, depth, rotationAngle;
    Color *color;
    int texture;

    Cube(Point *pos, float width, float height, float depth, Point *rotPoint, float rotAngle, int rotAxis[3], Color *color, int texture);
};


#endif //HISTORIC_CENTER_CUBE_H
