#include <queue>
#include <stack>

#include "Face.h"
#include "Cube.h"
#include "Cylinder.h"

#ifndef HISTORIC_CENTER_MODEL_H
#define HISTORIC_CENTER_MODEL_H

using namespace std;

class Model {
public:
    float x, y, z;
    vector<Face *> faces;
    vector<Cube *> cubes;
    vector<Cylinder *> cylinders;
    stack<Point *> lastPoints;

    Model();

    void addRectFace(Point *p1, Point *p2, Point *p3, Point *p4, Color *color);

    void addRectFace(Point *p3, Point *p4, Color *color);

    void addTriangFace(Point *p1, Point *p2, Point *p3, Color *color);

    void addFace(Face *face);

    void addCube(Point *pos, float width, float height, float depth, Point *rotPoint, float rotAngle, int rotAxis[3],
                 Color *color);

    void addCube(Point *pos, float width, float height, float depth, float rotAngle, int rotAxis[3], Color *color);

    void addCube(Point *pos, float width, float height, float depth, Color *color);

    void translate(float x, float y, float z);

    void addCylinder(Point *pos, float base, float top, float height, float rotAngle, int rotAxis[3], Color *color);
};

#endif //HISTORIC_CENTER_MODEL_H
