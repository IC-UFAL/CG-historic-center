#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Color.h"
#include "Point.h"
#include "Face.h"
#include "Model.h"
#include "Camera.h"

#define GLUT_KEY_SHIFT 112
#define COLOR_FOUNDATION new Color(230, 220, 170)
#define COLOR_EXTERNAL_WALL new Color(239, 228, 176)
#define COLOR_EXTERNAL_WALL_2 new Color(249, 238, 186)
#define COLOR_EXTERNAL_DETAILS new Color(255, 243, 191)
#define COLOR_FLOOR new Color(1.0, 1.0, 1.0)
#define COLOR_STAIRS_FRONT new Color(0.7, 0.7, 0.7)
#define COLOR_STAIRS_TOP new Color(0.55, 0.55, 0.55)
#define COLOR_DOOR new Color(77, 57, 36)
#define COLOR_STATIC_WINDOW new Color(100, 80, 60)

Camera *cam = new Camera(*(new Point(15, 10, 25)), *(new Point(0, tan(-0.05), -1)), 0.03, 0.001);
Model building;

static unsigned int redisplay_interval = 1000 / 60;

float door_angle = 0.0f;

bool keyPressed[256], specialKeyPressed[256];

void update(int);

void init() {
    int axisY[3] = {0, 1, 0}, axisZ[] = {0, 0, 1};
    // sky color
    glClearColor(0.0, 0.7, 1.0, 1.0);

    // Foundation
    building.addRectFace(new Point(-0.3, 0, 0.3), new Point(-0.3, 2, 0.3), new Point(7.8, 2, 0.3),
                         new Point(7.8, 0, 0.3), COLOR_FOUNDATION);
    building.addRectFace(new Point(7.8, 2, -1.5), new Point(7.8, 0, -1.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(22.2, 2, -1.5), new Point(22.2, 0, -1.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(22.2, 2, 0.3), new Point(22.2, 0, 0.3), COLOR_FOUNDATION);
    building.addRectFace(new Point(30.3, 2, 0.3), new Point(30.3, 0, 0.3), COLOR_FOUNDATION);
    building.addRectFace(new Point(30.3, 2, -15.3), new Point(30.3, 0, -15.3), COLOR_FOUNDATION);
    building.addRectFace(new Point(-0.3, 2, -15.3), new Point(-0.3, 0, -15.3), COLOR_FOUNDATION);
    building.addRectFace(new Point(-0.3, 2, 0.3), new Point(-0.3, 0, 0.3), COLOR_FOUNDATION);

    // Foundation floor
    building.addRectFace(new Point(-0.3, 2, 0.3), new Point(-0.3, 2, -15.3), new Point(7.8, 2, -15.3),
                         new Point(7.8, 2, 0.3), COLOR_FLOOR);
    building.addRectFace(new Point(7.8, 2, -1.5), new Point(7.8, 2, -15.3), new Point(22.2, 2, -15.3),
                         new Point(22.2, 2, -1.5), COLOR_FLOOR);
    building.addRectFace(new Point(22.2, 2, 0.3), new Point(22.2, 2, -15.3), new Point(30.3, 2, -15.3),
                         new Point(30.3, 2, 0.3), COLOR_FLOOR);

    // Second floor
    building.addRectFace(new Point(0.0, 5.0, 0.0), new Point(0.0, 5.0, -4.3), new Point(7.5, 5.0, -4.3),
                         new Point(7.5, 5.0, 0.0), COLOR_FLOOR);
    building.addRectFace(new Point(0.0, 5.0, -10.0), new Point(0.0, 5.0, -15.0), new Point(7.8, 5.0, -15.0),
                         new Point(7.8, 5.0, -10.0), COLOR_FLOOR);
    building.addRectFace(new Point(3.8, 5.0, -4.3), new Point(3.8, 5.0, -10.3), new Point(7.8, 5.0, -10.3),
                         new Point(7.8, 5.0, -4.3), COLOR_FLOOR);
    building.addRectFace(new Point(7.5, 5.0, -1.5), new Point(7.5, 5.0, -15.0), new Point(22.5, 5.0, -15.0),
                         new Point(22.5, 5.0, -1.5), COLOR_FLOOR);
    building.addRectFace(new Point(22.5, 5.0, 0.0), new Point(22.5, 5.0, -15.0), new Point(30.0, 5.0, -15.0),
                         new Point(30.0, 5.0, 0.0), COLOR_FLOOR);

    // External stairs
    building.addRectFace(new Point(10.5, 2, 0), new Point(19.5, 2, 0), new Point(19.5, 2, 0.35),
                         new Point(10.5, 2, 0.35), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(19.5, 1.75, 0.35), new Point(10.5, 1.75, 0.35), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(19.5, 1.75, 0.7), new Point(10.5, 1.75, 0.7), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(19.5, 1.5, 0.7), new Point(10.5, 1.5, 0.7), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(19.5, 1.5, 1.05), new Point(10.5, 1.5, 1.05), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(19.5, 1.25, 1.05), new Point(10.5, 1.25, 1.05), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(19.5, 1.25, 1.4), new Point(10.5, 1.25, 1.4), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(19.5, 1.0, 1.4), new Point(10.5, 1.0, 1.4), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(19.5, 1.0, 2.1), new Point(10.5, 1.0, 2.1), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(19.5, 0.75, 2.1), new Point(10.5, 0.75, 2.1), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(19.5, 0.75, 2.45), new Point(10.5, 0.75, 2.45), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(19.5, 0.5, 2.45), new Point(10.5, 0.5, 2.45), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(19.5, 0.5, 2.8), new Point(10.5, 0.5, 2.8), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(19.5, 0.25, 2.8), new Point(10.5, 0.25, 2.8), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(19.5, 0.25, 3.05), new Point(10.5, 0.25, 3.05), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(19.5, 0.0, 3.05), new Point(10.5, 0.0, 3.05), COLOR_STAIRS_FRONT);

    // Internal stairs
    building.addRectFace(new Point(3.8, 2, -7.3), new Point(3.8, 2, -10.3), new Point(3.8, 2.25, -10.3),
                         new Point(3.8, 2.25, -7.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(3.5, 2.25, -10.3), new Point(3.5, 2.25, -7.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(3.5, 2.5, -10.3), new Point(3.5, 2.5, -7.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(3.2, 2.5, -10.3), new Point(3.2, 2.5, -7.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(3.2, 2.75, -10.3), new Point(3.2, 2.75, -7.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(2.9, 2.75, -10.3), new Point(2.9, 2.75, -7.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(2.9, 3, -10.3), new Point(2.9, 3, -7.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(2.6, 3, -10.3), new Point(2.6, 3, -7.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(2.6, 3.25, -10.3), new Point(2.6, 3.25, -7.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(2.3, 3.25, -10.3), new Point(2.3, 3.25, -7.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(2.3, 3.5, -10.3), new Point(2.3, 3.5, -7.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(0.0, 3.5, -10.3), new Point(0.0, 3.5, -7.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(0.0, 3.5, -4.3), new Point(0.0, 3.5, -7.3), new Point(2.3, 3.5, -7.3),
                         new Point(2.3, 3.5, -4.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(2.3, 3.75, -7.3), new Point(2.3, 3.75, -4.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(2.6, 3.75, -7.3), new Point(2.6, 3.75, -4.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(2.6, 4.0, -7.3), new Point(2.6, 4.0, -4.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(2.9, 4.0, -7.3), new Point(2.9, 4.0, -4.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(2.9, 4.25, -7.3), new Point(2.9, 4.25, -4.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(3.2, 4.25, -7.3), new Point(3.2, 4.25, -4.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(3.2, 4.5, -7.3), new Point(3.2, 4.5, -4.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(3.5, 4.5, -7.3), new Point(3.5, 4.5, -4.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(3.5, 4.75, -7.3), new Point(3.5, 4.75, -4.3), COLOR_STAIRS_FRONT);
    building.addRectFace(new Point(3.8, 4.75, -7.3), new Point(3.8, 4.75, -4.3), COLOR_STAIRS_TOP);
    building.addRectFace(new Point(3.8, 5.0, -7.3), new Point(3.8, 5.0, -4.3), COLOR_STAIRS_FRONT);


    // Pillars bases
    building.addCube(new Point(10, 0, 2.5), 1, 1, 1, COLOR_EXTERNAL_WALL);
    building.addCube(new Point(19, 0, 2.5), 1, 1, 1, COLOR_EXTERNAL_WALL);
    building.addCube(new Point(10, 0, -0.5), 1, 2.5, 1, COLOR_EXTERNAL_WALL);
    building.addCube(new Point(13, 0, -0.5), 1, 2.5, 1, COLOR_EXTERNAL_WALL);
    building.addCube(new Point(16, 0, -0.5), 1, 2.5, 1, COLOR_EXTERNAL_WALL);
    building.addCube(new Point(19, 0, -0.5), 1, 2.5, 1, COLOR_EXTERNAL_WALL);

    // Handrail flaps
    building.addRectFace(new Point(10.35, 0, 2.5), new Point(10.35, 0, 0.5), new Point(10.35, 2.3, 0.5),
                         new Point(10.35, 0.8, 2.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(10.7, 2.3, 0.5), new Point(10.7, 0.8, 2.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(10.7, 0, 0.5), new Point(10.7, 0, 2.5), COLOR_FOUNDATION);

    building.addRectFace(new Point(19.35, 0, 2.5), new Point(19.35, 0, 0.5), new Point(19.35, 2.3, 0.5),
                         new Point(19.35, 0.8, 2.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(19.7, 2.3, 0.5), new Point(19.7, 0.8, 2.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(19.7, 0, 0.5), new Point(19.7, 0, 2.5), COLOR_FOUNDATION);

    building.addRectFace(new Point(10.35, 0, -0.5), new Point(10.35, 0, -1.5), new Point(10.35, 2.3, -1.5),
                         new Point(10.35, 2.3, -0.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(10.7, 2.3, -1.5), new Point(10.7, 2.3, -0.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(10.7, 2, -1.5), new Point(10.7, 2, -0.5), COLOR_FOUNDATION);

    building.addRectFace(new Point(19.35, 2, -0.5), new Point(19.35, 2, -1.5), new Point(19.35, 2.3, -1.5),
                         new Point(19.35, 2.3, -0.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(19.7, 2.3, -1.5), new Point(19.7, 2.3, -0.5), COLOR_FOUNDATION);
    building.addRectFace(new Point(19.7, 0, -1.5), new Point(19.7, 0, -0.5), COLOR_FOUNDATION);

    // Entrance floor
    building.addRectFace(new Point(10.5, 2, -1.5), new Point(10.5, 2, 0), new Point(19.5, 2, 0),
                         new Point(19.5, 2, -1.5), COLOR_FLOOR);

    // External Walls
    building.addRectFace(new Point(22.5, 2, -1.5), new Point(22.5, 10, -1.5), new Point(22.5, 10, 0),
                         new Point(22.5, 2, 0),
                         COLOR_EXTERNAL_WALL_2);
    building.addRectFace(new Point(30, 10, 0), new Point(30, 2, 0), COLOR_EXTERNAL_WALL);
    building.addRectFace(new Point(30, 10, -15), new Point(30, 2, -15), COLOR_EXTERNAL_WALL_2);
    building.addRectFace(new Point(0, 10, -15), new Point(0, 2, -15), COLOR_EXTERNAL_WALL);
    building.addRectFace(new Point(0, 10, 0), new Point(0, 2, 0), COLOR_EXTERNAL_WALL_2);
    building.addRectFace(new Point(7.5, 10, 0), new Point(7.5, 2, 0), COLOR_EXTERNAL_WALL);
    building.addRectFace(new Point(7.5, 10, -1.5), new Point(7.5, 2, -1.5), COLOR_EXTERNAL_WALL_2);
    building.addRectFace(new Point(14, 10, -1.5), new Point(14, 2, -1.5), COLOR_EXTERNAL_WALL);
    building.addRectFace(new Point(14, 5, -1.5), new Point(14, 10, -1.5), new Point(16, 10, -1.5),
                         new Point(16, 5, -1.5), COLOR_EXTERNAL_WALL_2);
    building.addRectFace(new Point(16, 2, -1.5), new Point(16, 10, -1.5), new Point(22.5, 10, -1.5),
                         new Point(22.5, 2, -1.5), COLOR_EXTERNAL_WALL);

    // Static front windows
    building.addCube(new Point(3, 2.5, -0.05), 1.5, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(25.5, 2.5, -0.05), 1.5, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(8.6, 2.5, -1.55), 0.9, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(20.5, 2.5, -1.55), 0.9, 2.5, 0.1, COLOR_STATIC_WINDOW);

    building.addCube(new Point(3, 6.5, -0.05), 1.5, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(25.5, 6.5, -0.05), 1.5, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(8.6, 6.5, -1.55), 0.9, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(11.4, 6.5, -1.55), 1.2, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(14.4, 6.5, -1.55), 1.2, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(17.4, 6.5, -1.55), 1.2, 2.5, 0.1, COLOR_STATIC_WINDOW);
    building.addCube(new Point(20.5, 6.5, -1.55), 0.9, 2.5, 0.1, COLOR_STATIC_WINDOW);

    // Static front doors
    building.addCube(new Point(11.2, 2, -1.55), 1.6, 3, 0.1, COLOR_DOOR);
    building.addCube(new Point(17.2, 2, -1.55), 1.6, 3, 0.1, COLOR_DOOR);

    // Static right windows
    building.addCube(new Point(29.95, 2.5, -2.5), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);
    building.addCube(new Point(29.95, 2.5, -7.1), 0.8, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);
    building.addCube(new Point(29.95, 2.5, -11), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);

    building.addCube(new Point(29.95, 6.5, -7.1), 0.8, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);
    building.addCube(new Point(29.95, 6.5, -2.5), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);
    building.addCube(new Point(29.95, 6.5, -11), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);

    // Static left windows
    building.addCube(new Point(-0.05, 2.5, -2.5), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);
    building.addCube(new Point(-0.05, 2.5, -7.1), 0.8, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);
    building.addCube(new Point(-0.05, 2.5, -11), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);

    building.addCube(new Point(-0.05, 6.5, -7.1), 0.8, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);
    building.addCube(new Point(-0.05, 6.5, -2.5), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);
    building.addCube(new Point(-0.05, 6.5, -11), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW);

    // Ceiling facade
    building.addRectFace(new Point(10.5, 10, 0), new Point(10.5, 11, 0), new Point(19.5, 11, 0), new Point(19.5, 10, 0),
                         COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(19.5, 11, -1.5), new Point(19.5, 10, -1.5), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(22, 11, -1.5), new Point(22.5, 10, -1.5), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(22, 11, 0.5), new Point(22.5, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(30.5, 11, 0.5), new Point(30, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(30.5, 11, -15.5), new Point(30, 10, -15), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(-0.5, 11, -15.5), new Point(0, 10, -15), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(-0.5, 11, 0.5), new Point(0, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(8, 11, 0.5), new Point(7.5, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(8, 11, -1.5), new Point(7.5, 10, -1.5), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(10.5, 11, -1.5), new Point(10.5, 10, -1.5), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(10.5, 11, 0), new Point(10.5, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.addRectFace(new Point(10.5, 10, 0), new Point(10.5, 10, -1.5), new Point(19.5, 10, -1.5),
                         new Point(19.5, 10, 0), COLOR_EXTERNAL_DETAILS);

    // Ceiling
    building.addCube(new Point(10, 11, -1.5), 10, 0.15, 2, COLOR_EXTERNAL_WALL);
    building.addCube(new Point(10, 11, -1.5), float(5 / 0.85), 0.15, 2, 30, axisZ, COLOR_EXTERNAL_WALL);
    building.addCube(new Point(20, 11, -1.5), float(5 / 0.85), 0.15, 2, new Point(-float(5 / 0.85) / 2, 0.075, 1), -30,
                     axisZ, COLOR_EXTERNAL_WALL);
    building.addTriangFace(new Point(10, 11, 0), new Point(15, 11 + 5 * 0.57735026919, 0), new Point(20, 11, 0),
                           COLOR_EXTERNAL_DETAILS);

//    glPushMatrix();
//    glColor3f(0, 0, 0);
//    glTranslatef(-5, 5, 0);
//    gluCylinder(gluNewQuadric(), 0.5, 0.5, 10, 8, 100);
//    glPopMatrix();
}

void drawCube(Point *p, float width, float height, float depth, Point *rotationPoint, float angle, int rotationAxis[3],
              Color *color) {
    glPushMatrix();
    glColor3f(GLfloat(color->R), GLfloat(color->G), GLfloat(color->B));
    glTranslatef(GLfloat(p->x), GLfloat(p->y), GLfloat(p->z));
    glRotatef(angle, rotationAxis[0], rotationAxis[1], rotationAxis[2]);
    glTranslatef(GLfloat(rotationPoint->x), GLfloat(rotationPoint->y), GLfloat(rotationPoint->z));
    glScalef(width, height, depth);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawBuilding() {
    for (auto &face : building.faces) {
        glColor3f(GLfloat(face->color->R), GLfloat(face->color->G), GLfloat(face->color->B));
        glBegin(GL_POLYGON);
        for (auto &point : face->points) {
            glVertex3f(GLfloat(point->x), GLfloat(point->y), GLfloat(point->z));
        }
        glEnd();
    }

    for (auto &cube : building.cubes) {
        glColor3f(GLfloat(cube->color->R), GLfloat(cube->color->G), GLfloat(cube->color->B));
        drawCube(cube->position, cube->width, cube->height, cube->depth, cube->rotationPoint, cube->rotationAngle,
                 cube->rotationAxis, cube->color);
        glEnd();
    }
}

void drawDoor() {
    auto *doorColor = COLOR_DOOR;
    int orientation[] = {0, 1, 0};
    // Left
    drawCube(new Point(14, 2, -1.5), 1, 3, 0.1, new Point(0.5, 1.5, 0.05), door_angle, orientation, doorColor);

    // Right
    orientation[1] = -1;
    drawCube(new Point(16, 2, -1.5), 1, 3, 0.1, new Point(-0.5, 1.5, 0.05), door_angle, orientation, doorColor);
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0) h = 1;
    double ratio = w * 1.0 / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void updateCamera() {
    if (keyPressed['w']) {
        cam->moveForward();
    }
    if (keyPressed['s']) {
        cam->moveBackward();
    }
    if (keyPressed['a']) {
        cam->moveLeft();
    }
    if (keyPressed['d']) {
        cam->moveRight();
    }
    if (keyPressed[' ']) {
        cam->moveUp();
    }
    if (specialKeyPressed[GLUT_KEY_SHIFT]) {
        cam->moveDown();
    }
    if (specialKeyPressed[GLUT_KEY_LEFT]) {
        cam->lookLeft();
    }
    if (specialKeyPressed[GLUT_KEY_RIGHT]) {
        cam->lookRight();
    }
    if (specialKeyPressed[GLUT_KEY_UP]) {
        cam->lookUp();
    }
    if (specialKeyPressed[GLUT_KEY_DOWN]) {
        cam->lookDown();
    }

}

void updateDoor() {
    if (keyPressed['o'])
        if (door_angle <= 118.0f)
            door_angle += 2.0f;
    if (keyPressed['c'])
        if (door_angle >= 2.0f)
            door_angle -= 2.0f;
}

void update(int) {
    updateCamera();
    updateDoor();

    glutPostRedisplay();
    glutTimerFunc(redisplay_interval, update, 0);
}

void renderScene() {
    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera
    updateCamera();
    gluLookAt(cam->position.x, cam->position.y, cam->position.z,
              cam->position.x + cam->direction.x, cam->position.y + cam->direction.y,
              cam->position.z + cam->direction.z,
              0.0f, 1.0f, 0.0f);

    // Draw ground
    glColor3f(0.0, 0.65, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    drawBuilding();

    updateDoor();
    drawDoor();

    glFlush();
    glutSwapBuffers();
}


void keyboardHandler(unsigned char key, int x, int y) {
    keyPressed[key] = true;

    if (key == 27)
        exit(0);
}

void keyboardUpHandler(unsigned char key, int x, int y) {
    keyPressed[key] = false;
}

void specialFuncHandler(int key, int x, int y) {
    specialKeyPressed[key] = true;
}

void specialFuncUpHandler(int key, int x, int y) {
    specialKeyPressed[key] = false;
}

int main(int argc, char **argv) {

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(800, 600);
    glutCreateWindow("CH - Associacao Comercial no Jaragua");

    init();

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(keyboardHandler);
    glutKeyboardUpFunc(keyboardUpHandler);
    glutSpecialFunc(specialFuncHandler);
    glutSpecialUpFunc(specialFuncUpHandler);


    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}
