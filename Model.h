#include <queue>
#include <vector>

#include "Face.h"

#ifndef HISTORIC_CENTER_MODEL_H
#define HISTORIC_CENTER_MODEL_H

using namespace std;

class Model {
public:
    vector<Face*> faces;
    queue<Point*> lastPoints;

    void addFace(Point* p1, Point* p2, Point* p3, Point* p4, Color* color);

    void addFace(Point* p3, Point* p4, Color* color);
};

#endif //HISTORIC_CENTER_MODEL_H
