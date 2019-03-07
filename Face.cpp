#include "Point.h"
#include "Color.h"
#include "Face.h"

Face::Face(int vertices, Point* points[], Color* color) {
    for (int i = 0; i < vertices; i++) {
        this->points.push_back(points[i]);
    }
    this->color = color;
}

void Face::setColor(Color *color) {
    this->color = color;
}
