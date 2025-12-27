#include "_inputs.h"

// Constructor: Initialize input states
_inputs::_inputs()
{
    mouse_translate = false;
    mouse_rotate = false;

    // Jump parameters initialization
    jumpHeight = 1.0f;
    groundLevel = 0.0f;
    isJumping = false;
    keyHeld = false;
}

// Destructor
_inputs::~_inputs()
{
    // Empty destructor
}

// Handle key presses for model rotation
void _inputs::keyPressed(_model* mdl)
{
    switch(wParam)
    {
        case VK_LEFT:
            mdl->rot.y += 1.0;
            break;
        case VK_RIGHT:
            mdl->rot.y -= 1.0;
            break;
        case VK_UP:
            mdl->rot.x += 1.0;
            break;
        case VK_DOWN:
            mdl->rot.x -= 1.0;
            break;
    }
}

// Handle key presses for parallax movement
void _inputs::keyPressed(_parallax* prlx)
{
    switch(wParam)
    {
        case VK_RIGHT:
            isRightPressed = true;
            break;
    }
}

// Handle key releases for parallax movement
void _inputs::keyUp(_parallax* prlx)
{
    switch(wParam)
    {
        case VK_RIGHT:
            isRightPressed = false;
            break;
    }
}

// Update parallax background based on input
void _inputs::updateParallax(_parallax* prlx, bool Front)
{
    if (isRightPressed && !Front)
    {
        prlx->xMin += 0.01;
        prlx->xMax += 0.01;
    }
    else if (isRightPressed && Front)
    {
        prlx->xMin += 0.01;
        prlx->xMax += 0.01;
    }
}

// Handle key presses for 3D player movement
void _inputs::keyPressed(_3dmodelloader* ply)
{
    switch(wParam)
    {
        case VK_RIGHT:
            isRightPressed = true;
            break;
        case VK_UP:
            if (!isJumping && !keyHeld)
            {
                isJumping = true;
                keyHeld = true;
                ply->pos.y += jumpHeight;
                ply->actionTrigger = ply->JUMP;
            }
            break;
        default:
            ply->actionTrigger = ply->STAND;
            break;
    }
}

// Handle key releases for 3D player movement
void _inputs::keyUp(_3dmodelloader* ply)
{
    switch(wParam)
    {
        case VK_UP:
            if(isJumping)
            {
                isJumping = false;
                keyHeld = false;
                ply->pos.y -= jumpHeight;
                ply->actionTrigger = ply->JUMP;
            }
            break;
        case VK_RIGHT:
            isRightPressed = false;
            break;
    }
}

// Update player state based on input
void _inputs::updatePlayer(_3dmodelloader* ply, _sounds* snds, char *fileName)
{
    if (isRightPressed)
    {
        ply->actionTrigger = ply->RUN;
    }
    if (isRightPressed && isJumping)
    {
        ply->actionTrigger = ply->JUMP;
        snds->PlaySoundA(fileName);
    }
    if (keyHeld && isJumping)
    {
        ply->actionTrigger = ply->JUMP;
        snds->PlaySoundA(fileName);
    }
    if (!isRightPressed && !isJumping)
    {
        ply->actionTrigger = ply->STAND;
    }
}

// Handle key presses for skybox rotation
void _inputs::keyPressed(_skyBox* sky)
{
    switch(wParam)
    {
        case VK_LEFT:
            sky->rot.y -= 1.0;
            break;
        case VK_RIGHT:
            sky->rot.y += 1.0;
            break;
        case VK_UP:
            sky->rot.x += 1.0;
            break;
        case VK_DOWN:
            sky->rot.x -= 1.0;
            break;
        case VK_F2:
            sky->pos.z += 1.0;
            break;
        case VK_F3:
            sky->pos.z -= 1.0;
            break;
    }
}

// Handle key presses for 2D player movement
void _inputs::keyPressed(_2DPlyer* ply)
{
    switch(wParam)
    {
        case VK_LEFT:
            ply->actiontrigger = ply->WALKLEFT;
            break;
        case VK_RIGHT:
            ply->actiontrigger = ply->WALKRIGHT;
            break;
        case VK_UP:
            ply->actiontrigger = ply->WALKFRONT;
            break;
        case VK_DOWN:
            ply->actiontrigger = ply->WALKBACK;
            break;
    }
}

// Handle key presses for sound effects
void _inputs::keyPressed(_sounds* snds, char *fileName)
{
    switch(wParam)
    {
        case VK_SPACE:
            snds->PlaySoundA(fileName);
            break;
    }
}

// Handle key presses for camera movement
void _inputs::keyPressed(_camera* cm)
{
    switch(wParam)
    {
        case 0x57: // W
            cm->camMoveFdBd(-1);
            break;
        case 0x53: // S
            cm->camMoveFdBd(1);
            break;
        case 65: // A
            cm->camMoveLtRt(-1);
            break;
        case 68: // D
            cm->camMoveLtRt(1);
            break;
        case VK_SPACE:
            cm->camReset();
            break;
        case 73:
            cm->rotAngle.x -= 1;
            cm->rotationXY();
            break;
        case 74:
            cm->rotAngle.y -= 1;
            cm->rotationXY();
            break;
        case 75:
            cm->rotAngle.x += 1;
            cm->rotationXY();
            break;
        case 76:
            cm->rotAngle.y += 1;
            cm->rotationXY();
            break;
    }
}

// Handle mouse button presses
void _inputs::mouseEventDown(_model* mdl, double x, double y)
{
    switch(wParam)
    {
        case MK_LBUTTON:
            mouse_rotate = true;
            break;
        case MK_RBUTTON:
            mouse_translate = true;
            break;
        case MK_MBUTTON:
            break;
    }
    prev_mouseX = x;
    prev_mousey = y;
}

// Handle mouse button releases
void _inputs::mouseEventUp()
{
    mouse_rotate = false;
    mouse_translate = false;
}

// Handle mouse wheel movement
void _inputs::mouseWheel(_model* mdl, double delta)
{
    mdl->pos.z += delta/100.0;
}

// Handle mouse movement for skybox
void _inputs::mouseMove(_skyBox* mdl, double x, double y)
{
    if(mouse_rotate)
    {
        mdl->rot.y += (x-prev_mouseX)/3.0;
        mdl->rot.x += (y-prev_mousey)/3.0;
    }
    if(mouse_translate)
    {
        mdl->pos.x += (x-prev_mouseX)/100.0;
        mdl->pos.y -= (y-prev_mousey)/100.0;
    }
    prev_mouseX = x;
    prev_mousey = y;
}
