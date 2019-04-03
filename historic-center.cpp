#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Color.h"
#include "Point.h"
#include "Face.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"

#define GLUT_KEY_SHIFT 112
#define COLOR_FOUNDATION new Color(230, 220, 170)
#define COLOR_EXTERNAL_WALL new Color(237, 219, 165)
#define COLOR_EXTERNAL_WALL_2 new Color(227, 202, 153)
#define COLOR_EXTERNAL_DETAILS new Color(255, 243, 191)
#define COLOR_FLOOR new Color(208, 156, 98)
#define COLOR_STAIRS_FRONT new Color(0.7, 0.7, 0.7)
#define COLOR_STAIRS_TOP new Color(0.55, 0.55, 0.55)
#define COLOR_DOOR new Color(77, 57, 36)
#define COLOR_STATIC_WINDOW new Color(100, 80, 60)
#define COLOR_TABLE new Color(100, 57, 36)
#define COLOR_TABLE_TOP_SIDE new Color(100, 57, 50)
#define COLOR_TABLE_TOP new Color(179, 203, 229)
#define COLOR_TABLE_GOLD new Color(228, 220, 120)
#define COLOR_PILLAR new Color(255, 243, 202)
#define COLOR_PILLAR_BASE new Color(239, 228, 176)
#define COLOR_COUCH_WOOD new Color(82, 46, 30)
#define COLOR_COUCH_FOOT new Color(63, 35, 22)
#define COLOR_COUCH_PAD new Color(190, 203, 240)
#define COLOR_COUCH_PAD_SEAT new Color(174, 190, 236)
#define COLOR_CHAIR_WOOD new Color(100, 0, 0)

void initializeBuilding();

void checkSpecMode();

static unsigned int redisplay_interval = 1000 / 60;
static int axisY[3] = {0, 1, 0}, axisZ[3] = {0, 0, 1}, axisX[3] = {1, 0, 0};

Camera *cam = new Camera(*(new Point(12.5, 10, 25)), *(new Point(0, tan(-0.05), -1)), 0.1, 0.02);
Model building, doors, fancyTable, bigTable, chair, fancyChair, fancyCouch, ceiling;
Point *fancyChairSeats[2][4] = {
        {new Point(0, 0.92, 0.15), new Point(0, 0.92, 0.85), new Point(0.6, 0.92, 0.85), new Point(0.6, 0.92, 0.15)},
        {new Point(0, 1, 0.08),    new Point(0.6, 1, 0.08),  new Point(0.6, 1.9, 0.08),  new Point(0.0, 1.9, 0.08)}
};
GLuint textures[256];

float door_angle = 0.0f;
bool keyPressed[256], specialKeyPressed[256], enableLight = true, showCeiling = true;
GLfloat lightPosition[] = {12.5, 10, 20, 1};
GLfloat lightAmbient[] = {0.5, 0.5, 0.5};
GLfloat lightDiffuse[] = {0.3, 0.3, 0.3};
GLfloat lightSpecular[] = {0, 0, 0};

int specMode = 0;

float firstFloorHeight = 2, secondFloorHeight = 6;

void setupLight() {
//    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glShadeModel(GL_SMOOTH);
}

void light() {
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    GLfloat whiteMaterial[] = {1, 1, 1};
    GLfloat blankMaterial[] = {0.0, 0.0, 0.0};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, whiteMaterial);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, whiteMaterial);
    glPushMatrix();
    glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
    glColor3f(1, 1, 1);
    glutSolidSphere(0.1, 100, 100);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, blankMaterial);
}

void init() {
    setupLight();

    initializeBuilding();

    textures[0] = loadTexture("../textures/main_floor.bmp", 512, 512);
    textures[1] = loadTexture("../textures/chess_floor.bmp", 1104, 1104);
    textures[2] = loadTexture("../textures/fancy_couch_pad.bmp", 512, 512);
    textures[3] = loadTexture("../textures/main_wall.bmp", 256, 256);
    textures[4] = loadTexture("../textures/concrete_stair_top.bmp", 512, 512);
    textures[5] = loadTexture("../textures/concrete_stair_side.bmp", 512, 512);
    textures[6] = loadTexture("../textures/facade_name.bmp", 1024, 103);
    textures[7] = loadTexture("../textures/entrance_ceiling.bmp", 1024, 235);
    textures[8] = loadTexture("../textures/facade_side.bmp", 500, 800);
    textures[9] = loadTexture("../textures/foundation_side.bmp", 500, 200);
    textures[10] = loadTexture("../textures/foundation_main.bmp", 500, 200);
    textures[11] = loadTexture("../textures/table_gold.bmp", 256, 256);
    textures[12] = loadTexture("../textures/facade_side_ceil.bmp", 512, 102);
    textures[13] = loadTexture("../textures/facade_main_ceil.bmp", 512, 102);
    textures[14] = loadTexture("../textures/big_table_top.bmp", 280, 280);
    textures[15] = loadTexture("../textures/chair_pad.bmp", 500, 500);
    textures[16] = loadTexture("../textures/stair_marble.bmp", 512, 512);
    textures[17] = loadTexture("../textures/stair_wood.bmp", 512, 512);
    textures[18] = loadTexture("../textures/facade_front_ceil.bmp", 1024, 1024);
    textures[19] = loadTexture("../textures/chair_pad_yellow.bmp", 512, 512);
    textures[20] = loadTexture("../textures/window.bmp", 300, 500);
    textures[21] = loadTexture("../textures/window_slim.bmp", 180, 500);
    textures[22] = loadTexture("../textures/door.bmp", 500, 750);
    textures[23] = loadTexture("../textures/door_left.bmp", 250, 750);
    textures[24] = loadTexture("../textures/door_right.bmp", 250, 750);
    textures[25] = loadTexture("../textures/wood.bmp", 280, 280);
    textures[26] = loadTexture("../textures/clear_wood.bmp", 1000, 1000);
}

void drawCube(Point *p, float width, float height, float depth, Point *rotationPoint, float angle, int rotationAxis[3],
              Color *color, GLuint texture) {
    glPushMatrix();
    glColor3f(GLfloat(color->R), GLfloat(color->G), GLfloat(color->B));
    glTranslatef(GLfloat(p->x), GLfloat(p->y), GLfloat(p->z));
    glRotatef(angle, rotationAxis[0], rotationAxis[1], rotationAxis[2]);
    glTranslatef(GLfloat(rotationPoint->x), GLfloat(rotationPoint->y), GLfloat(rotationPoint->z));
    glScalef(width, height, depth);
    if (texture != -1) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[texture]);
    }

    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.03f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.03f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.03f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.03f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);  // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);  // Bottom Left Of The Texture and Quad
    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.5f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);  // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);  // Bottom Right Of The Texture and Quad
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);  // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);  // Top Left Of The Texture and Quad
    glEnd();
//    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//    glEnable(GL_TEXTURE_GEN_S);
//    glEnable(GL_TEXTURE_GEN_T);

//    glutSolidCube(1.0);
//    glDisable(GL_TEXTURE_GEN_T);
//    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawCylinder(Point *pos, float base, float top, float height, float rotAngle, int rotationAxis[3], Color *color) {
    glPushMatrix();
    glTranslatef(pos->x, pos->y, pos->z);
    glColor3f(GLfloat(color->R), GLfloat(color->G), GLfloat(color->B));
    glRotatef(rotAngle, rotationAxis[0], rotationAxis[1], rotationAxis[2]);
    gluCylinder(gluNewQuadric(), base, top, height, 50, 50);
    glPopMatrix();
}

void drawModel(const Model &model) {
    for (auto &face : model.faces) {
        if (face->texture_id != -1)
            glEnable(GL_TEXTURE_2D);

        if (face->debug)
            printf("xDiff: %f, yDiff: %f, zDiff: %f, points: %llu, texDir: %d\n", face->xDiff, face->yDiff, face->zDiff,
                   face->points.size(), face->texDir);


        float xTexScale, yTexScale;
        if (face->zDiff == 0) {
            xTexScale = face->xDiff;
            yTexScale = face->yDiff;
        } else if (face->yDiff == 0) {
            xTexScale = face->xDiff;
            yTexScale = face->zDiff;
        } else {
            xTexScale = face->zDiff;
            yTexScale = face->yDiff;
        }

        if (face->texDir == -1) {
            float aux = xTexScale;
            xTexScale = yTexScale;
            yTexScale = aux;
        }

        xTexScale *= 1 / face->texScaleX;
        yTexScale *= 1 / face->texScaleY;

        if (face->debug)
            printf("xScale: %f, yScale: %f\n", xTexScale, yTexScale);

        glBindTexture(GL_TEXTURE_2D, textures[face->texture_id]);
        glColor3f(GLfloat(face->color->R), GLfloat(face->color->G), GLfloat(face->color->B));

        glBegin(GL_POLYGON);
        int i = 0;
        for (auto &point : face->points) {
            glNormal3d(point->x, point->y, point->z);

            float xTex = i, yTex = i;
            if (face->hasTexPoints) {
                xTex = face->texPoints[i]->x;
                yTex = face->texPoints[i]->y;
            } else {
                if (face->points.size() == 3) {
                    xTex = i / 2.0f;
                    yTex = i == 1 ? 1 : 0;
                    xTex -= 0.05;
                } else if (face->points.size() == 4) {
                    xTex = i < 2 ? 0 : 1;
                    xTex += 0.05;
                    yTex = i == 0 || i == 3 ? 0 : 1;
                }
            }

            glTexCoord2d(xTex * xTexScale, yTex * yTexScale);
            glVertex3f(point->x, point->y, point->z);
            i++;
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    for (auto &cube : model.cubes) {
        glColor3f(GLfloat(cube->color->R), GLfloat(cube->color->G), GLfloat(cube->color->B));
        drawCube(cube->position, cube->width, cube->height, cube->depth, cube->rotationPoint, cube->rotationAngle,
                 cube->rotationAxis, cube->color, (GLuint) cube->texture);
    }

    for (auto &cylinder : model.cylinders) {
        drawCylinder(cylinder->position, cylinder->base, cylinder->top, cylinder->height, cylinder->rotationAngle,
                     cylinder->rotationAxis, cylinder->color);
    }
}

void drawDoors() {
    int factor = 1;
    for (auto &cube : doors.cubes) {
        glColor3f(GLfloat(cube->color->R), GLfloat(cube->color->G), GLfloat(cube->color->B));
        drawCube(cube->position, cube->width, cube->height, cube->depth, cube->rotationPoint, door_angle * factor,
                 cube->rotationAxis, cube->color, (GLuint) cube->texture);
        factor = -1;
    }
}

void drawFancyTable(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(1.2, 1, 1.2);
    drawModel(fancyTable);
    glPopMatrix();
}

void drawBigTable(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.8, 0.8, 0.8);
    drawModel(bigTable);
    glPopMatrix();
}

void drawChair(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.6, 0.6, 0.6);
    drawModel(chair);
    glPopMatrix();
}

void drawFancyChair(float x, float y, float z, float rotAngle, int rotAxis[3], float scaleX, float scaleY, float scaleZ,
                    Color *color, int texture_id) {
    Model seats;

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotAngle, rotAxis[0], rotAxis[1], rotAxis[2]);
    glScalef(scaleX, scaleY, scaleZ);
    drawModel(fancyChair);

    // Fancy seat
    seats.addRectFace(fancyChairSeats[0][0], fancyChairSeats[0][1], fancyChairSeats[0][2], fancyChairSeats[0][3],
                      color);
    seats.getLastFace()->setTexture(texture_id);

    // Fancy Backrest
    seats.addRectFace(fancyChairSeats[1][0], fancyChairSeats[1][1], fancyChairSeats[1][2], fancyChairSeats[1][3],
                      color);
    seats.getLastFace()->setTexture(texture_id);
    drawModel(seats);

    glPopMatrix();
}

void drawFancyCouch(float x, float y, float z, int rotationAxis[], float angle) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, rotationAxis[0], rotationAxis[1], rotationAxis[2]);
    glScalef(1.5, 1.5, 1.5);
    drawModel(fancyCouch);
    glPopMatrix();
}

void drawBuilding() {
    glPushMatrix();
    glTranslatef(building.x, building.y, building.z);

    drawModel(building);

    if (showCeiling)
        drawModel(ceiling);

    drawDoors();
    drawFancyTable(22.4, 6.01, -5);
    drawFancyCouch(20.1, 6.01, -6.8f, axisY, 90);
    drawBigTable(15.4f, 6, -2.5f);

    // simple chairs
    drawChair(16.5, 6, -8);
    drawChair(15.8, 6, -8);
    drawChair(15.1, 6, -8);
    drawChair(14.4, 6, -8);

    drawChair(13.2, 6, -8);
    drawChair(12.5, 6, -8);
    drawChair(11.8, 6, -8);
    drawChair(11.1, 6, -8);

    drawChair(9.9, 6, -8);
    drawChair(9.2, 6, -8);
    drawChair(8.5, 6, -8);
    drawChair(7.8, 6, -8);

    drawChair(16.5, 6, -9);
    drawChair(15.8, 6, -9);
    drawChair(15.1, 6, -9);
    drawChair(14.4, 6, -9);

    drawChair(13.2, 6, -9);
    drawChair(12.5, 6, -9);
    drawChair(11.8, 6, -9);
    drawChair(11.1, 6, -9);

    drawChair(9.9, 6, -9);
    drawChair(9.2, 6, -9);
    drawChair(8.5, 6, -9);
    drawChair(7.8, 6, -9);

    // fancy chairs
    drawFancyChair(14.8, 6, -2, 180.0, axisY, 0.8, 0.8, 0.8, COLOR_CHAIR_WOOD, 15);
    drawFancyChair(12.8, 6.3, -2, 180.0, axisY, 0.8, 0.8, 0.8, COLOR_CHAIR_WOOD, 15);
    drawFancyChair(10.6, 6, -2, 180.0, axisY, 0.8, 0.8, 0.8, COLOR_CHAIR_WOOD, 15);

    drawFancyChair(22.8, 6, -5.3f, 0.0, axisY, 0.7, 0.7, 0.7, COLOR_COUCH_PAD, 2);
    drawFancyChair(23.2, 6, -2.1f, 180.0, axisY, 0.7, 0.7, 0.7, COLOR_COUCH_PAD, 2);
    drawFancyChair(24.1, 6, -4.2f, -90.0f, axisY, 0.7, 0.7, 0.7, COLOR_COUCH_PAD, 2);
    drawFancyChair(22, 6, -3.3f, 90.0, axisY, 0.7, 0.7, 0.7, COLOR_COUCH_PAD, 2);

    glPopMatrix();
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
    // Lights
    if (keyPressed['l']) {
        if (keyPressed['w']) {
            lightPosition[2] -= 0.05;
        }
        if (keyPressed['s']) {
            lightPosition[2] += 0.05;
        }
        if (keyPressed['a']) {
            lightPosition[0] -= 0.05;
        }
        if (keyPressed['d']) {
            lightPosition[0] += 0.05;
        }
        if (keyPressed[' ']) {
            lightPosition[1] += 0.05;
        }
        if (specialKeyPressed[GLUT_KEY_SHIFT]) {
            lightPosition[1] -= 0.05;
        }

        if (specialKeyPressed[GLUT_KEY_F1]) {
            if (lightAmbient[0] < 1)
                lightAmbient[0] = lightAmbient[1] = lightAmbient[2] += 0.05;
        } else if (keyPressed['1']) {
            if (lightAmbient[0] > 0)
                lightAmbient[0] = lightAmbient[1] = lightAmbient[2] -= 0.05;
        } else if (specialKeyPressed[GLUT_KEY_F2]) {
            if (lightDiffuse[0] < 1)
                lightDiffuse[0] = lightDiffuse[1] = lightDiffuse[2] += 0.05;
        } else if (keyPressed['2']) {
            if (lightDiffuse[0] > 0)
                lightDiffuse[0] = lightDiffuse[1] = lightDiffuse[2] -= 0.05;
        } else if (specialKeyPressed[GLUT_KEY_F3]) {
            if (lightSpecular[0] < 1)
                lightSpecular[0] = lightSpecular[1] = lightSpecular[2] += 0.05;
        } else if (keyPressed['3']) {
            if (lightSpecular[0] > 0)
                lightSpecular[0] = lightSpecular[1] = lightSpecular[2] -= 0.05;
        }
    } else {
        // Movement
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
            if (specMode == 0)
                cam->moveUp();
        }
        if (specialKeyPressed[GLUT_KEY_SHIFT]) {
            if (specMode == 0)
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
}

void updateDoor() {
    if (keyPressed['o'])
        if (door_angle <= 118.0f)
            door_angle += 2.0f;
    if (keyPressed['c'])
        if (door_angle >= 2.0f)
            door_angle -= 2.0f;
}

void update() {
    updateCamera();
    updateDoor();
}

void renderScene(int) {
    update();

    // sky color
    glClearColor(0.0, 0.7, 1.0, 1.0);

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera
    gluLookAt(cam->position.x, cam->position.y, cam->position.z,
              cam->position.x + cam->direction.x, cam->position.y + cam->direction.y,
              cam->position.z + cam->direction.z,
              0.0f, 1.0f, 0.0f);

    // Set the light
    light();

    // Draw ground
    glColor3f(0.0, 0.65, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    drawBuilding();

    glFlush();
    glutSwapBuffers();
    glutTimerFunc(redisplay_interval, renderScene, 0);
}

void renderScene() {
    renderScene(0);
}

void checkSpecMode() {
    if (specMode == 1) {
        cam->position.y = firstFloorHeight + 1.8f;
    } else if (specMode == 2) {
        cam->position.y = secondFloorHeight + 1.8f;
    }
}

void keyboardHandler(unsigned char key, int, int) {
    keyPressed[tolower(key)] = true;

        printf("%d\n", key);
    if (key == 27)
        exit(0);
    else if (key == 13) {
        enableLight = !enableLight;
        if (enableLight)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);
    } else if (key == 9) {
        specMode = !specMode;

        checkSpecMode();
    } else if (key == ' ' && specMode != 0) {
        if (specMode == 1)
            specMode = 2;
        else if (specMode == 2)
            specMode = 1;

        checkSpecMode();
    } else if (tolower(key) == 'c') {
        showCeiling = !showCeiling;
    }
}

void keyboardUpHandler(unsigned char key, int, int) {
    keyPressed[tolower(key)] = false;
    if (key == 'q' && cam->speed > 0.03) {
        cam->speed -= 0.01;
    }
    if (key == 'e' && cam->speed < 0.2) {
        cam->speed += 0.01;
    }
}

void specialFuncHandler(int key, int, int) {
    specialKeyPressed[key] = true;
}

void specialFuncUpHandler(int key, int, int) {
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
//    glutIdleFunc(update);
    glutTimerFunc(redisplay_interval, renderScene, 0);
    glutKeyboardFunc(keyboardHandler);
    glutKeyboardUpFunc(keyboardUpHandler);
    glutSpecialFunc(specialFuncHandler);
    glutSpecialUpFunc(specialFuncUpHandler);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}

void initializeBuilding() {
    // Foundation
    building.addRectFace(new Point(-0.3, 0, 0.3), new Point(-0.3, 2, 0.3), new Point(5.3, 2, 0.3),
                         new Point(5.3, 0, 0.3), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(9, 6, 2);
    building.addRectFace(new Point(5.3, 2, -1.5), new Point(5.3, 0, -1.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(10, 6, 2);
    building.addRectFace(new Point(19.7, 2, -1.5), new Point(19.7, 0, -1.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(10, 6, 2);
    building.addRectFace(new Point(19.7, 2, 0.3), new Point(19.7, 0, 0.3), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(10, 6, 2);
    building.addRectFace(new Point(25.3, 2, 0.3), new Point(25.3, 0, 0.3), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(9, 6, 2);
    building.addRectFace(new Point(25.3, 2, -10.3), new Point(25.3, 0, -10.3), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(10, 6, 2);
    building.addRectFace(new Point(-0.3, 2, -10.3), new Point(-0.3, 0, -10.3), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(10, 6, 2);
    building.addRectFace(new Point(-0.3, 2, 0.3), new Point(-0.3, 0, 0.3), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(10, 6, 2);

    // Foundation floor
    building.addRectFace(new Point(-0.3, 2, 0.3), new Point(-0.3, 2, -10.3), new Point(5.3, 2, -10.3),
                         new Point(5.3, 2, 0.3), COLOR_FLOOR);
    building.getLastFace()->setTexture(0);
    building.addRectFace(new Point(5.3, 2, -1.5), new Point(5.3, 2, -10.3), new Point(19.7, 2, -10.3),
                         new Point(19.7, 2, -1.5), COLOR_FLOOR);
    building.getLastFace()->setTexture(0);
    building.addRectFace(new Point(19.7, 2, 0.3), new Point(19.7, 2, -10.3), new Point(25.3, 2, -10.3),
                         new Point(25.3, 2, 0.3), COLOR_FLOOR);
    building.getLastFace()->setTexture(0);

    // Second floor
    building.addRectFace(new Point(0.0, 6, 0.0), new Point(0.0, 6, -4.3), new Point(5, 6, -4.3),
                         new Point(5, 6, 0.0), COLOR_FLOOR);
    building.getLastFace()->setTexture(0);
    building.addRectFace(new Point(5, 6, -1.5), new Point(5, 6, -10), new Point(20, 6, -10),
                         new Point(20, 6, -1.5), COLOR_FLOOR);
    building.getLastFace()->setTexture(0);
    building.addRectFace(new Point(20, 6, 0.0), new Point(20, 6, -10), new Point(25, 6, -10),
                         new Point(25, 6, 0.0), COLOR_FLOOR);
    building.getLastFace()->setTexture(1);

    // First floor internal ceiling
    building.addRectFace(new Point(0.0, 5.99, 0.0), new Point(0.0, 5.99, -4.3), new Point(5, 5.99, -4.3),
                         new Point(5, 5.99, 0.0), COLOR_FLOOR);
    building.addRectFace(new Point(5, 5.99, -1.5), new Point(5, 5.99, -10), new Point(20, 5.99, -10),
                         new Point(20, 5.99, -1.5), COLOR_FLOOR);
    building.addRectFace(new Point(20, 5.99, 0.0), new Point(20, 5.99, -10), new Point(25, 5.99, -10),
                         new Point(25, 5.99, 0.0), COLOR_FLOOR);

    // Second floor internal ceiling
    ceiling.addRectFace(new Point(0.0, 9.99, 0.0), new Point(0.0, 9.99, -10), new Point(5, 9.99, -10),
                         new Point(5, 9.99, 0.0), COLOR_FLOOR);
    ceiling.addRectFace(new Point(5, 9.99, -1.5), new Point(5, 9.99, -10), new Point(20, 9.99, -10),
                         new Point(20, 9.99, -1.5), COLOR_FLOOR);
    ceiling.addRectFace(new Point(20, 9.99, 0.0), new Point(20, 9.99, -10), new Point(25, 9.99, -10),
                         new Point(25, 9.99, 0.0), COLOR_FLOOR);

    // External stairs
    building.addRectFace(new Point(8, 2, 0), new Point(17, 2, 0), new Point(17, 2, 0.35),
                         new Point(8, 2, 0.35), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(4);
    building.addRectFace(new Point(17, 1.75, 0.35), new Point(8, 1.75, 0.35), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(5);
    building.addRectFace(new Point(17, 1.75, 0.7), new Point(8, 1.75, 0.7), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(4);
    building.addRectFace(new Point(17, 1.5, 0.7), new Point(8, 1.5, 0.7), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(5);
    building.addRectFace(new Point(17, 1.5, 1.05), new Point(8, 1.5, 1.05), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(4);
    building.addRectFace(new Point(17, 1.25, 1.05), new Point(8, 1.25, 1.05), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(5);
    building.addRectFace(new Point(17, 1.25, 1.4), new Point(8, 1.25, 1.4), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(4);
    building.addRectFace(new Point(17, 1.0, 1.4), new Point(8, 1.0, 1.4), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(5);
    building.addRectFace(new Point(17, 1.0, 2.1), new Point(8, 1.0, 2.1), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(4);
    building.addRectFace(new Point(17, 0.75, 2.1), new Point(8, 0.75, 2.1), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(5);
    building.addRectFace(new Point(17, 0.75, 2.45), new Point(8, 0.75, 2.45), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(4);
    building.addRectFace(new Point(17, 0.5, 2.45), new Point(8, 0.5, 2.45), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(5);
    building.addRectFace(new Point(17, 0.5, 2.8), new Point(8, 0.5, 2.8), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(4);
    building.addRectFace(new Point(17, 0.25, 2.8), new Point(8, 0.25, 2.8), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(5);
    building.addRectFace(new Point(17, 0.25, 3.05), new Point(8, 0.25, 3.05), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(4);
    building.addRectFace(new Point(17, 0.0, 3.05), new Point(8, 0.0, 3.05), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(5);

    // Internal stairs
    building.addRectFace(new Point(3.8, 2, -7), new Point(3.8, 2, -10), new Point(3.8, 2.25, -10),
                         new Point(3.8, 2.25, -7), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(3.5, 2.25, -10), new Point(3.5, 2.25, -7), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(3.5, 2.5, -10), new Point(3.5, 2.5, -7), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(3.2, 2.5, -10), new Point(3.2, 2.5, -7), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(3.2, 2.75, -10), new Point(3.2, 2.75, -7), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(2.9, 2.75, -10), new Point(2.9, 2.75, -7), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(2.9, 3, -10), new Point(2.9, 3, -7), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(2.6, 3, -10), new Point(2.6, 3, -7), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(2.6, 3.25, -10), new Point(2.6, 3.25, -7), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(2.3, 3.25, -10), new Point(2.3, 3.25, -7), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(2.3, 3.5, -10), new Point(2.3, 3.5, -7), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(0.0, 3.5, -10), new Point(0.0, 3.5, -7), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(0.0, 3.5, -4.3), new Point(0.0, 3.5, -7), new Point(2.3, 3.5, -7),
                         new Point(2.3, 3.5, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(2.3, 3.75, -7), new Point(2.3, 3.75, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(2.6, 3.75, -7), new Point(2.6, 3.75, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(2.6, 4.0, -7), new Point(2.6, 4.0, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(2.9, 4.0, -7), new Point(2.9, 4.0, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(2.9, 4.25, -7), new Point(2.9, 4.25, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(3.2, 4.25, -7), new Point(3.2, 4.25, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(3.2, 4.5, -7), new Point(3.2, 4.5, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(3.5, 4.5, -7), new Point(3.5, 4.5, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(3.5, 4.75, -7), new Point(3.5, 4.75, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(3.8, 4.75, -7), new Point(3.8, 4.75, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(3.8, 5.0, -7), new Point(3.8, 5.0, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(4.1, 5.0, -7), new Point(4.1, 5.0, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(4.1, 5.25, -7), new Point(4.1, 5.25, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(4.4, 5.25, -7), new Point(4.4, 5.25, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(4.4, 5.5, -7), new Point(4.4, 5.5, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(4.7, 5.5, -7), new Point(4.7, 5.5, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(4.7, 5.75, -7), new Point(4.7, 5.75, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);
    building.addRectFace(new Point(5.0, 5.75, -7), new Point(5.0, 5.75, -4.3), COLOR_STAIRS_TOP);
    building.getLastFace()->setTexture(16);
    building.addRectFace(new Point(5.0, 6.0, -7), new Point(5.0, 6.0, -4.3), COLOR_STAIRS_FRONT);
    building.getLastFace()->setTexture(17);

    // Pillars bases
    building.addCube(new Point(7.5, 0, 2.5), 1, 1, 1, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(16.5, 0, 2.5), 1, 1, 1, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(7.5, 0, -0.5), 1, 2.5, 1, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(10.5, 0, -0.5), 1, 2.5, 1, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(13.5, 0, -0.5), 1, 2.5, 1, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(16.5, 0, -0.5), 1, 2.5, 1, COLOR_PILLAR_BASE, 3);

    building.addCube(new Point(7.7, 0.3, -5.3), 0.6, 2.3, 0.6, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(10.7, 0.3, -5.3), 0.6, 2.3, 0.6, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(13.7, 0.3, -5.3), 0.6, 2.3, 0.6, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(16.7, 0.3, -5.3), 0.6, 2.3, 0.6, COLOR_PILLAR_BASE, 3);

    // Pillars top
    building.addCube(new Point(7.5, 9.497, -0.5), 1, 0.5, 1, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(10.5, 9.497, -0.5), 1, 0.5, 1, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(13.5, 9.497, -0.5), 1, 0.5, 1, COLOR_PILLAR_BASE, 3);
    building.addCube(new Point(16.5, 9.497, -0.5), 1, 0.5, 1, COLOR_PILLAR_BASE, 3);

    // External pillars
    building.addCylinder(new Point(11.0, 2.0, 0.0), 0.4, 0.4, 8.0, -90.0f, axisX, COLOR_PILLAR);
    building.addCylinder(new Point(14.0, 2.0, 0.0), 0.4, 0.4, 8.0, -90.0f, axisX, COLOR_PILLAR);
    building.addCylinder(new Point(8.0, 2.0, 0.0), 0.4, 0.4, 8.0, -90.0f, axisX, COLOR_PILLAR);
    building.addCylinder(new Point(17.0, 2.0, 0.0), 0.4, 0.4, 8.0, -90.0f, axisX, COLOR_PILLAR);

    // Internal pillars
    building.addCylinder(new Point(11.0, 2.0, -5.0), 0.2, 0.2, 4.0, -90.0f, axisX, COLOR_PILLAR);
    building.addCylinder(new Point(14.0, 2.0, -5.0), 0.2, 0.2, 4.0, -90.0f, axisX, COLOR_PILLAR);
    building.addCylinder(new Point(8.0, 2.0, -5.0), 0.2, 0.2, 4.0, -90.0f, axisX, COLOR_PILLAR);
    building.addCylinder(new Point(17.0, 2.0, -5.0), 0.2, 0.2, 4.0, -90.0f, axisX, COLOR_PILLAR);

    // Handrail flaps
    building.addRectFace(new Point(7.85, 0, 2.5), new Point(7.85, 0, 0.5), new Point(7.85, 2.3, 0.5),
                         new Point(7.85, 0.8, 2.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(8.2, 2.3, 0.5), new Point(8.2, 0.8, 2.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(8.2, 0, 0.5), new Point(8.2, 0, 2.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);

    building.addRectFace(new Point(16.85, 0, 2.5), new Point(16.85, 0, 0.5), new Point(16.85, 2.3, 0.5),
                         new Point(16.85, 0.8, 2.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(17.2, 2.3, 0.5), new Point(17.2, 0.8, 2.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(17.2, 0, 0.5), new Point(17.2, 0, 2.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);

    building.addRectFace(new Point(7.85, 0, -0.5), new Point(7.85, 0, -1.5), new Point(7.85, 2.3, -1.5),
                         new Point(7.85, 2.3, -0.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(8.2, 2.3, -1.5), new Point(8.2, 2.3, -0.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(8.2, 2, -1.5), new Point(8.2, 2, -0.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);

    building.addRectFace(new Point(16.85, 2, -0.5), new Point(16.85, 2, -1.5), new Point(16.85, 2.3, -1.5),
                         new Point(16.85, 2.3, -0.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(17.2, 2.3, -1.5), new Point(17.2, 2.3, -0.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(17.2, 0, -1.5), new Point(17.2, 0, -0.5), COLOR_FOUNDATION);
    building.getLastFace()->setTexture(3);

    // Entrance floor
    building.addRectFace(new Point(8, 2, -1.5), new Point(8, 2, 0), new Point(17, 2, 0),
                         new Point(17, 2, -1.5), COLOR_FLOOR);
    building.getLastFace()->setTexture(0);

    // External Walls
    building.addRectFace(new Point(20, 2, -1.5), new Point(20, 10, -1.5), new Point(20, 10, 0),
                         new Point(20, 2, 0), COLOR_EXTERNAL_WALL_2);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(25, 10, 0), new Point(25, 2, 0), COLOR_EXTERNAL_WALL);
    building.getLastFace()->setTexture(8, 5, 8);
    building.addRectFace(new Point(25, 10, -10), new Point(25, 2, -10), COLOR_EXTERNAL_WALL_2);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(0, 10, -10), new Point(0, 2, -10), COLOR_EXTERNAL_WALL);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(0, 10, 0), new Point(0, 2, 0), COLOR_EXTERNAL_WALL_2);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(5, 10, 0), new Point(5, 2, 0), COLOR_EXTERNAL_WALL);
    building.getLastFace()->setTexture(8, 5, 8);
    building.addRectFace(new Point(5, 10, -1.5), new Point(5, 2, -1.5), COLOR_EXTERNAL_WALL_2);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(11.5, 10, -1.5), new Point(11.5, 2, -1.5), COLOR_EXTERNAL_WALL);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(11.5, 5, -1.5), new Point(11.5, 10, -1.5), new Point(13.5, 10, -1.5),
                         new Point(13.5, 5, -1.5), COLOR_EXTERNAL_WALL_2);
    building.getLastFace()->setTexture(3);
    building.addRectFace(new Point(13.5, 2, -1.5), new Point(13.5, 10, -1.5), new Point(20, 10, -1.5),
                         new Point(20, 2, -1.5), COLOR_EXTERNAL_WALL);
    building.getLastFace()->setTexture(3);

    // Internal second floor wall
    building.addRectFace(new Point(20, 6.0, -1.5), new Point(20, 6.0, -10), new Point(20, 10, -10),
                         new Point(20, 10, -1.5), COLOR_EXTERNAL_WALL_2);
    building.getLastFace()->setTexture(3);

    // Static front windows
    building.addCube(new Point(1.72, 2.71, -0.05), 1.5, 2.1, 0.1, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(21.72, 2.71, -0.05), 1.5, 2.1, 0.1, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(6.1, 2.5, -1.55), 0.9, 2.5, 0.1, COLOR_STATIC_WINDOW, 21);
    building.addCube(new Point(18, 2.5, -1.55), 0.9, 2.5, 0.1, COLOR_STATIC_WINDOW, 21);

    building.addCube(new Point(1.77, 6.5, -0.05), 1.4, 1.96, 0.1, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(21.77, 6.5, -0.05), 1.4, 1.96, 0.1, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(6.1, 6.5, -1.55), 0.9, 2.5, 0.1, COLOR_STATIC_WINDOW, 21);
    building.addCube(new Point(8.9, 6.5, -1.55), 1.2, 2.5, 0.1, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(11.9, 6.5, -1.55), 1.2, 2.5, 0.1, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(14.9, 6.5, -1.55), 1.2, 2.5, 0.1, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(18, 6.5, -1.55), 0.9, 2.5, 0.1, COLOR_STATIC_WINDOW, 21);

    // Static front doors
    building.addCube(new Point(8.7, 2, -1.55), 1.6, 3, 0.1, COLOR_DOOR, 22);
    building.addCube(new Point(14.7, 2, -1.55), 1.6, 3, 0.1, COLOR_DOOR, 22);

    // Static second floor doors
    building.addCube(new Point(19.95, 6.01, -2), 1.6, 3, 0.1, 90, axisY, COLOR_DOOR, 22);
    building.addCube(new Point(19.95, 6.01, -5), 1.6, 3, 0.1, 90, axisY, COLOR_DOOR, 22);

    // Static right windows
    building.addCube(new Point(24.95, 2.5, -1.55), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(24.95, 2.5, -4.6), 0.8, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 21);
    building.addCube(new Point(24.95, 2.5, -6.95), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 20);

    building.addCube(new Point(24.95, 6.5, -1.55), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(24.95, 6.5, -4.6), 0.8, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 21);
    building.addCube(new Point(24.95, 6.5, -6.95), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 20);

    // Static left windows
    building.addCube(new Point(-0.05, 2.5, -1.55), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(-0.05, 2.5, -4.6), 0.8, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 21);
    building.addCube(new Point(-0.05, 2.5, -6.95), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 20);

    building.addCube(new Point(-0.05, 6.5, -1.55), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 20);
    building.addCube(new Point(-0.05, 6.5, -4.6), 0.8, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 21);
    building.addCube(new Point(-0.05, 6.5, -6.95), 1.5, 2.5, 0.1, 90, axisY, COLOR_STATIC_WINDOW, 20);

    // Ceiling facade
    building.addRectFace(new Point(8, 10, 0.75), new Point(8, 11, 0.75), new Point(17, 11, 0.75),
                         new Point(17, 10, 0.75), COLOR_PILLAR_BASE);
    building.getLastFace()->setTexture(6, 9, 1);
    building.addRectFace(new Point(17, 11, -1.5), new Point(17, 10, -1.5), COLOR_PILLAR_BASE);
    building.getLastFace()->setTexture(13, 2, 1);
    building.addRectFace(new Point(19.5, 11, -1.5), new Point(20, 10, -1.5), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(13, 3, 1);
    building.addRectFace(new Point(19.5, 11, 0.5), new Point(20, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(13, 3, 1);
    building.addRectFace(new Point(25.5, 11, 0.5), new Point(25, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(12, 0.5, 1);
    building.addRectFace(new Point(25.5, 11, -10.5), new Point(25, 10, -10), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(13, 5, 1);
    building.addRectFace(new Point(-0.5, 11, -10.5), new Point(0, 10, -10), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(13, 0.11, 1);
    building.addRectFace(new Point(-0.5, 11, 0.5), new Point(0, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(13, 5, 1);
    building.addRectFace(new Point(5.5, 11, 0.5), new Point(5, 10, 0), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(12, 0.5, 1);
    building.addRectFace(new Point(5.5, 11, -1.5), new Point(5, 10, -1.5), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(13, 3, 1);
    building.addRectFace(new Point(8, 11, -1.5), new Point(8, 10, -1.5), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(13, 3, 1);
    building.addRectFace(new Point(8, 11, 0.75), new Point(8, 10, 0.75), COLOR_PILLAR_BASE);
    building.getLastFace()->setTexture(13, 2, 1);
    building.addRectFace(new Point(8, 10, 0.75), new Point(8, 10, -1.5), new Point(17, 10, -1.5),
                         new Point(17, 10, 0.75), COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(7, 8.5, 2);

    // Ceiling
    building.addCube(new Point(7.5, 11, -1.5), 10, 0.15, 3, COLOR_EXTERNAL_WALL, 3);
    building.addCube(new Point(7.5, 11, -1.5), float(5 / 0.85), 0.15, 3, 30, axisZ, COLOR_EXTERNAL_WALL, 3);
    building.addCube(new Point(17.5, 11, -1.5), float(5 / 0.85), 0.15, 3, new Point(-float(5 / 0.85) / 2, 0.075, 1.5),
                     -30, axisZ, COLOR_EXTERNAL_WALL, 3);
    building.addTriangFace(new Point(7.5, 11, 1), new Point(12.5, 11 + 5 * 0.57735026919, 1), new Point(17.5, 11, 1),
                           COLOR_EXTERNAL_DETAILS);
    building.getLastFace()->setTexture(18, 9, 3);
    building.getLastFace()->setTexture(18, 9, 3);

    ///--------------------------------------------- Moving Doors ---------------------------------------------------///
    doors.addCube(new Point(11.5, 2, -1.5), 1, 3, 0.1, new Point(0.5, 1.5, 0.05), 0, axisY, COLOR_DOOR, 23);
    doors.addCube(new Point(13.5, 2, -1.5), 1, 3, 0.1, new Point(-0.5, 1.5, 0.05), 0, axisY, COLOR_DOOR, 24);

    ///-------------------------------------------- Fancy Table -----------------------------------------------------///
    // Back Left Leg
    fancyTable.addRectFace(new Point(0.03, 0, 0.03), new Point(0, 0.15, 0),
                           new Point(0.1, 0.15, 0), new Point(0.07, 0, 0.03), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0.1, 0.15, 0.1), new Point(0.07, 0, 0.07), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0, 0.15, 0.1), new Point(0.03, 0, 0.07), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0, 0.15, 0), new Point(0.03, 0, 0.03), COLOR_TABLE);
    fancyTable.addCube(new Point(0, 0.15, 0), 0.1, 0.65, 0.1, COLOR_TABLE, 25);

    // Back Right Leg
    fancyTable.addRectFace(new Point(0.93, 0, 0.03), new Point(0.9, 0.15, 0),
                           new Point(1, 0.15, 0), new Point(0.97, 0, 0.03), COLOR_TABLE);
    fancyTable.addRectFace(new Point(1, 0.15, 0.1), new Point(0.97, 0, 0.07), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0.9, 0.15, 0.1), new Point(0.93, 0, 0.07), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0.9, 0.15, 0), new Point(0.93, 0, 0.03), COLOR_TABLE);
    fancyTable.addCube(new Point(0.9, 0.15, 0), 0.1, 0.65, 0.1, COLOR_TABLE, 25);

    // Front Left Leg
    fancyTable.addRectFace(new Point(0.03, 0, 2.03), new Point(0, 0.15, 2),
                           new Point(0.1, 0.15, 2), new Point(0.07, 0, 2.03), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0.1, 0.15, 2.1), new Point(0.07, 0, 2.07), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0, 0.15, 2.1), new Point(0.03, 0, 2.07), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0, 0.15, 2), new Point(0.03, 0, 2.03), COLOR_TABLE);
    fancyTable.addCube(new Point(0, 0.15, 2), 0.1, 0.65, 0.1, COLOR_TABLE, 25);

    // Front Right Leg
    fancyTable.addRectFace(new Point(0.93, 0, 2.03), new Point(0.9, 0.15, 2),
                           new Point(1, 0.15, 2), new Point(0.97, 0, 2.03), COLOR_TABLE);
    fancyTable.addRectFace(new Point(1, 0.15, 2.1), new Point(0.97, 0, 2.07), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0.9, 0.15, 2.1), new Point(0.93, 0, 2.07), COLOR_TABLE);
    fancyTable.addRectFace(new Point(0.9, 0.15, 2), new Point(0.93, 0, 2.03), COLOR_TABLE);
    fancyTable.addCube(new Point(0.9, 0.15, 2), 0.1, 0.65, 0.1, COLOR_TABLE, 25);

    // Golden decorations
    // Back left
    fancyTable.addRectFace(new Point(0.2, 0.6, -0.01), new Point(0.35, 0.8, -0.01),
                           new Point(-0.01, 0.8, -0.01), new Point(-0.01, 0.6, -0.01),
                           COLOR_TABLE_GOLD);
    Point *tex4PointsG1[] = {new Point(0.64, 0), new Point(1.07, 1), new Point(0.07, 1), new Point(0.07, 0)};
    fancyTable.getLastFace()->setTexturePoints(tex4PointsG1);
    fancyTable.getLastFace()->setTexture(11, 0.35, 0.2);
    fancyTable.addRectFace(new Point(-0.01, 0.8, 0.35), new Point(-0.01, 0.6, 0.2),
                           COLOR_TABLE_GOLD);
    Point *tex4PointsG2[] = {new Point(0.07, 0), new Point(0.07, 1), new Point(1.07, 1), new Point(0.64, 0)};
    fancyTable.getLastFace()->setTexturePoints(tex4PointsG2);
    fancyTable.getLastFace()->setTexture(11, 0.35, 0.2);
    // Back right
    fancyTable.addRectFace(new Point(0.8, 0.6, -0.01), new Point(0.65, 0.8, -0.01),
                           new Point(1.01, 0.8, -0.01), new Point(1.01, 0.6, -0.01),
                           COLOR_TABLE_GOLD);
    fancyTable.getLastFace()->setTexturePoints(tex4PointsG1);
    fancyTable.getLastFace()->setTexture(11, 0.35, 0.2);
    fancyTable.addRectFace(new Point(1.01, 0.8, 0.35), new Point(1.01, 0.6, 0.2),
                           COLOR_TABLE_GOLD);
    fancyTable.getLastFace()->setTexturePoints(tex4PointsG2);
    fancyTable.getLastFace()->setTexture(11, 0.35, 0.2);
    // Front left
    fancyTable.addRectFace(new Point(0.2, 0.6, 2.11), new Point(0.35, 0.8, 2.11),
                           new Point(-0.01, 0.8, 2.11), new Point(-0.01, 0.6, 2.11),
                           COLOR_TABLE_GOLD);
    fancyTable.getLastFace()->setTexturePoints(tex4PointsG1);
    fancyTable.getLastFace()->setTexture(11, 0.35, 0.2);
    fancyTable.addRectFace(new Point(-0.01, 0.8, 1.75), new Point(-0.01, 0.6, 1.9),
                           COLOR_TABLE_GOLD);
    fancyTable.getLastFace()->setTexturePoints(tex4PointsG2);
    fancyTable.getLastFace()->setTexture(11, 0.35, 0.2);
    // Front right
    fancyTable.addRectFace(new Point(0.8, 0.6, 2.11), new Point(0.65, 0.8, 2.11),
                           new Point(1.01, 0.8, 2.11), new Point(1.01, 0.6, 2.11),
                           COLOR_TABLE_GOLD);
    fancyTable.getLastFace()->setTexturePoints(tex4PointsG1);
    fancyTable.getLastFace()->setTexture(11, 0.35, 0.2);
    fancyTable.addRectFace(new Point(1.01, 0.8, 1.75), new Point(1.01, 0.6, 1.9),
                           COLOR_TABLE_GOLD);
    fancyTable.getLastFace()->setTexturePoints(tex4PointsG2);
    fancyTable.getLastFace()->setTexture(11, 0.35, 0.2);

    // Top
    fancyTable.addCube(new Point(-0.1, 0.8, -0.1), 1.2, 0.1, 2.3, COLOR_TABLE_TOP_SIDE, 25);
    fancyTable.addCube(new Point(-0.05, 0.81, -0.05), 1.1, 0.1, 2.2, COLOR_TABLE_TOP, 2);

    ///---------------------------------------------- Big Table -----------------------------------------------------///
    // platform
    bigTable.addCube(new Point(-4.6, 0.01, -0.9), 2.4, 0.3, 1.7, COLOR_DOOR, 25);

    // top
    bigTable.addRectFace(new Point(0, 0.1, 0), new Point(0, 0.1, -1.7), new Point(0, 1.3, -1.7),
                         new Point(0, 1.3, 0), COLOR_TABLE);
    bigTable.getLastFace()->setTexture(26);
    bigTable.addRectFace(new Point(-2.6, 1.3, -1.7), new Point(-2.6, 1.3, 0), COLOR_DOOR);
    bigTable.getLastFace()->setTexture(14);
    bigTable.addRectFace(new Point(-2.6, 1.6, -1.7), new Point(-2.6, 1.6, 0), COLOR_DOOR);
    bigTable.getLastFace()->setTexture(14);
    bigTable.addRectFace(new Point(-4.3, 1.6, -1.7), new Point(-4.3, 1.6, 0), COLOR_DOOR);
    bigTable.getLastFace()->setTexture(14);
    bigTable.addRectFace(new Point(-4.3, 1.3, -1.7), new Point(-4.3, 1.3, 0), COLOR_DOOR);
    bigTable.getLastFace()->setTexture(14);
    bigTable.addRectFace(new Point(-6.9, 1.3, -1.7), new Point(-6.9, 1.3, 0), COLOR_DOOR);
    bigTable.getLastFace()->setTexture(14);
    bigTable.addRectFace(new Point(-6.9, 0.1, -1.7), new Point(-6.9, 0.1, 0), COLOR_TABLE);
    bigTable.getLastFace()->setTexture(26);

    // front
    bigTable.addRectFace(new Point(0, 0.1, -1.7), new Point(0, 1.3, -1.7), new Point(-6.9, 1.3, -1.7),
                         new Point(-6.9, 0.1, -1.7), COLOR_TABLE);
    bigTable.getLastFace()->setTexture(26);
    bigTable.addRectFace(new Point(-2.6, 1.3, -1.7), new Point(-4.3, 1.3, -1.7), new Point(-4.3, 1.6, -1.7),
                         new Point(-2.6, 1.6, -1.7), COLOR_TABLE);
    bigTable.getLastFace()->setTexture(26);

    // detail top
    bigTable.addTriangFace(new Point(-2.6, 1.6, -1.7), new Point(-4.3, 1.6, -1.7), new Point(-3.45, 1.8, -1.7),
                           COLOR_TABLE);
    bigTable.getLastFace()->setTexture(26);
    bigTable.addTriangFace(new Point(-2.6, 1.6, -1.6), new Point(-4.3, 1.6, -1.6), new Point(-3.45, 1.8, -1.6),
                           COLOR_TABLE);
    bigTable.getLastFace()->setTexture(26);
    bigTable.addRectFace(new Point(-4.3, 1.6, -1.7), new Point(-4.3, 1.6, -1.6), new Point(-3.45, 1.8, -1.6),
                         new Point(-3.45, 1.8, -1.7), COLOR_TABLE);
    bigTable.getLastFace()->setTexture(26);
    bigTable.addRectFace(new Point(-2.6, 1.6, -1.6), new Point(-2.6, 1.6, -1.7), COLOR_TABLE);
    bigTable.getLastFace()->setTexture(26);

    // legs
    bigTable.addCylinder(new Point(-0.1, 0, -0.1), 0.1, 0.1, 0.1, -90.0f, axisX, COLOR_DOOR);
    bigTable.addCylinder(new Point(-0.1, 0, -1.6), 0.1, 0.1, 0.1, -90.0f, axisX, COLOR_DOOR);
    bigTable.addCylinder(new Point(-6.8, 0, -1.6), 0.1, 0.1, 0.1, -90.0f, axisX, COLOR_DOOR);
    bigTable.addCylinder(new Point(-6.8, 0, -0.1), 0.1, 0.1, 0.1, -90.0f, axisX, COLOR_DOOR);

    ///-------------------------------------------------- Chair -----------------------------------------------------///
    // Back Left Leg
    chair.addRectFace(new Point(0.03, 0, 0.03), new Point(0, 0.15, 0),
                      new Point(0.1, 0.15, 0), new Point(0.07, 0, 0.03), COLOR_TABLE);
    chair.addRectFace(new Point(0.1, 0.15, 0.1), new Point(0.07, 0, 0.07), COLOR_TABLE);
    chair.addRectFace(new Point(0, 0.15, 0.1), new Point(0.03, 0, 0.07), COLOR_TABLE);
    chair.addRectFace(new Point(0, 0.15, 0), new Point(0.03, 0, 0.03), COLOR_TABLE);
    chair.addCube(new Point(0, 0.15, 0), 0.1, 0.75, 0.1, COLOR_TABLE, 26);

    // Back Right Leg
    chair.addRectFace(new Point(0.53, 0, 0.03), new Point(0.5, 0.15, 0),
                      new Point(0.6, 0.15, 0), new Point(0.57, 0, 0.03), COLOR_TABLE);
    chair.addRectFace(new Point(0.6, 0.15, 0.1), new Point(0.57, 0, 0.07), COLOR_TABLE);
    chair.addRectFace(new Point(0.5, 0.15, 0.1), new Point(0.53, 0, 0.07), COLOR_TABLE);
    chair.addRectFace(new Point(0.5, 0.15, 0), new Point(0.53, 0, 0.03), COLOR_TABLE);
    chair.addCube(new Point(0.5, 0.15, 0), 0.1, 0.65, 0.1, COLOR_TABLE, 26);

    // Front Left Leg
    chair.addRectFace(new Point(0.03, 0, 0.83), new Point(0, 0.15, 0.8),
                      new Point(0.1, 0.15, 0.8), new Point(0.07, 0, 0.83), COLOR_TABLE);
    chair.addRectFace(new Point(0.1, 0.15, 0.9), new Point(0.07, 0, 0.87), COLOR_TABLE);
    chair.addRectFace(new Point(0, 0.15, 0.9), new Point(0.03, 0, 0.87), COLOR_TABLE);
    chair.addRectFace(new Point(0, 0.15, 0.8), new Point(0.03, 0, 0.83), COLOR_TABLE);
    chair.addCube(new Point(0, 0.15, 0.8), 0.1, 0.65, 0.1, COLOR_TABLE, 26);

    // Front Right Leg
    chair.addRectFace(new Point(0.53, 0, 0.83), new Point(0.5, 0.15, 0.8),
                      new Point(0.6, 0.15, 0.8), new Point(0.57, 0, 0.83), COLOR_TABLE);
    chair.addRectFace(new Point(0.6, 0.15, 0.9), new Point(0.57, 0, 0.87), COLOR_TABLE);
    chair.addRectFace(new Point(0.5, 0.15, 0.9), new Point(0.53, 0, 0.87), COLOR_TABLE);
    chair.addRectFace(new Point(0.5, 0.15, 0.8), new Point(0.53, 0, 0.83), COLOR_TABLE);
    chair.addCube(new Point(0.5, 0.15, 0.8), 0.1, 0.65, 0.1, COLOR_TABLE, 26);

    // Seat
    chair.addCube(new Point(-0.1, 0.8, 0), 0.8, 0.1, 0.95, COLOR_TABLE_TOP_SIDE, 26);

    // Gold seat
    chair.addRectFace(new Point(0, 0.92, 0.15), new Point(0, 0.92, 0.85),
                      new Point(0.6, 0.92, 0.85), new Point(0.6, 0.92, 0.15), COLOR_TABLE_GOLD);
    chair.getLastFace()->setTexture(19);

    // Backrest
    chair.addCube(new Point(-0.1, 0.8, 0), 0.8, 0.9, 0.075, COLOR_TABLE_TOP_SIDE, 26);

    // Arms
    chair.addCube(new Point(-0.1, 1.15, 0), 0.05, 0.05, 0.9, COLOR_TABLE, 26);
    chair.addCube(new Point(0.65, 1.15, 0), 0.05, 0.05, 0.9, COLOR_TABLE, 26);
    chair.addCube(new Point(-0.1, 0.8, 0.8), 0.05, 0.35, 0.05, COLOR_TABLE_TOP_SIDE, 26);
    chair.addCube(new Point(0.65, 0.8, 0.8), 0.05, 0.35, 0.05, COLOR_TABLE_TOP_SIDE, 26);

    ///-------------------------------------------- Fancy Chair -----------------------------------------------------///
    // Back Left Leg
    fancyChair.addRectFace(new Point(0.03, 0, 0.03), new Point(0, 0.15, 0),
                           new Point(0.1, 0.15, 0), new Point(0.07, 0, 0.03), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0.1, 0.15, 0.1), new Point(0.07, 0, 0.07), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0, 0.15, 0.1), new Point(0.03, 0, 0.07), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0, 0.15, 0), new Point(0.03, 0, 0.03), COLOR_TABLE);
    fancyChair.addCube(new Point(0, 0.15, 0), 0.1, 0.75, 0.1, COLOR_TABLE, 25);

    // Back Right Leg
    fancyChair.addRectFace(new Point(0.53, 0, 0.03), new Point(0.5, 0.15, 0),
                           new Point(0.6, 0.15, 0), new Point(0.57, 0, 0.03), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0.6, 0.15, 0.1), new Point(0.57, 0, 0.07), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0.5, 0.15, 0.1), new Point(0.53, 0, 0.07), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0.5, 0.15, 0), new Point(0.53, 0, 0.03), COLOR_TABLE);
    fancyChair.addCube(new Point(0.5, 0.15, 0), 0.1, 0.65, 0.1, COLOR_TABLE, 25);

    // Front Left Leg
    fancyChair.addRectFace(new Point(0.03, 0, 0.83), new Point(0, 0.15, 0.8),
                           new Point(0.1, 0.15, 0.8), new Point(0.07, 0, 0.83), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0.1, 0.15, 0.9), new Point(0.07, 0, 0.87), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0, 0.15, 0.9), new Point(0.03, 0, 0.87), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0, 0.15, 0.8), new Point(0.03, 0, 0.83), COLOR_TABLE);
    fancyChair.addCube(new Point(0, 0.15, 0.8), 0.1, 0.65, 0.1, COLOR_TABLE, 25);

    // Front Right Leg
    fancyChair.addRectFace(new Point(0.53, 0, 0.83), new Point(0.5, 0.15, 0.8),
                           new Point(0.6, 0.15, 0.8), new Point(0.57, 0, 0.83), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0.6, 0.15, 0.9), new Point(0.57, 0, 0.87), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0.5, 0.15, 0.9), new Point(0.53, 0, 0.87), COLOR_TABLE);
    fancyChair.addRectFace(new Point(0.5, 0.15, 0.8), new Point(0.53, 0, 0.83), COLOR_TABLE);
    fancyChair.addCube(new Point(0.5, 0.15, 0.8), 0.1, 0.65, 0.1, COLOR_TABLE, 25);

    // Seat
    fancyChair.addCube(new Point(-0.1, 0.8, 0), 0.85, 0.1, 0.95, COLOR_TABLE_TOP_SIDE, 25);


    // Backrest
    fancyChair.addCube(new Point(-0.1, 0.8, 0), 0.8, 1.2, 0.075, COLOR_TABLE_TOP_SIDE, 25);

    // Backrest detail
    fancyChair.addTriangFace(new Point(-0.1, 2, 0), new Point(0.7, 2, 0), new Point(0.3, 2.2, 0), COLOR_TABLE);
    fancyChair.getLastFace()->setTexture(25);
    fancyChair.addTriangFace(new Point(-0.1, 2, 0.075), new Point(0.7, 2, 0.075), new Point(0.3, 2.2, 0.075),
                             COLOR_TABLE);
    fancyChair.getLastFace()->setTexture(25);
    fancyChair.addRectFace(new Point(-0.1, 2, 0), new Point(-0.1, 2, 0.075), new Point(0.3, 2.2, 0.075),
                           new Point(0.3, 2.2, 0), COLOR_TABLE);
    fancyChair.getLastFace()->setTexture(25);
    fancyChair.addRectFace(new Point(0.7, 2.0, 0.075), new Point(0.7, 2.0, 0), COLOR_TABLE);
    fancyChair.getLastFace()->setTexture(25);

    // Arms
    fancyChair.addCube(new Point(-0.1, 1.15, 0), 0.05, 0.05, 0.9, COLOR_TABLE, 25);
    fancyChair.addCube(new Point(0.65, 1.15, 0), 0.05, 0.05, 0.9, COLOR_TABLE, 25);
    fancyChair.addCube(new Point(-0.1, 0.8, 0.8), 0.05, 0.35, 0.05, COLOR_TABLE_TOP_SIDE, 25);
    fancyChair.addCube(new Point(0.65, 0.8, 0.8), 0.05, 0.35, 0.05, COLOR_TABLE_TOP_SIDE, 25);

    ///-------------------------------------------- Fancy Couch -----------------------------------------------------///
    // Feet
    fancyCouch.addCube(new Point(), 0.1, 0.15, 0.1, COLOR_COUCH_FOOT, 26);
    fancyCouch.addCube(new Point(0.5, 0, 0), 0.1, 0.15, 0.1, COLOR_COUCH_FOOT, 26);
    fancyCouch.addCube(new Point(1.5, 0, 0), 0.1, 0.15, 0.1, COLOR_COUCH_FOOT, 26);
    fancyCouch.addCube(new Point(1.9, 0, 0), 0.1, 0.15, 0.1, COLOR_COUCH_FOOT, 26);
    fancyCouch.addCube(new Point(0, 0, 0.4), 0.1, 0.15, 0.1, COLOR_COUCH_FOOT, 26);
    fancyCouch.addCube(new Point(0.5, 0, 0.4), 0.1, 0.15, 0.1, COLOR_COUCH_FOOT, 26);
    fancyCouch.addCube(new Point(1.5, 0, 0.4), 0.1, 0.15, 0.1, COLOR_COUCH_FOOT, 26);
    fancyCouch.addCube(new Point(1.9, 0, 0.4), 0.1, 0.15, 0.1, COLOR_COUCH_FOOT, 26);

    // Base
    fancyCouch.addCube(new Point(0, 0.15, 0), 2, 0.1, 0.5, COLOR_COUCH_WOOD, 26);

    // Arm supports
    fancyCouch.addCube(new Point(0, 0.15, 0), 0.1, 0.35, 0.5, COLOR_COUCH_WOOD, 26);
    fancyCouch.addRectFace(new Point(-0.01, 0.17, 0.02), new Point(-0.01, 0.48, 0.02), new Point(-0.01, 0.48, 0.48),
                           new Point(-0.01, 0.17, 0.48), COLOR_COUCH_PAD);
    fancyCouch.getLastFace()->setTexture(2, 0.5, 0.5);
    fancyCouch.addRectFace(new Point(0.11, 0.15, 0.02), new Point(0.11, 0.48, 0.02), new Point(0.11, 0.48, 0.48),
                           new Point(0.11, 0.15, 0.48), COLOR_COUCH_PAD);
    fancyCouch.getLastFace()->setTexture(2, 0.5, 0.5);

    fancyCouch.addCube(new Point(1.9, 0.15, 0), 0.1, 0.35, 0.5, COLOR_COUCH_WOOD, 26);
    fancyCouch.addRectFace(new Point(1.89, 0.15, 0.02), new Point(1.89, 0.48, 0.02), new Point(1.89, 0.48, 0.48),
                           new Point(1.89, 0.15, 0.48), COLOR_COUCH_PAD);
    fancyCouch.getLastFace()->setTexture(2, 0.5, 0.5);
    fancyCouch.addRectFace(new Point(2.01, 0.15, 0.02), new Point(2.01, 0.48, 0.02), new Point(2.01, 0.48, 0.48),
                           new Point(2.01, 0.15, 0.48), COLOR_COUCH_PAD);
    fancyCouch.getLastFace()->setTexture(2, 0.5, 0.5);

    // Golden details
    fancyCouch.addRectFace(new Point(0.1, 0.5, 0.51), new Point(0.1, 0.4, 0.51), new Point(0, 0.4, 0.51),
                           new Point(0, 0.5, 0.51), COLOR_TABLE_GOLD);
    fancyCouch.addRectFace(new Point(2, 0.5, 0.51), new Point(2, 0.4, 0.51), new Point(1.9, 0.4, 0.51),
                           new Point(1.9, 0.5, 0.51), COLOR_TABLE_GOLD);

    // Back
    Point *points[] = {
            new Point(0, 0.15, 0), new Point(0, 0.65, 0), new Point(0.3, 0.65, 0), new Point(0.75, 0.9, 0),
            new Point(1.25, 0.9, 0), new Point(1.7, 0.65, 0), new Point(2, 0.65, 0), new Point(2, 0.15, 0)
    },
            *tex8Points[] = {
            new Point(0, 0), new Point(0, 0.66), new Point(0.15, 0.66), new Point(0.375, 1), new Point(0.625, 1),
            new Point(0.85, 0.66), new Point(1, 0.66), new Point(1, 0)
    };

    fancyCouch.addFace(new Face(8, points, COLOR_COUCH_WOOD));
    fancyCouch.getLastFace()->setTexture(26);

    Point *points2[] = {new Point(0.05, 0.15, 0.01), new Point(0.05, 0.6, 0.01), new Point(0.3, 0.60, 0.01),
                        new Point(0.75, 0.85, 0.01), new Point(1.25, 0.85, 0.01), new Point(1.7, 0.60, 0.01),
                        new Point(1.95, 0.6, 0.01), new Point(1.95, 0.15, 0.01)};
    fancyCouch.addFace(new Face(8, points2, COLOR_COUCH_PAD));
    fancyCouch.getLastFace()->setTexturePoints(tex8Points);
    fancyCouch.getLastFace()->setTexture(2, 0.5, 0.5);

    // Seat
    fancyCouch.addCube(new Point(0.1, 0.25, 0.01), 1.8, 0.07, 0.49, COLOR_COUCH_PAD_SEAT, 2);
}