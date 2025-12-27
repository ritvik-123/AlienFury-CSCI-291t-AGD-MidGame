#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>
#include <vector>
#include "_obstruction.h"

class _scene
{
    public:
        _scene();
        virtual ~_scene();

        GLvoid resizeWindow(GLsizei width, GLsizei height);// resizing window
        GLint IniGL();                                    //initialization
        GLvoid renderScene();
        std::vector<Obstacle> obstacles;
        int avoidedCount;
        float spawnTimer;                            // Draw Scene

        int winMsg(HWND	hWnd,			// Handle For This Window
                   UINT	uMsg,			// Message For This Window
                   WPARAM wParam,			// Additional Message Information
                   LPARAM	lParam);

         vec2 dim;
    protected:

    private:
};

#endif // _SCENE_H
