#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include "Body.h"

#define     X_AXIS  1
#define     Y_AXIS  2
#define     Z_AXIS  3
#define   ABS_DIST  4

#define     G   0.001

class SolarSystem
{
    public:
        // Initialization methods
        SolarSystem(char *fileName);
        int loadSystemFromFile(); // Retrieve all the informations about each body

        // Graphics methods
        void display(int detailsOn, int projOn, int mode); // Display in depth buffer each body

        // Physics methods
        void interactBodies(); // Calculates all the interactions two by two

    private:
        Body **body;    // The vector which saves the pointers to each body object
        int bodyNumber; // The number of bodies in solar system
        char *sysDoc; // The file that contains all the informations to the planets
};

#endif // SOLARSYSTEM_H
