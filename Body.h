/// C++ libraries
#include <iostream>

/// GL libraries
#include <GL/gl.h>
#include <GL/glut.h>

/// C libraries
#include <cstdio>
#include <cstdlib>

/// Other libraries
#include <math.h>

#ifndef BODY_H
#define BODY_H

#define     SHOW_FORCE_VECTOR   0
#define     SHOW_SPEED_VECTOR   1
#define     SHOW_ACCEL_VECTOR   2

typedef struct Material // A struct to save the material properties
{
    float specular [4];
    float diffuse  [4];
    float shininess[1];

} Material;

/* Auxiliar functions */
void displayAuxiliarData(float radius, double deltaX, double deltaY, double deltaZ);

/* The main class for a body */
class Body
{
    public:
        // Inherent properties
        float mass;         // Mass
        float radius;       // Radius of sphere
        Material *material;       // The material corresponding integer

        // Temporary properties
        double x, y, z;     // Coordinates
        double dx, dy, dz;  // Speed components
        double ax, ay, az;  // Acceleration components
        double fx, fy, fz;  // Force components

        Body(float, float, float);

        // Auxiliar methods
        void printData();   // Prints the data
        inline void printCoord();  // Prints the coordinates
        inline void printSpeed();  // Prints the speed components
        inline void printAccel();  // Prints the acceleration components
        void printForce();  // Prints the force components
        void loadPropertiesFromFile(FILE *document); // loadPropertiesFromFile

        // Physics methods
        void updatePosition();

        // Graphics methods
        void setMaterial();
        void display(int detailsOn, int projOn, int mode);
};


#endif // BODY_H











