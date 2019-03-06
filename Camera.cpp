#include <math.h>

#include "Point.h"
#include "Camera.h"

Camera::Camera(Point pos, Point dir, double speedMov, double angularVel) {
    position = pos;
    direction = dir;
    speed = speedMov;
    angularVelocity = angularVel;
}

void Camera::moveForward() {
    position.x += direction.x * speed;
    position.z += direction.z * speed;
}

void Camera::moveBackward() {
    position.x -= direction.x * speed;
    position.z -= direction.z * speed;
}

void Camera::moveLeft() {
    position.x += direction.z * speed;
    position.z -= direction.x * speed;
}

void Camera::moveRight() {
    position.x -= direction.z * speed;
    position.z += direction.x * speed;
}

void Camera::moveUp() {
    position.y += speed;
}

void Camera::moveDown() {
    if (position.y > 2 * speed)
        position.y -= speed;
}

void Camera::lookLeft() {
    angleXZ -= angularVelocity;
    direction.x = sin(angleXZ);
    direction.z = -cos(angleXZ);
}

void Camera::lookRight() {
    angleXZ += angularVelocity;
    direction.x = sin(angleXZ);
    direction.z = -cos(angleXZ);
}

void Camera::lookUp() {
    if (angleY < M_PI / 2 - angularVelocity)
        angleY += angularVelocity;
    direction.y = tan(angleY);
}

void Camera::lookDown() {
    if (angleY > -M_PI / 2 + angularVelocity)
        angleY -= angularVelocity;
    direction.y = tan(angleY);
}