#include "Point.h"
#include "Color.h"

#ifndef HISTORIC_CENTER_CYLINDER_H
#define HISTORIC_CENTER_CYLINDER_H


class Cylinder {
public:
    Point *position;
    int *rotationAxis;
    float base, height, top, rotationAngle;
    Color *color;

    Cylinder(Point *pos, float base, float top, float height, float rotAngle, int rotAxis[3], Color *color);

};


#endif //HISTORIC_CENTER_CYLINDER_H
