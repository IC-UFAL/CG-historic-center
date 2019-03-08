#include <queue>
#include <stack>

#include "Face.h"
#include "Cube.h"

#ifndef HISTORIC_CENTER_MODEL_H
#define HISTORIC_CENTER_MODEL_H

using namespace std;

class Model {
public:
    vector<Face *> faces;
    vector<Cube *> cubes;
    stack<Point *> lastPoints;

    void addRectFace(Point *p1, Point *p2, Point *p3, Point *p4, Color *color);

    void addRectFace(Point *p3, Point *p4, Color *color);

    void addTriangFace(Point *p1, Point *p2, Point *p3, Color *color);

    void addFace(Face *face);

    void addCube(Point *pos, float width, float height, float depth, Point *rotPoint, float rotAngle, int rotAxis[3],
                 Color *color);

    void addCube(Point *pos, float width, float height, float depth, Color *color);
};

#endif //HISTORIC_CENTER_MODEL_H
