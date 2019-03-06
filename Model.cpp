#include <vector>
#include <queue>

#include "Face.h"
#include "Model.h"

using namespace std;

void Model::addFace(Point *p1, Point *p2, Point *p3, Point *p4, Color *color) {
    while (!lastPoints.empty())
        lastPoints.pop();

    faces.push_back(new Face(p1, p2, p3, p4, color));
    lastPoints.push(p3);
    lastPoints.push(p4);
}

void Model::addFace(Point *p3, Point *p4, Color *color) {
    Point *p1 = lastPoints.top();
    lastPoints.pop();
    Point *p2 = lastPoints.top();
    lastPoints.pop();
    addFace(p1, p2, p3, p4, color);
}
