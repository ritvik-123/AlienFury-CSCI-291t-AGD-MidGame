#ifndef _SPHERE_H
#define _SPHERE_H


class Sphere {
public:
    float x, y, z;
    float radius;

    Sphere(float x, float y, float z, float radius)
        : x(x), y(y), z(z), radius(radius) {}

    void draw() {
        glPushMatrix();
        glTranslatef(x, y, z);
        glutSolidSphere(radius, 20, 20);
        glPopMatrix();
    }

    void move(float dx) {
        x += dx;
    }
};

#endif // _SPHERE_H
