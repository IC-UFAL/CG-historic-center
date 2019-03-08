#include <vector>
#include <queue>

#include "Face.h"
#include "Model.h"

using namespace std;

void Model::addRectFace(Point *p1, Point *p2, Point *p3, Point *p4, Color *color) {
    while (!lastPoints.empty())
        lastPoints.pop();

    Point* points[] = {p1, p2, p3, p4};
    faces.push_back(new Face(4, points, color));
    lastPoints.push(p3);
    lastPoints.push(p4);
}

void Model::addRectFace(Point *p3, Point *p4, Color *color) {
    Point *p1 = lastPoints.top();
    lastPoints.pop();
    Point *p2 = lastPoints.top();
    lastPoints.pop();

    addRectFace(p1, p2, p3, p4, color);
}

void Model::addTriangFace(Point *p1, Point *p2, Point *p3, Color *color) {
    while (!lastPoints.empty())
        lastPoints.pop();

    Point* points[] = {p1, p2, p3};
    faces.push_back(new Face(3, points, color));
    lastPoints.push(p2);
    lastPoints.push(p3);
}

void Model::addFace(Face *face) {
    while (!lastPoints.empty())
        lastPoints.pop();

    faces.push_back(face);
}
