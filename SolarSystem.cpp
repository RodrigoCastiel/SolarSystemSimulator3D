#include "SolarSystem.h"


double distanceBetween(Body *b1, Body *b2, int axis)
{
    if(axis == X_AXIS)
        return b1->x - b2->x;
    else if(axis == Y_AXIS)
        return b1->y - b2->y;
    else if(axis == Z_AXIS)
        return b1->z - b2->z;

    return sqrtf((b1->x - b2->x)*(b1->x - b2->x) + (b1->y - b2->y)*(b1->y - b2->y) + (b1->z - b2->z)*(b1->z - b2->z));
}

double absd(double n)
{
    if(n < 0)
        return -n;

    return n;
}

double attraction(Body *b1, Body *b2, double d)
{
//    if(d == 0) // Avoid division by 0
//        d = 1.0; // Default value

    return -(G * 1000.0 *b1->mass * b2->mass)/(100000.0 * d*d);
}

///////////////////////////////////////////////////////////////////////////////////////////////

SolarSystem::SolarSystem(char *fileName)
{
    sysDoc = fileName;
}

int SolarSystem::loadSystemFromFile() // Returns 1 if loading was sucessfuly, 0 otherwaise
{
    // Open the file to read
    FILE *document = fopen(sysDoc, "r");

    if(document != 0) // Open sucessfuly
    {
        // Get the body number
        fscanf(document, "%d", &bodyNumber);
        printf("> BodyNumber: %d. \n", bodyNumber);

        // Allocate the vector of bodies
        body = new Body* [bodyNumber];

        for(int i = 0; i < bodyNumber; i++) // Load each body
        {
            body[i] = new Body(0, 0, 0); // Allocate each body

            if(body[i] != 0) // If the allocation was sucessfully realized
            {
                body[i]->loadPropertiesFromFile(document);
                body[i]->printData();
            }

            else
                printf("> Memory allocation error!\n");
        }

        fclose(document); // Close the document to avoid loss of information

        return 1;
    }

    else // Did not open sucessfuly
    {
        printf("> Wrong directory: %s\n", sysDoc);

        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void SolarSystem::display(int detailsOn, int projOn, int mode)
{
    for(int i = 0; i < bodyNumber; i++) // Display each body
    {
        if(body[i] != 0) // If the current body was sucessfuly allocated
            body[i]->display(detailsOn, projOn, mode);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

void SolarSystem::interactBodies()
{
    static double force, deltaX, deltaY, deltaZ, fx, fy, fz, dist3d;

    for(int i = 0; i < bodyNumber; i++)
    {
        if(body[i] != 0)
            (body[i]->fx) = (body[i]->fy) = (body[i]->fz) = 0.0;
    }

    // Calculate the attraction between all two pairs of bodies
    for(int i = 0; i < bodyNumber; i++)
    {
        for(int j = i; j < bodyNumber; j++)
        {
            if((body[i] != 0) and (body[j] != 0))
            {
                deltaX = distanceBetween(body[i], body[j], X_AXIS);
                deltaY = distanceBetween(body[i], body[j], Y_AXIS);
                deltaZ = distanceBetween(body[i], body[j], Z_AXIS);

                dist3d = distanceBetween(body[i], body[j], ABS_DIST);
                force  = absd(attraction(body[i], body[j], dist3d));

                if(dist3d != 0)
                {
                    fx = force * (deltaX/dist3d);
                    fy = force * (deltaY/dist3d);
                    fz = force * (deltaZ/dist3d);
                }

                body[i]->fx -= fx;
                body[i]->fy -= fy;
                body[i]->fz -= fz;

                body[j]->fx += fx;
                body[j]->fy += fy;
                body[j]->fz += fz;
            }
        }
    }

    for(int i = 0; i < bodyNumber; i++)
    {
        if(body[i] != 0)
        {
            body[i]->updatePosition();
           // body[i]->printSpeed();
        }
    }
}







