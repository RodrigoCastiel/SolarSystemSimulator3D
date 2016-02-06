#include "Camera.h"

float absf(float n)
{
    if(n < 0)
        return -n;

    return n;
}

double rad(double deg)
{
    return (3.14159265 * deg)/180.0;
}

Camera::Camera()
{
    x = y = z = 0; // Positioning in the origin
    rotX = rotY;
}

void Camera::rotateOnMouse(int X, int Y, int screenWidth, int screenHeight)
{
    // Translate and rotate to other system coordinate, which has the origin in the centre
    X = X - screenWidth/2;
    Y = -Y + screenHeight/2;

    float kY = (float)Y/(float)screenHeight;
    float kX = (float)X/(float)screenWidth;

    // First, analyse if the mouse pointer coordinates exceed the threshold
    if(absf(kY) > 0.1) // Rotate X
    {
        rotX += 2 * kY;
    }

    if(absf(kX) > 0.1) // Rotate Y
    {
        rotY -= 2 * kX;
    }

    if(rotX > 360.0)
        rotX -= 360.0;
    else if(rotX < -360.0)
        rotX += 360.0;

    if(rotY > 360.0)
        rotY -= 360.0;
    else if(rotY < -360.0)
        rotY += 360.0;

   // printf("> rotX = %f.\n", rotX);
}

void Camera::positioning()
{
    glRotated(-rotX, 1, 0, 0);
    glRotated(-rotY, 0, 1, 0);
    glTranslatef(x, y - 2, z);
}

void Camera::translateOnKeyboard(unsigned char key)
{
    if((key == 'w') or (key == 'W')) // UP
    {
        x += sin(rad(rotY)) * 0.1;
        y -= sin(rad(rotX)) * 0.06;
        z += cos(rad(rotY)) * 0.1;
    }

    else if((key == 's') or (key == 'S')) // DOWN
    {
        x -= sin(rad(rotY)) * 0.1;
        y += sin(rad(rotX)) * 0.06;
        z -= cos(rad(rotY)) * 0.1;
    }

    else if((key == 'a') or (key == 'A')) // LEFT
    {
        x += cos(rad(rotY)) * 0.1;
        z -= sin(rad(rotY)) * 0.1;
    }

    else if((key == 'd') or (key == 'D')) // RIGHT
    {
        x -= cos(rad(rotY)) * 0.1;
        z += sin(rad(rotY)) * 0.1;
    }

    else
        printf("< Nothing to do. >\n");
}











