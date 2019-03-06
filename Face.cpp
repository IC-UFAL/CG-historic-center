#include "Point.h"
#include "Color.h"

class Face {
public:
    Point* points[4];
    Color* color;

    Face(Point* p1, Point* p2, Point* p3, Point* p4, Color* color) {
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        points[3] = p4;
        this->color = color;
    }

    void setColor(Color* color) {
        this->color = color;
    }
};
