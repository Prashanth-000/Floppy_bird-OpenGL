#include <GL/freeglut.h>
#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

float birdY = 300.0f;
float birdVelocity = 0.0f;
float gravity = -0.5f;
bool gameOver = false;
bool gameStarted = false;

float pipeX = 800.0f;
float pipeWidth = 80.0f;
float gapY = 300.0f;
float gapHeight = 200.0f;

int score = 0;
bool passedPipe = false;

void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void drawText(float x, float y, const char* string) {
    glRasterPos2f(x, y);
    for (int i = 0; string[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Bird
    glColor3f(1, 1, 0); // Yellow
    drawRect(100, birdY, 30, 30);

    // Pipes
    glColor3f(0, 1, 0); // Green
    drawRect(pipeX, 0, pipeWidth, gapY - gapHeight / 2);
    drawRect(pipeX, gapY + gapHeight / 2, pipeWidth, WINDOW_HEIGHT);

    // Score
    glColor3f(0, 0, 0); // Black
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    drawText(10, WINDOW_HEIGHT - 30, scoreText);

    // Instructions or Game Over
    if (!gameStarted) {
        glColor3f(0, 0, 0);
        drawText(270, 300, "Press SPACE to Start");
    } else if (gameOver) {
        glColor3f(1, 0, 0);
        drawText(250, 300, "Game Over! Press SPACE to Restart.");
    }

    glutSwapBuffers();
}

void update(int value) {
    if (gameStarted && !gameOver) {
        // Bird physics
        birdVelocity += gravity;
        birdY += birdVelocity;

        // Pipe movement
        pipeX -= 5.0f;

        if (pipeX + pipeWidth < 0) {
            pipeX = WINDOW_WIDTH;
            gapY = rand() % 400 + 100;
            passedPipe = false;
        }

        // Scoring
        if (!passedPipe && pipeX + pipeWidth < 100) {
            score++;
            passedPipe = true;
        }

        // Collision
        if (birdY < 0 || birdY + 30 > WINDOW_HEIGHT ||
            (pipeX < 130 && pipeX + pipeWidth > 100 &&
             (birdY < gapY - gapHeight / 2 || birdY + 30 > gapY + gapHeight / 2))) {
            gameOver = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 32) { // Spacebar
        if (!gameStarted) {
            gameStarted = true;
            birdVelocity = 8.0f;
        } else if (gameOver) {
            // Restart game
            birdY = 300;
            birdVelocity = 0;
            pipeX = WINDOW_WIDTH;
            gapY = rand() % 400 + 100;
            score = 0;
            passedPipe = false;
            gameOver = false;
            gameStarted = false;  // Wait again for space to start
        } else {
            birdVelocity = 8.0f;
        }
    }
}

void init() {
    glClearColor(0.5f, 0.8f, 1.0f, 1); // Sky blue background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    srand(time(NULL));
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Flappy Bird in C - With Score & Start Delay");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}

