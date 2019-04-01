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
    float xDiff, yDiff, zDiff;
    int texture_id = -1, texDir;

    bool debug = false;

    Face(int vertices, Point* points[], Color* color);

    void setTextureId(int id);
};

#endif //HISTORIC_CENTER_FACE_H
