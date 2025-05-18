#include <GL/freeglut.h> // OpenGL Utility Toolkit for windowing and graphics
#include <stdio.h>       // For input/output functions (e.g., sprintf)
#include <stdlib.h>      // For general utilities like rand()
#include <stdbool.h>     // For using boolean data types
#include <time.h>        // For random number seeding

// Window dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Bird parameters
float birdY = 300.0f;      // Initial vertical position of the bird
float birdVelocity = 0.0f; // Vertical speed of the bird
float gravity = -0.5f;     // Gravity pulling the bird down
bool gameOver = false;     // Flag to indicate if the game is over
bool gameStarted = false;  // Flag to indicate if the game has started

// Pipe parameters
float pipeX = 800.0f;     // Horizontal position of the pipe
float pipeWidth = 80.0f;  // Width of the pipe
float gapY = 300.0f;      // Y position of the gap between pipes
float gapHeight = 200.0f; // Vertical size of the gap

// Score and state
int score = 0;           // Player score
bool passedPipe = false; // Whether the bird has passed the pipe to count the score

// Function to draw a rectangle (used for bird and pipes)
void drawRect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// Function to draw text on screen
void drawText(float x, float y, const char *string)
{
    glRasterPos2f(x, y);
    for (int i = 0; string[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

// Display callback function: renders everything on screen
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear screen

    // Draw the bird (yellow square)
    glColor3f(1, 1, 0); // RGB: Yellow
    drawRect(100, birdY, 30, 30);

    // Draw the pipes (green rectangles)
    glColor3f(0, 1, 0);                                              // RGB: Green
    drawRect(pipeX, 0, pipeWidth, gapY - gapHeight / 2);             // Bottom pipe
    drawRect(pipeX, gapY + gapHeight / 2, pipeWidth, WINDOW_HEIGHT); // Top pipe

    // Draw the score
    glColor3f(0, 0, 0); // RGB: Black
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    drawText(10, WINDOW_HEIGHT - 30, scoreText); // Show score at top-left

    // Show instruction or game over message
    if (!gameStarted)
    {
        glColor3f(0, 0, 0);
        drawText(270, 300, "Press SPACE to Start");
    }
    else if (gameOver)
    {
        glColor3f(1, 0, 0); // Red color
        drawText(250, 300, "Game Over! Press SPACE to Restart.");
    }

    glutSwapBuffers(); // Swap the frame buffers
}

// Timer function: updates game logic every 16 ms (~60 FPS)
void update(int value)
{
    if (gameStarted && !gameOver)
    {
        // Update bird position based on velocity and gravity
        birdVelocity += gravity;
        birdY += birdVelocity;

        // Move the pipe leftward
        pipeX -= 5.0f;

        // Reset pipe when it goes off-screen and generate new gap position
        if (pipeX + pipeWidth < 0)
        {
            pipeX = WINDOW_WIDTH;
            gapY = rand() % 400 + 100; // Random gap between 100 and 500
            passedPipe = false;
        }

        // Update score when bird passes the pipe
        if (!passedPipe && pipeX + pipeWidth < 100)
        {
            score++;
            passedPipe = true;
        }

        // Check for collision: ground, top, or hitting the pipe
        if (birdY < 0 || birdY + 30 > WINDOW_HEIGHT ||
            (pipeX < 130 && pipeX + pipeWidth > 100 &&
             (birdY < gapY - gapHeight / 2 || birdY + 30 > gapY + gapHeight / 2)))
        {
            gameOver = true;
        }
    }

    // Re-render the screen
    glutPostRedisplay();

    // Call this function again after 16 milliseconds
    glutTimerFunc(16, update, 0);
}

// Keyboard callback: controls bird flap and game restart
void keyboard(unsigned char key, int x, int y)
{
    if (key == 32)
    { // ASCII for SPACEBAR
        if (!gameStarted)
        {
            // Start game with upward bird velocity
            gameStarted = true;
            birdVelocity = 8.0f;
        }
        else if (gameOver)
        {
            // Restart game: reset all states
            birdY = 300;
            birdVelocity = 0;
            pipeX = WINDOW_WIDTH;
            gapY = rand() % 400 + 100;
            score = 0;
            passedPipe = false;
            gameOver = false;
            gameStarted = false; // Wait for space again
        }
        else
        {
            // Make the bird "flap" by giving upward velocity
            birdVelocity = 8.0f;
        }
    }
}

// OpenGL initialization
void init()
{
    glClearColor(0.5f, 0.8f, 1.0f, 1);             // Set background color (sky blue)
    glMatrixMode(GL_PROJECTION);                   // Set projection mode
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT); // 2D coordinate system
    srand(time(NULL));                             // Seed random number generator for pipe gaps
}

// Main function: entry point of the program
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                           // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);     // Use double buffering and RGB colors
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // Set window size
    glutCreateWindow("Jumping Box game");            // Window title

    init();                      // Call init function
    glutDisplayFunc(display);    // Set display callback
    glutKeyboardFunc(keyboard);  // Set keyboard input callback
    glutTimerFunc(0, update, 0); // Set the timer callback
    glutMainLoop();              // Enter the event-processing loop
    return 0;
}
