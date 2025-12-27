#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "_3dmodelloader.h"
#include <stdlib.h>

class Obstacle {
public:
    _3dmodelloader model;     // 3D model for the obstacle
    float speed;              // Movement speed towards player
    bool isActive;           // Active state flag
    float bounds[3];         // Collision bounds (width, height, depth)

    Obstacle(float startX, float startY, float startZ,
            float moveSpeed, const char* modelFile);
    void update(float deltaTime);
    bool checkCollision(const _3dmodelloader& player);
};

#endif
