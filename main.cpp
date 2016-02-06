// C++ libraries
#include <iostream>

#include "Camera.h"
#include "SolarSystem.h"

/// Defines
#define     FPS     17

/// Global Variables
int mainWindow, windowWidth, windowHeight;
int mouseX, mouseY;
double currentAspect;

// States
int axisOn = 1, gridOn = 1, detailsOn = 1, mode, projOn;

// Main Objects
SolarSystem *solarSystem;
Camera *camera;

using namespace std;


///////////////////////////////////////////////////////////////////////////////
/** Auxiliar functions **/

// Draw a string in the screen
void drawBitmapString(char *string, float x, float y, float z)
{
    char *c;

    glRasterPos3f(x, y, z);

    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void displayGridAxis()
{
    if(axisOn) // Then, display the axis
    {
        // Axis
        glDisable(GL_LIGHTING);
        glColor3f(0.85, 0.85, 0.85);

        glBegin(GL_LINES);
            // X
            glVertex3f(-20.0, 0.0, 0.0);
            glVertex3f(20.0, 0.0, 0.0);
            // Y
            glVertex3f(0.0, -20.0, 0.0);
            glVertex3f(0.0, 20.0, 0.0);
            // Z
            glVertex3f(0.0, 0.0, -20.0);
            glVertex3f(0.0, 0.0, 20.0);

            glLineWidth(0.5);
        glEnd();

        glEnable(GL_LIGHTING);
    }

    if(gridOn) // Then, display the grid
    {
        glDisable(GL_LIGHTING);
        glColor3f(0.4, 0.4, 0.4);

        glBegin(GL_LINES);
            for(float d = -20.0; d <= 20.0; d += 2)
            {
                glVertex3f(-20.0, 0.0, d);
                glVertex3f(20.0, 0.0, d);

                glVertex3f(d, 0.0, -20.0);
                glVertex3f(d, 0.0, 20.0);
            }
        glEnd();

        glEnable(GL_LIGHTING);
    }
}

//////////////////////////////////////////////////////////////////////////

/** GLUT setup and initialization **/
void setupGlut(int *argc, char **argv)
{
    windowHeight = windowWidth = 600;

    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // Context initialization
    glutInitWindowSize(600, 600);                 // Size setting
    mainWindow = glutCreateWindow("SolarSystemSimulator");     // Window creation
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

void setLights()
{
    GLfloat light_position[] = {0.0, 5.0, 10.0, 0.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};

    GLfloat light2_position[] = {0.0, -5.0, 10.0, 0.0};
    GLfloat light2_specular[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT1, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light2_specular);
}

void setGL()
{
    printf("-> Setting shades, lights and anti-aliasing.\n\n");
    // Color and shades
    glClearColor(0.0f, 0.0f, 0.04f, 0.0f); // Define the color to clear the screen
    glShadeModel(GL_SMOOTH);

    // Enables and disables
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE); // Anti aliasing

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    setLights();
}

/** Deinitilization Function (if ESC was pressed)**/
void glutDeinit()
{
    glutDestroyWindow(mainWindow);
    exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////

/** CALLBACK for Display Function **/
void display()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Calculate the perspective projection matrix
    glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, currentAspect, 0.1, 100.0);

    // Now, calculate and transform the model view matrix
    glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        camera->positioning();
        solarSystem->display(detailsOn, projOn, mode);
        displayGridAxis();

    // Send to GPU vertex data and graphics commands
    glFlush();
}

/** CALLBACK for Resize Function **/
void reshape(GLint width, GLint height)
{
    // Save the new dimensions
    windowWidth  = width;
    windowHeight = height;

    // Calculate the new aspect of screen
    currentAspect = ((GLdouble)windowWidth)/windowHeight;


    glViewport(0, 0, width, height);
}

///////////////////////////////////////////////////////////////////////////////

/** CALLBACK for Timer Function **/
void timerDisplay(GLint value)
{
    glutPostRedisplay();
    glutTimerFunc(16, timerDisplay, 0);
}

void physicsTimer(GLint value)
{
    // Rotate camera
    camera->rotateOnMouse(mouseX, mouseY, windowWidth, windowHeight);
    solarSystem->interactBodies();
    glutTimerFunc(20, physicsTimer, 0);
}


/** CALLBACK for Keyboard keys and mouse**/
void keyPressed(unsigned char key, int x, int y)
{
    // Analyse the key and realize the desired action
    if(key == 27) // ESC
    {
        printf("ESC pressed.\n");
        glutDeinit();
    }

    else if(key == '1') // Toggle show axis
    {
        axisOn = !axisOn;
        printf("-> Toggle show axis.\n");
    }

    else if(key == '2') // Toggle show grid
    {
        gridOn = !gridOn;
        printf("-> Toggle show grid.\n");
    }
    else if(key == '3') // Toggle show details of each body
    {
        detailsOn = !detailsOn;
        printf("-> Toggle show details of each body.\n");
    }

    else if(key == '4')
    {
        if(mode < 2)
            mode++;
        else
            mode = 0;

        printf("-> Change vector type to: %d.\n", mode);
    }

    else if(key == '5') // Toggle show projections in XZ plan
    {
        projOn = !projOn;
        printf("-> Toggle show projections in XZ plan.\n");
    }

    else
        camera->translateOnKeyboard(key);
}

void specialKeyPressed(GLint key, GLint x, GLint y)
{
    switch(key)
    {
//    case GLUT_KEY_DOWN:
//        rotX -= 2;
//        break;
//    case GLUT_KEY_UP:
//        rotX += 2;
//        break;
//    case GLUT_KEY_LEFT:
//        rotY += 2;
//        break;
//    case GLUT_KEY_RIGHT:
//        rotY -= 2;
//        break;

    default:
        printf("< Nothing to do. >\n");
    }
}

void passiveMouse(int x, int y)
{
    // Save the  mouse coordinates
    mouseX = x;
    mouseY = y;
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    char *directory = argv[1];

    if(directory != 0)
    {
        // Initialization of objects
        printf("-> Loading the solar system at %s; \n", directory);
        solarSystem = new SolarSystem(directory);
        solarSystem->loadSystemFromFile();
        camera = new Camera();

        printf("-> Setting up the Graphics Window; \n");
        setupGlut(&argc, argv); // GLUT initialization
        setGL();

        /* Set the callback functions */
        // Graphics
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);

        // Keyboard, mouse and timers
        glutKeyboardFunc(keyPressed);       // Character keys
        glutSpecialFunc(specialKeyPressed); // Special keys pressed
        glutPassiveMotionFunc(passiveMouse);

        glutTimerFunc(16, timerDisplay, 0);
        glutTimerFunc(20, physicsTimer, 0);

        glutMainLoop();
    }

    else
    {
        printf("> No input file passed as argument.\n");
    }

    return 0;
}











