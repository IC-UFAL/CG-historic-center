#include <vector>

#include "Point.h"
#include "Color.h"

#ifndef HISTORIC_CENTER_FACE_H
#define HISTORIC_CENTER_FACE_H

using namespace std;

class Face {
public:
    vector<Point*> points;
    Color* color;

    Face(int vertices, Point* points[], Color* color);

    void setColor(Color* color);
};

#endif //HISTORIC_CENTER_FACE_H
