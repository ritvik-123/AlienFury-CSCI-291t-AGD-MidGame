#include "_scene.h"
#include <_lighting.h>
#include <_model.h>
#include <_inputs.h>
#include <_parallax.h>
#include <_skybox.h>
#include <_2dplyer.h>
#include <_camera.h>
#include <_3dmodelloader.h>
#include <_sounds.h>
#include <_timer.h>
#include <_obstacles.h>
#include<_textureloader.h>

// Global game objects
_lighting *myLight = new _lighting();
_model *myModel = new _model();
_inputs *myKbMs = new _inputs();
_parallax *prlx = new _parallax();
_parallax *prlx2 = new _parallax();
_parallax *prlx3 = new _parallax();
_parallax *prlx1 = new _parallax();

_skyBox *sky = new _skyBox();
_2DPlyer *ply2D = new _2DPlyer();
_camera *cam = new _camera();
_3dmodelloader * mdl3D = new _3dmodelloader();
_3dmodelloader * mdl3DW = new _3dmodelloader();
_sounds * snds = new _sounds();
_timer * tmr = new _timer();

// Game state variables
const int numObstacles = 100;
_obstacles obs[numObstacles];
int numCollisions = 0;
int numAvoided = -1;
bool gameEnded = false;
int currentLevel = 1;
bool levelCleared = false;
int frameDelayCounter = 0;
const int maxLevels = 3;

// Game states enum
enum GameState { LANDING_PAGE, MENU_PAGE, IN_GAME, PAUSED, HELP_PAGE, EXIT_GAME };
GameState currentState = LANDING_PAGE;

// Scene constructor/destructor
_scene::_scene() {
    currentState = LANDING_PAGE;
}
_scene::~_scene() {}

// Text rendering function
void renderBitmapString(float x, float y, void* font, const char* str) {
    glRasterPos2f(x, y);
    for (const char* c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Texture loader objects
_textureLoader *bTex1 = new _textureLoader();
_textureLoader *bTex2 = new _textureLoader();
_textureLoader *bTex3 = new _textureLoader();
_textureLoader *bTex4 = new _textureLoader();
_textureLoader *bTex5 = new _textureLoader();

// Load all textures for different screens
void loadAllTextures() {
    bTex1->loadImage("images/land.jpg");
    bTex2->loadImage("images/main.jpg");
    bTex3->loadImage("images/help.jpg");
    bTex4->loadImage("images/pause.jpg");
    bTex5->loadImage("images/exit.jpg");
}

// Render full screen texture
void renderFullScreenTexture(GLuint texID) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(800, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(800, 600);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 600);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Screen rendering functions
void renderLandingPage() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFullScreenTexture(bTex1->texID);
}

void renderMenuPage() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFullScreenTexture(bTex2->texID);
}

void renderHelpPage() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFullScreenTexture(bTex3->texID);
}

void renderPauseMenu() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFullScreenTexture(bTex4->texID);
}

void renderExitConfirmation() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderFullScreenTexture(bTex5->texID);
}

// Window resize handler
GLvoid _scene::resizeWindow(GLsizei width, GLsizei height) {
    GLfloat Ratio = (GLfloat)width/(GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, Ratio, 0.1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);
}

// OpenGL initialization
GLint _scene::IniGL() {
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    myLight->setupLight(GL_LIGHT0);
    myModel->initModel("images/skin.jpg");
    prlx->parallaxInit("images/prlx-3.jpg");
    prlx1->parallaxInit("images/prlx-6.jpg");
    prlx2->parallaxInit("images/prlx-5.jpg");
    prlx3->parallaxInit("images/prlx-4.jpg");
    ply2D->ply2Dinit("images/plm.png", 12, 4);
    mdl3D->initModel("models/alien/tris.md2");
    mdl3DW->initModel("models/alien/weapon.md2");

    loadAllTextures();

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    sky->skyBoxInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cam->camInit();

    snds->initSound();
    snds->playMusic("sounds/Wakanda.MP3");

    for (int i = 0; i < numObstacles; i++) {
        obs[i].initObstacle(-5 + i * 5, -2.7, -8.5f, 0.3f, "images/fire.jpeg");
    }
    return true;
}

// Draw level message
void drawLevelMessage(const char* message, float x, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    renderBitmapString(x, y, GLUT_BITMAP_HELVETICA_18, message);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Start new game
void startNewGame() {
    gameEnded = false;
    levelCleared = false;
    currentLevel = 1;
    numCollisions = 0;
    numAvoided = -1;

    for (int i = 0; i < numObstacles; i++) {
        obs[i].initObstacle(-5 + i * 5, -2.7, -8.5f, 0.3f, "images/fire.jpeg");
        obs[i].isVisible = true;
    }
}

// Main rendering function
GLvoid _scene::renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cam->setUPcamera();

    switch (currentState) {
        case LANDING_PAGE:
            renderLandingPage();
            break;
        case MENU_PAGE:
            renderMenuPage();
            break;
        case IN_GAME:
            switch(currentLevel) {
                case 1:
                case 2:
                case 3:
                    glPushMatrix();
                    glScalef(4.33, 4.33, 1);
                    glDisable(GL_LIGHTING);
                    (currentLevel == 1 ? prlx : currentLevel == 2 ? prlx2 : prlx3)->drawBkgrnd(dim.x, dim.y);
                    glEnable(GL_LIGHTING);
                    glPopMatrix();
                    break;
                default:
                    glPushMatrix();
                    glScalef(4.33, 4.33, 1);
                    glDisable(GL_LIGHTING);
                    prlx->drawBkgrnd(dim.x, dim.y);
                    glEnable(GL_LIGHTING);
                    glPopMatrix();
                    break;
            }

            glPushMatrix();
            glTranslatef(mdl3D->pos.x, mdl3D->pos.y - 1.7, mdl3D->pos.z + 1.5);
            glScalef(0.05, 0.05, 0.05);
            glRotatef(-90, 1, 0, 0);
            mdl3D->actions();
            mdl3DW->Draw();
            mdl3D->Draw();
            glPopMatrix();

            if (!gameEnded) {
                for (int i = 0; i < numObstacles; i++) {
                    obs[i].moveObstacle();
                    obs[i].drawObstacle();
                    obs[i].checkCollision(mdl3D, numCollisions, numAvoided, snds, "sounds/Big.mp3");
                }
            }

            myKbMs->updateParallax(prlx,0);
            myKbMs->updateParallax(prlx1,1);
            myKbMs->updateParallax(prlx2,0);
            myKbMs->updateParallax(prlx3,0);
            myKbMs->updatePlayer(mdl3D,snds, "sounds/jump1.wav");

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            gluOrtho2D(0, 800, 0, 600);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glDisable(GL_LIGHTING);
            glColor3f(1, 1, 1);
            char buffer[64];

            if(currentLevel <= maxLevels) {
                glColor3f(1, 1, 1);
                sprintf(buffer, "Level: %d", currentLevel);
                renderBitmapString(10, 510, GLUT_BITMAP_HELVETICA_18, buffer);
            }

            sprintf(buffer, "Collisions: %d", numCollisions);
            renderBitmapString(10, 570, GLUT_BITMAP_HELVETICA_18, buffer);

            sprintf(buffer, "Avoided: %d", numAvoided);
            renderBitmapString(10, 540, GLUT_BITMAP_HELVETICA_18, buffer);


            glEnable(GL_LIGHTING);

            if (!levelCleared && numAvoided >= 6) {
                levelCleared = true;
                gameEnded = true;
                frameDelayCounter = 0;
            }

            if (levelCleared) {
                char levelMsg[64];

                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                gluOrtho2D(0, 800, 0, 600);

                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_LIGHTING);
                glDisable(GL_DEPTH_TEST);
                glColor3f(1.0f, 0.44f, 0.0f);

                if (currentLevel <= maxLevels) {
                    sprintf(levelMsg, "Level %d Cleared!", currentLevel);
                    renderBitmapString(300, 300, GLUT_BITMAP_HELVETICA_18, levelMsg);
                } else {
                    renderBitmapString(300, 270, GLUT_BITMAP_HELVETICA_18, "All Levels Complete! Press Escape to Exit");
                }

                glEnable(GL_DEPTH_TEST);
                glEnable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);

                glPopMatrix();
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);

                frameDelayCounter++;

                if (frameDelayCounter > 120) {
                    currentLevel++;
                    if (currentLevel <= maxLevels) {
                        gameEnded = false;
                        levelCleared = false;
                        numAvoided = -1;
                        numCollisions = 0;

                        for (int i = 0; i < numObstacles; i++) {
                            obs[i].initObstacle(-5 + i * 5, -2.7, -8.5f, 0.3f, "images/fire.jpeg");
                            obs[i].isVisible = true;
                        }
                    }
                }
            }
            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            break;
        case PAUSED:
            renderPauseMenu();
            break;
        case HELP_PAGE:
            renderHelpPage();
            break;
        case EXIT_GAME:
            renderExitConfirmation();
            break;
    }
}

// Windows message handler
int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int mouseX, mouseY;
    switch (uMsg) {
    case WM_KEYDOWN:
        if (currentState == LANDING_PAGE) {
            if (wParam == VK_RETURN) {
                currentState = MENU_PAGE;
            }
        }
        else if (currentState == MENU_PAGE) {
            if (wParam == 'N' || wParam == 'n') {
                currentState = IN_GAME;
                startNewGame();
            } else if (wParam == 'H' || wParam == 'h') {
                currentState = HELP_PAGE;
            } else if (wParam == 'E' || wParam == 'e') {
                currentState = EXIT_GAME;
            }
        }
        else if (currentState == HELP_PAGE) {
            if (wParam == 'E' || wParam == 'e') {
                currentState = MENU_PAGE;
            }
        }
        else if (currentState == IN_GAME) {
            if (wParam == 'P' || wParam == 'p') {
                currentState = PAUSED;
            }
        }
        else if (currentState == PAUSED) {
            if (wParam == 'E' || wParam == 'e') {
                currentState = EXIT_GAME;
            } else if (wParam == 'R' || wParam == 'r') {
                currentState = IN_GAME;
            }
            else if (wParam == 'm' || wParam == 'M') {
                currentState = MENU_PAGE;
            }
        }
        else if (currentState == EXIT_GAME) {
            if (wParam == 'Y' || wParam == 'y') {
                PostQuitMessage(0);
            }
            else if (wParam == 'N' || wParam == 'n') {
                currentState = MENU_PAGE;
            }
        }
        myKbMs->wParam = wParam;
        myKbMs->keyPressed(myModel);
        myKbMs->keyPressed(prlx);
        myKbMs->keyPressed(prlx1);
        myKbMs->keyPressed(prlx2);
        myKbMs->keyPressed(prlx3);
        myKbMs->keyPressed(sky);
        myKbMs->keyPressed(ply2D);
        myKbMs->keyPressed(cam);
        myKbMs->keyPressed(mdl3D);
        myKbMs->keyPressed(snds, "sounds/Big.mp3");
        break;
    case WM_KEYUP:
        cout<<"J";
        mdl3D->actionTrigger = 0;
        myKbMs->wParam = wParam;
        myKbMs->keyUp(mdl3D);
        myKbMs->keyUp(prlx);
        myKbMs->keyUp(prlx1);
        myKbMs->keyUp(prlx2);
        myKbMs->keyUp(prlx3);
        break;
    case WM_LBUTTONDOWN:
        if (currentState == LANDING_PAGE) {
            currentState = MENU_PAGE;
        }
        else if (currentState == MENU_PAGE) {
            currentState = IN_GAME;
            startNewGame();
        }
        break;
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        myKbMs->wParam = wParam;
        myKbMs->mouseEventDown(myModel, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        myKbMs->mouseEventUp();
        break;
    case WM_MOUSEMOVE:
        myKbMs->mouseMove(sky, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEWHEEL:
        myKbMs->mouseWheel(myModel, (double)GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    }
    return 0;
}
