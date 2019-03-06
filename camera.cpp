#include "point.cpp"

class Camera {
public:
    Point position, direction;
    double speed, angularVelocity;
    double angleXZ = 0.0f, angleY = 0.0f;

    Camera(Point pos, Point dir, double speedMov, double angularVel) {
        position = pos;
        direction = dir;
        speed = speedMov;
        angularVelocity = angularVel;
    }

    void moveForward() {
        position.x += direction.x * speed;
        position.z += direction.z * speed;
    }

    void modeBackward() {
        position.x -= direction.x * speed;
        position.z -= direction.z * speed;
    }

    void moveLeft() {
        position.x += direction.z * speed;
        position.z -= direction.x * speed;
    }

    void moveRight() {
        position.x -= direction.z * speed;
        position.z += direction.x * speed;
    }

    void moveUp() {
        position.y += speed;
    }

    void moveDown() {
        if (position.y > 2 * speed)
            position.y -= speed;
    }

    void lookLeft() {
        angleXZ -= angularVelocity;
        direction.x = sin(angleXZ);
        direction.z = -cos(angleXZ);
    }

    void lookRight() {
        angleXZ += angularVelocity;
        direction.x = sin(angleXZ);
        direction.z = -cos(angleXZ);
    }

    void lookUp() {
        if (angleY < M_PI / 2 - angularVelocity)
            angleY += angularVelocity;
        direction.y = tan(angleY);
    }

    void lookDown() {
        if (angleY > -M_PI / 2 + angularVelocity)
            angleY -= angularVelocity;
        direction.y = tan(angleY);
    }
};