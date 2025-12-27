#include "_spheres.h"

void Sphere::draw() {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

void Sphere::move(float dx) {
    x += dx;
}
