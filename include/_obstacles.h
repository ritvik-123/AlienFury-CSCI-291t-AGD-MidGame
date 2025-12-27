#ifndef _OBSTACLES_H
#define _OBSTACLES_H

#include<gl/gl.h>
#include<gl/glu.h>
#include<GL/glut.h>
#include<_textureloader.h>
#include<_3dmodelloader.h>
#include<_sounds.h>


class _obstacles {
public:
    float posX, posY, posZ; // Position of the obstacle
    float radius; // Radius of the obstacle

    _obstacles(); // Constructor
    ~_obstacles(); // Destructor
    _textureLoader *myTex = new _textureLoader();

    void initObstacle(float x, float y, float z, float r, char*);
    void moveObstacle(); // Function to move the obstacle horizontally
    void drawObstacle(); // Function to draw the obstacle (sphere)
    bool isVisible;      // To track visibility after collision
    bool hasCollided;    // To avoid double counting per sphere

    void checkCollision(_3dmodelloader* player, int& numCollisions, int& numAvoided,_sounds *,char *);



    bool collided = false;

};

#endif  // _OBSTACLES_H

