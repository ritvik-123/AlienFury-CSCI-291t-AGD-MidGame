#include "_parallax.h"

// Constructor: Initialize texture coordinates
_parallax::_parallax()
{
    xMin = 0.0;
    xMax = 1.0;
    yMin = 0.0;
    yMax = 1.0;
}

// Destructor
_parallax::~_parallax()
{
    // Currently no resources to clean up
}

// Draw the parallax background
void _parallax::drawBkgrnd(float width, float height)
{
    glColor3f(1.0, 1.0, 1.0);       // White color (no tint)
    bTex->textureBinder();           // Bind the background texture

    // Draw textured quad
    glBegin(GL_POLYGON);
        glTexCoord2f(xMin, yMax);    // Bottom-left texture coordinate
        glVertex3f(-width / height, -1, -10);  // Bottom-left vertex

        glTexCoord2f(xMax, yMax);    // Bottom-right texture coordinate
        glVertex3f(width / height, -1, -10);   // Bottom-right vertex

        glTexCoord2f(xMax, yMin);    // Top-right texture coordinate
        glVertex3f(width / height, 1, -10);     // Top-right vertex

        glTexCoord2f(xMin, yMin);    // Top-left texture coordinate
        glVertex3f(-width / height, 1, -10);    // Top-left vertex
    glEnd();
}

// Initialize parallax with texture
void _parallax::parallaxInit(char* fileName)
{
    bTex->loadImage(fileName);  // Load background texture
}

// Auto-scroll the parallax background
void _parallax::prlxScrollAuto(string dir, float speed)
{
    if(tmr->getTicks() > 50)  // Throttle scrolling with timer
    {
        if(dir == "left")
        {
            xMin += speed;
            xMax += speed;
        }
        else if(dir == "right")
        {
            xMin -= speed;
            xMax -= speed;
        }
        else if(dir == "up")
        {
            yMin -= speed;
            yMax -= speed;
        }
        else if(dir == "down")
        {
            yMin += speed;
            yMax += speed;
        }
        tmr->reset();  // Reset the timer
    }
}
