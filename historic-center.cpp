#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "camera.cpp"

#define GLUT_KEY_SHIFT 112

// actual vector representing the camera's direction
//float lx = 0.0f, lz = -1.0f, ly = 0.0f;

// position of the camera
//float x = 0.0f, z = 7.0f, y = 1.0f;
Camera* cam = new Camera(*(new Point(0.0f, 1.0f, 7.0f)), *(new Point(0.0f, 0.0f, -1.0f)), 0.1f, 0.05f);

float door_angle = 0.0f;

// angle for rotating
//float angle = 0.0f;

bool keyPressed[256], specialKeyPressed[256];

void init(void) {
    // sky color
    glClearColor(0.0, 0.7, 1.0, 1.0);
}

void drawHouse() {

    // Front side
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.65f);

    glBegin(GL_QUADS);  // Wall
    glVertex3f(0.3, 0, 1);
    glVertex3f(2, 0, 1);
    glVertex3f(2, -1.5, 1);
    glVertex3f(0.3, -1.5, 1);
    glEnd();

    glBegin(GL_QUADS);  // Wall
    glVertex3f(-2, 0, 1);
    glVertex3f(-0.3, 0, 1);
    glVertex3f(-0.3, -1.5, 1);
    glVertex3f(-2, -1.5, 1);
    glEnd();

    glBegin(GL_QUADS);  // Wall
    glVertex3f(0.3, 0, 1);
    glVertex3f(-0.3, 0, 1);
    glVertex3f(-0.3, -0.25, 1);
    glVertex3f(0.3, -0.25, 1);
    glEnd();

    glColor3f(0.4f, 0.0f, 0.0f);
    glBegin(GL_QUADS);  // Roof
    glVertex3f(-2.2, 0.5, 0);
    glVertex3f(2.2, 0.5, 0);
    glVertex3f(2.2, -0.1, 1.25);
    glVertex3f(-2.2, -0.1, 1.25);
    glEnd();

    glColor3f(0.7f, 0.7f, 0.0f);
    glBegin(GL_QUADS);  // Window Left
    glVertex3f(-1.5, -0.3, 1.0001);
    glVertex3f(-0.75, -0.3, 1.0001);
    glVertex3f(-0.75, -0.8, 1.0001);
    glVertex3f(-1.5, -0.8, 1.0001);
    glEnd();

    glBegin(GL_QUADS);  // Window Right
    glVertex3f(1.5, -0.3, 1.0001);
    glVertex3f(0.75, -0.3, 1.0001);
    glVertex3f(0.75, -0.8, 1.0001);
    glVertex3f(1.5, -0.8, 1.0001);
    glEnd();
    glPopMatrix();

    // Back side
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.65f);
    glBegin(GL_QUADS);  // Wall
    glVertex3f(-2, 0, -1);
    glVertex3f(2, 0, -1);
    glVertex3f(2, -1.5, -1);
    glVertex3f(-2, -1.5, -1);
    glEnd();

    glColor3f(0.4f, 0.0f, 0.0f);
    glBegin(GL_QUADS);  // Roof
    glVertex3f(-2.2, 0.5, 0);
    glVertex3f(2.2, 0.5, 0);
    glVertex3f(2.2, -0.1, -1.25);
    glVertex3f(-2.2, -0.1, -1.25);
    glEnd();

    glBegin(GL_QUADS);  // Window Left
    glVertex3f(-1.5, -0.3, -1.0001);
    glVertex3f(-0.75, -0.3, -1.0001);
    glVertex3f(-0.75, -0.8, -1.0001);
    glVertex3f(-1.5, -0.8, -1.0001);
    glEnd();

    glBegin(GL_QUADS);  // Window Right
    glVertex3f(1.5, -0.3, -1.0001);
    glVertex3f(0.75, -0.3, -1.0001);
    glVertex3f(0.75, -0.8, -1.0001);
    glVertex3f(1.5, -0.8, -1.0001);
    glEnd();
    glPopMatrix();

    // Right side
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.65f);
    glBegin(GL_QUADS);  // Wall
    glVertex3f(2, 0, 1);
    glVertex3f(2, 0, -1);
    glVertex3f(2, -1.5, -1);
    glVertex3f(2, -1.5, 1);
    glEnd();

    glBegin(GL_TRIANGLES);  // Wall Upper
    glVertex3f(2, 0.5, 0);
    glVertex3f(2, 0, 1);
    glVertex3f(2, 0, -1);
    glEnd();
    glPopMatrix();

    // Left side
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 0.0f);
    glColor3f(0.0f, 0.0f, 0.65f);
    glBegin(GL_QUADS);  // Wall
    glVertex3f(-2, 0, 1);
    glVertex3f(-2, 0, -1);
    glVertex3f(-2, -1.5, -1);
    glVertex3f(-2, -1.5, 1);
    glEnd();

    glBegin(GL_TRIANGLES);  // Wall Upper
    glVertex3f(-2, 0.5, 0);
    glVertex3f(-2, 0, 1);
    glVertex3f(-2, 0, -1);
    glEnd();
    glPopMatrix();
}

void drawDoor() {

    glPushMatrix();
    glTranslatef(-0.3f, 0.5f, 1);
    glRotatef(door_angle, 0, 1, 0);
    glTranslatef(0.3f, 0, 0);
    glColor3f(0.7f, 0.7f, 0.7f);
    glScalef(0.6, 1.1, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0) h = 1;
    float ratio = w * 1.0 / h;

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
        cam->modeBackward();
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

    glutPostRedisplay();
}

void updateDoor() {
    if (keyPressed['o'])
        if (door_angle <= 118.0f)
            door_angle += 2.0f;
    if (keyPressed['c'])
        if (door_angle >= 2.0f)
            door_angle -= 2.0f;
}

void renderScene(void) {

    // Para ver os objetos em modo polígono (somente os traços)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera
    gluLookAt(cam->position.x, cam->position.y, cam->position.z,
              cam->position.x + cam->direction.x, cam->position.y + cam->direction.y, cam->position.z + cam->direction.z,
              0.0f, 1.0f, 0.0f);

    // Draw ground
    glColor3f(0.0, 0.65, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    drawHouse();
    drawDoor();
    updateDoor();

    updateCamera();

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

void specialFuncHandler(int key, int xx, int yy) {
    specialKeyPressed[key] = true;
}

void specialFuncUpHandler(int key, int xx, int yy) {
    specialKeyPressed[key] = false;
}

int main(int argc, char **argv) {

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(800, 600);
    glutCreateWindow("House 3D");

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
