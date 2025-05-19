#include <GL/glut.h>
#include <math.h>

float boatX = -140.0f;
float sunA  = 0.0f;
int isPaused = 0;  // 0 = running, 1 = paused

float mix(float a, float b, float t) {
    return a * (1 - t) + b * t;
}

void circle(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 60; ++i) {
        float ang = 2.0f * 3.1415926f * i / 60.0f;
        glVertex2f(cx + r * cosf(ang), cy + r * sinf(ang));
    }
    glEnd();
}

void sky(float t) {
    glBegin(GL_QUADS);
        glColor3f(mix(0.9, 0.2, t), mix(0.5, 0.4, t), mix(0.2, 1.0, t));
        glVertex2f(0, 600); glVertex2f(800, 600);
        glColor3f(mix(1.0, 0.9, t), mix(0.7, 0.6, t), mix(0.3, 0.8, t));
        glVertex2f(800, 200); glVertex2f(0, 200);
    glEnd();
}

void mountains(void) {
    glColor3f(0.25f, 0.3f, 0.28f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0, 200);   glVertex2f(400, 200); glVertex2f(200, 520);
        glVertex2f(400, 200); glVertex2f(800, 200); glVertex2f(600, 520);
    glEnd();
}

void ground(void) {
    glColor3f(0.25f, 0.55f, 0.25f);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);   glVertex2f(800, 0);
        glVertex2f(800, 200); glVertex2f(0, 200);
    glEnd();
}

void waterStrip(void) {
    glColor3f(0.15f, 0.35f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(0, 60); glVertex2f(800, 60);
        glVertex2f(800, 140); glVertex2f(0, 140);
    glEnd();
}

void house(void) {
    glColor3f(0.9f, 0.4f, 0.3f);   // walls
    glBegin(GL_QUADS);
        glVertex2f(100, 145); glVertex2f(180, 145);
        glVertex2f(180, 215); glVertex2f(100, 215);
    glEnd();

    glColor3f(0.35f, 0.17f, 0.1f); // roof
    glBegin(GL_TRIANGLES);
        glVertex2f(90, 215); glVertex2f(190, 215); glVertex2f(140, 255);
    glEnd();

    // door
    glColor3f(0.3f, 0.15f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(125, 145); glVertex2f(155, 145);
        glVertex2f(155, 190); glVertex2f(125, 190);
    glEnd();
}

void tree(float x, float y) {
    float s = 1.5f;
    glColor3f(0.55f, 0.27f, 0.07f); // trunk
    glBegin(GL_QUADS);
        glVertex2f(x - 2 * s, y); glVertex2f(x + 2 * s, y);
        glVertex2f(x + 2 * s, y + 20 * s); glVertex2f(x - 2 * s, y + 20 * s);
    glEnd();
    glColor3f(0.0f, 0.5f, 0.0f); // leaves
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 10 * s, y + 20 * s); glVertex2f(x + 10 * s, y + 20 * s); glVertex2f(x, y + 40 * s);
        glVertex2f(x - 8 * s, y + 30 * s); glVertex2f(x + 8 * s, y + 30 * s); glVertex2f(x, y + 50 * s);
    glEnd();
}

void boat(float x) {
    glColor3f(0.35f, 0.18f, 0.08f); // hull
    glBegin(GL_POLYGON);
        glVertex2f(x, 140); glVertex2f(x + 20, 115);
        glVertex2f(x + 100, 115); glVertex2f(x + 120, 140);
        glVertex2f(x + 100, 155); glVertex2f(x + 20, 155);
    glEnd();
    glColor3f(0.2f, 0.2f, 0.2f); // mast
    glBegin(GL_QUADS);
        glVertex2f(x + 60, 155); glVertex2f(x + 63, 155);
        glVertex2f(x + 63, 215); glVertex2f(x + 60, 215);
    glEnd();
    glColor3f(1, 1, 1); // sail
    glBegin(GL_TRIANGLES);
        glVertex2f(x + 63, 215); glVertex2f(x + 63, 155); glVertex2f(x + 115, 185);
    glEnd();
}

void display(void) {
    float dayT = 0.5f * (sinf(sunA - 1.5708f) + 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    sky(dayT);

    float sunY = 200 + 150.0f * fabsf(sinf(sunA)); // vertical sun
    glColor3f(1.0f, 0.85f, 0.05f);
    circle(400, sunY, 40);

    mountains();
    ground();
    waterStrip();
    house();

    // Trees on both sides of the river, outside water
    tree(50, 145);   tree(120, 145); tree(180, 150);
    tree(250, 145);  tree(280, 150); tree(320, 145);
    tree(350, 150);
    tree(700, 145);  tree(740, 148); tree(770, 150);
    tree(630, 145);  tree(680, 145);

    boat(boatX);

    glutSwapBuffers();
}

void timer(int v) {
    if (!isPaused) {
        boatX += 1.2f;           if (boatX > 920) boatX = -140;
        sunA += 0.002f;          if (sunA > 3.14159f) sunA -= 3.14159f;
        glutPostRedisplay();
    }
    glutTimerFunc(16, timer, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 's' || key == 'S') {
        isPaused = !isPaused;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sunrise Scene with River and Trees");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
