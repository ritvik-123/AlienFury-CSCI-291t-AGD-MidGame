#include "_obstacles.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <GL/glut.h>

// Constructor: Initialize obstacle properties
_obstacles::_obstacles() {
    posX = 0.0f;
    posY = -1.7f;
    posZ = -8.5f;
    radius = 1.0f;
    isVisible = true;
    hasCollided = false;
    myTex = new _textureLoader();
}

// Destructor
_obstacles::~_obstacles() {
    // Currently no dynamic resources to clean up
}

// Initialize obstacle with position, size and texture
void _obstacles::initObstacle(float x, float y, float z, float r, char* fileName) {
    myTex->loadImage(fileName);
    posX = x;
    posY = y;
    posZ = z;
    radius = r;
    isVisible = true;
    hasCollided = false;
}

// Move obstacle left and handle respawn
void _obstacles::moveObstacle() {
    if (!isVisible) return;

    posX -= 0.06f; // Movement speed

    // Respawn obstacle when it goes off-screen
    if (posX < -100.0f) {
        posX = 100.0f + static_cast<float>(rand() % 5); // Randomize respawn position
        isVisible = true;
        hasCollided = false;
    }
}

// Draw the obstacle as a textured sphere
void _obstacles::drawObstacle() {
    if (!isVisible) return;

    glPushMatrix();
    myTex->textureBinder();
    glTranslatef(posX, posY, posZ);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, radius, 40, 40); // Draw sphere with 40 slices and stacks

    gluDeleteQuadric(quad);
    glPopMatrix();
}

// Check collision between obstacle and player
void _obstacles::checkCollision(_3dmodelloader* player, int& numCollisions,
                               int& numAvoided, _sounds* snds, char *fileName) {
    if (!isVisible || hasCollided) return;

    // Calculate distance to player
    float dx = posX - player->pos.x;
    float dy = posY - player->pos.y;
    float distance = sqrt(dx * dx + dy * dy);

    float sumRadii = radius + 2.5f; // Collision threshold

    if (distance < sumRadii) {
        // Collision detected
        numCollisions++;
        isVisible = false;
        hasCollided = true;
        snds->PlaySoundA(fileName);
    }
    else if (posX < player->pos.x && !hasCollided) {
        // Obstacle avoided (passed player)
        numAvoided++;
        hasCollided = true; // Prevent multiple counts
    }
}
