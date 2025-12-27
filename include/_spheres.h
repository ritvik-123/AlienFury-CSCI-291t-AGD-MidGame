#ifndef SPHERES_H
#define SPHERES_H

#include <GL/glut.h>  // Include GLUT header for drawing the sphere

class MovingSphere {
public:
    float x, y, z;  // Position of the sphere
    float speed;     // Speed at which the sphere moves horizontally

    // Constructor to initialize the sphere
    MovingSphere();

    // Method to move the sphere horizontally
    void move();

    // Method to draw the sphere
    void draw();
};

#endif  // SPHERES_H
