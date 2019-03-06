#include "Point.h"

#ifndef HISTORIC_CENTER_CAMERA_H
#define HISTORIC_CENTER_CAMERA_H

class Camera {
public:
    Point position, direction;
    double speed, angularVelocity;
    double angleXZ, angleY;

    Camera(Point pos, Point dir, double speedMov, double angularVel);

    void moveForward();

    void modeBackward();

    void moveLeft();

    void moveRight();

    void moveUp();

    void moveDown();

    void lookLeft();

    void lookRight();

    void lookUp();

    void lookDown();
};

#endif //HISTORIC_CENTER_CAMERA_H
