#include "Point.h"
#include "Color.h"

#ifndef HISTORIC_CENTER_FACE_H
#define HISTORIC_CENTER_FACE_H

class Face {
public:
    Point* points[4];
    Color* color;

    Face(Point* p1, Point* p2, Point* p3, Point* p4, Color* color);

    void setColor(Color* color);
};

#endif //HISTORIC_CENTER_FACE_H
