#include <vector>
#include <queue>

#include "Face.h"

using namespace std;

class Model {
public:
    vector<Face*> faces;
    queue<Point*> lastPoints;

    void addFace(Point* p1, Point* p2, Point* p3, Point* p4, Color* color) {
        faces.push_back(new Face(p1, p2, p3, p4, color));
        lastPoints.push(p3);
        lastPoints.push(p4);
    }

    void addFace(Point* p3, Point* p4, Color* color) {
        Point* p1 = lastPoints.front(); lastPoints.pop();
        Point* p2 = lastPoints.front(); lastPoints.pop();
        addFace(p1, p2, p3, p4, color);
    }
};
