#include "Body.h"

/** IMPLEMENTATION **/

///////////////////////////////////////////////////////////////////////////////////

void displayAuxiliarData(float radius, double deltaX, double deltaY, double deltaZ)
{
    glDisable(GL_LIGHTING);

    // Vector speed
    glBegin(GL_LINES);
        glColor3f(1, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(100 * deltaX, 100 * deltaY, 100 * deltaZ);
    glEnd();

    // Axis
    glBegin(GL_LINES);
        // X
        glColor3f(1.0, 0, 0.0);
        glVertex3f(2 * radius, 0, 0);
        glVertex3f(0, 0, 0);

        // Y
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0, 2 * radius, 0);
        glVertex3f(0, 0, 0);

        // Z
        glColor3f(0.0, 0, 1.0);
        glVertex3f(0, 0, 2 * radius);
        glVertex3f(0, 0, 0);
    glEnd();

    // Cube
    glColor3f(1.0, 0, 1.0);
    glutWireCube(radius * 2);

    glEnable(GL_LIGHTING);
}

//////////////////////////////////////////////////////////////////

Body::Body(float X = 0, float Y = 0, float Z = 0)
{
    // Set default values
    fx = fy = fz = 0.0;    // 0 force
    ax = ay = az = 0.0;    // 0 for acceleration
    dx = dy = dz = 0.0;    // 0 for speed
    x = X;       // Origin for space
    y = Y;       // Origin for space
    z = Z;       // Origin for space
    mass = 1.0;
    radius = 0.5;

    // Allocate a material struct
    material = new Material;

    if(material != 0)
    {
        for(int i = 0; i < 4; i++) // Default values
        {
            material->diffuse [i] = 0.4;
            material->specular[i] = 0.6;
        }

        material->shininess[0] = 20.0;
    }
}

//////////////////////////////////////////////////////////////////

inline void Body::printCoord()
{
    printf("Coord: (%f, %f, %f).\n", x, y, z);
}

void Body::printForce()
{
    printf("Force: (%f, %f, %f).\n", fx, fy, fz);
}

inline void Body::printAccel()
{
    printf("Accel: (%f, %f, %f).\n", ax, ay, az);
}

inline void Body::printSpeed()
{
    printf("Speed: (%f, %f, %f).\n", dx, dy, dz);
}

void Body::printData()
{
    printf("\n-- Body --\n");
    printf("Mass:   %f.\n", mass);
    printf("Radius: %f.\n", radius);

    this->printCoord();
    this->printSpeed();
    this->printAccel();

    printf("Specular: (%f, %f, %f, %f).\n", material->specular[0], material->specular[1], material->specular[2], material->specular[3]);
    printf("Diffuse:  (%f, %f, %f, %f).\n", material->diffuse[0], material->diffuse[1], material->diffuse[2], material->diffuse[3]);

    printf("Shininnes: (%f).\n", material->shininess[0]);
}

//////////////////////////////////////////////////////////////////

void Body::setMaterial()
{
    glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material->shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse);
}

//////////////////////////////////////////////////////////////////

void Body::display(int detailsOn, int projOn, int mode)
{
    glPushMatrix();
        glTranslated(x, y, z);
        // Display axis and auxiliar data on the screen
        if(detailsOn)
        {
            if(mode == SHOW_ACCEL_VECTOR)
                displayAuxiliarData(radius, ax, ay, az);

            else if(mode == SHOW_FORCE_VECTOR)
                displayAuxiliarData(radius, fx, fy, fz);

            else if(mode == SHOW_SPEED_VECTOR)
                displayAuxiliarData(radius, dx, dy, dz);

        }

        if(projOn)
        {
            glDisable(GL_LIGHTING);
                glColor3fv(material->diffuse);

                glBegin(GL_LINES);
                    glVertex3f(radius, -y, radius);
                    glVertex3f(-radius, -y, -radius);

                    glVertex3f(radius, -y, -radius);
                    glVertex3f(-radius, -y, radius);
                glEnd();

            glEnable(GL_LIGHTING);
        }

        this->setMaterial();
        glutSolidSphere(radius, 24, 24);
    glPopMatrix();
}

//////////////////////////////////////////////////////////////////

void Body::updatePosition()
{
    // Update acceleration
    ax = fx/mass;
    ay = fy/mass;
    az = fz/mass;

    // Update speed
    dz += az;
    dy += ay;
    dx += ax;

    // Update coordinates
    z += dz;
    y += dy;
    x += dx;
}

//////////////////////////////////////////////////////////////////

void Body::loadPropertiesFromFile(FILE *document)
{
    // Inherent properties
    fscanf(document, "%f ", &mass);
    fscanf(document, "%f ", &radius);

    // Initial coordinates
    fscanf(document, "%lf ", &x);
    fscanf(document, "%lf ", &y);
    fscanf(document, "%lf ", &z);

    // Initial speed
    fscanf(document, "%lf ", &dx);
    fscanf(document, "%lf ", &dy);
    fscanf(document, "%lf ", &dz);

    // Materials
    for(int i = 0; i < 4; i++)
        fscanf(document, "%f", &material->specular[i]);

    for(int i = 0; i < 4; i++)
        fscanf(document, "%f", &material->diffuse[i]);

    fscanf(document, "%f", &material->shininess[0]);
}





