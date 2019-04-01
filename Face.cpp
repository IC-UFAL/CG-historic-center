#include <cfloat>
#include <cstdio>
#include "Point.h"
#include "Color.h"
#include "Face.h"

Face::Face(int vertices, Point* points[], Color* color) {
    double xMin = FLT_MAX, xMax = -FLT_MAX, yMin = FLT_MAX, yMax = -FLT_MAX, zMin = FLT_MAX, zMax = -FLT_MAX;
    for (int i = 0; i < vertices; i++) {
        this->points.push_back(points[i]);

        if (points[i]->x < xMin)
            xMin = points[i]->x;
        else if (points[i]->x > xMax)
            xMax = points[i]->x;

        if (points[i]->y < yMin)
            yMin = points[i]->y;
        else if (points[i]->y > yMax)
            yMax = points[i]->y;

        if (points[i]->z < zMin)
            zMin = points[i]->z;
        else if (points[i]->z > zMax)
            zMax = points[i]->z;
    }
    this->color = color;
    this->xDiff = xMax - xMin;
    this->yDiff = yMax - yMin;
    this->zDiff = zMax - zMin;

    if (zDiff == 0) {
        texDir = (this->points[1]->y - this->points[0]->y) != 0 ? 1 : -1;
    } else if (yDiff == 0) {
        texDir = (this->points[1]->z - this->points[0]->z) != 0 ? 1 : -1;
    } else if (xDiff == 0) {
        texDir = (this->points[1]->y - this->points[0]->y) != 0 ? 1 : -1;
    }
}

void Face::setTextureId(int id) {
    this->texture_id = id;
}
