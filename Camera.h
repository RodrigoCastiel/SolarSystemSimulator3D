#ifndef CAMERA_H
#define CAMERA_H

/// GL libraries
#include <GL/gl.h>
#include <GL/glut.h>

/// C libraries
#include <cstdio>
#include <cstdlib>

/// Other libraries and classes
#include <math.h>

class Camera
{
    public:
        // Constructor
        Camera();

        // Display methods
        void positioning(); // Realize the necessary transformations to positioning the 3D models

        // Modification methods
        void rotateOnMouse(int x, int y, int screenWidth, int screenHeight); // Rotates the camera with the mouse position
        void translateOnKeyboard(unsigned char key);

    private:
        float x, y, z;    // The coordinates of camera
        float rotX, rotY; // The rotation of camera
};

#endif // CAMERA_H
