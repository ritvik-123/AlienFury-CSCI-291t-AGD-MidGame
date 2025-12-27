#include "_spheres.h"

MovingSphere::MovingSphere() : x(0.0f), y(0.0f), z(0.0f), speed(0.05f) {
    // Initialize sphere's position and speed
}

void MovingSphere::move() {
    x += speed;  // Move to the right
    if (x > 50.0f) {  // If the sphere goes off-screen (to the right), reset its position
        x = -50.0f;   // Reset to the left side of the screen
    }
}

void MovingSphere::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);  // Move the sphere to its current position
    glutSolidSphere(1.0, 20, 20);  // Draw the sphere (radius, slices, stacks)
    glPopMatrix();
}
