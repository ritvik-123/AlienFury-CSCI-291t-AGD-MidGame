#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_model.h>
#include<_parallax.h>
#include<_skybox.h>
#include<_2dplyer.h>
#include <_camera.h>
#include<_3dmodelloader.h>
#include<_sounds.h>
#include<_timer.h>
#include<_obstacles.h>

class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void keyPressed(_model *);
        void keyUp(_3dmodelloader *); // ToDo: address accordingly
        void keyUp(_parallax* prlx);
        void updateParallax(_parallax* prlx, bool);
        void updatePlayer(_3dmodelloader *,_sounds *,char *);


        void keyPressed(_parallax *);
        void keyPressed(_skyBox *);
        void keyPressed(_obstacles *, int);
        void keyPressed(_2DPlyer *);
        void keyPressed(_3dmodelloader *);
        void keyPressed(_camera *);
        void keyPressed(_sounds *,char *);

        void mouseEventDown(_model*, double,double);
        void mouseEventUp();// ToDo: address accordingly

        void mouseWheel(_model*,double);
        void mouseMove(_skyBox*,double,double);

        double prev_mouseX;
        double prev_mousey;

        bool mouse_translate; // flag to indicate mouse move
        bool mouse_rotate; //flag to indicate mouse rotation

        float jumpHeight = 1.0f; // Adjust as needed
        float groundLevel = 0.0f; // Original y-position
        bool isJumping = false;
        bool keyHeld = false; //
        bool isRightPressed = false;
        bool isUpPressed = false;


        WPARAM wParam;

    protected:

    private:
};

#endif // _INPUTS_H
