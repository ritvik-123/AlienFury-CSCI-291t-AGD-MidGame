#include "_obstruction.h"

Obstacle::Obstacle(float startX, float startY, float startZ,
                 float moveSpeed, const char* modelFile) : speed(moveSpeed) {
    model.initModel(modelFile);
    model.pos.x = startX;
    model.pos.y = startY;
    model.pos.z = startZ;
    isActive = true;

    // Set collision bounds (adjust based on your model size)
    bounds[0] = 0.5f;  // Width
    bounds[1] = 0.5f;  // Height
    bounds[2] = 0.5f;  // Depth
}

void Obstacle::update(float deltaTime) {
    if(isActive) {
        // Move towards player (assuming player is at origin)
        model.pos.z -= speed * deltaTime;

        // Deactivate when past player
        if(model.pos.z < -10.0f) {
            isActive = false;
        }
    }
}

bool Obstacle::checkCollision(const _3dmodelloader& player) {
    if(!isActive) return false;

    // Player collision bounds (adjust based on player model)
    const float playerBounds[3] = {0.3f, 0.7f, 0.3f};

    // AABB Collision detection
    bool collisionX = (model.pos.x - bounds[0] < player.pos.x + playerBounds[0]) &&
                     (model.pos.x + bounds[0] > player.pos.x - playerBounds[0]);

    bool collisionY = (model.pos.y - bounds[1] < player.pos.y + playerBounds[1]) &&
                     (model.pos.y + bounds[1] > player.pos.y - playerBounds[1]);

    bool collisionZ = (model.pos.z - bounds[2] < player.pos.z + playerBounds[2]) &&
                     (model.pos.z + bounds[2] > player.pos.z - playerBounds[2]);

    return collisionX && collisionY && collisionZ;
}
