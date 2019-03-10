#include <vector>
#include <queue>

#include "Face.h"
#include "Model.h"

using namespace std;

Model::Model() {
    this->x = this->y = this->z = 0;
}

void Model::addRectFace(Point *p1, Point *p2, Point *p3, Point *p4, Color *color) {
    while (!lastPoints.empty())
        lastPoints.pop();

    Point *points[] = {p1, p2, p3, p4};
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

    Point *points[] = {p1, p2, p3};
    faces.push_back(new Face(3, points, color));
    lastPoints.push(p2);
    lastPoints.push(p3);
}

void Model::addFace(Face *face) {
    while (!lastPoints.empty())
        lastPoints.pop();

    faces.push_back(face);
}

void Model::addCube(Point *pos, float width, float height, float depth, Point *rotPoint, float rotAngle, int *rotAxis,
                    Color *color) {
    cubes.push_back(new Cube(pos, width, height, depth, rotPoint, rotAngle, rotAxis, color));
}

void Model::addCube(Point *pos, float width, float height, float depth, Color *color) {
    int axis[] = {0, 0, 0};
    addCube(pos, width, height, depth, new Point(width / 2, height / 2, depth / 2), 0, axis, color);
}

void Model::addCube(Point *pos, float width, float height, float depth, float rotAngle, int *rotAxis, Color *color) {
    addCube(pos, width, height, depth, new Point(width / 2, height / 2, depth / 2), rotAngle, rotAxis, color);
}

void Model::addCylinder(Point *pos, float base, float top, float height, float rotAngle, int rotAxis[3], Color *color) {
    cylinders.push_back(new Cylinder(pos, base, top, height, rotAngle, rotAxis, color));
}
