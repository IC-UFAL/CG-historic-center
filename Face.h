#include <vector>

#include "Point.h"
#include "Color.h"

#ifndef HISTORIC_CENTER_FACE_H
#define HISTORIC_CENTER_FACE_H

using namespace std;

class Face {
public:
    vector<Point *> points, texPoints;
    Color *color;
    float xDiff, yDiff, zDiff, texScaleX = 1, texScaleY = 1;
    int texture_id = -1, texDir;

    bool hasTexPoints = false, debug = false;

    Face(int vertices, Point *points[], Color *color);

    void setTexturePoints(Point *points[]);

    void setTexture(int id, float texScaleX, float texScaleY);

    void setTexture(int id);
};

#endif //HISTORIC_CENTER_FACE_H
