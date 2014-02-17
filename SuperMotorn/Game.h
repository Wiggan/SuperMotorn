#pragma once
#include <Windows.h>
#include "Timer.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "World.h"
#include "InputComponent.h"
class Game {
    HWND                            mWindow;
    Renderer                        mRenderer;
    ResourceLoader                  mResourceLoader;
    World                           mWorld;
    std::vector<BaseCamera*>        mCameras;
    std::vector<InputComponent*>    mInputComponents;
    Timer*                          mTimer;
    float                           mLastFpsTime = 0;
    int                             mFrameCount = 0;
    bool                            mShowFps;
    int                             mCurrentCamera = 0;

    void                            calcFps(float pDelta);
public:
    void    toggleFps();
    void    keyDown(UINT key);
    void    keyUp(UINT key);
    void    nextCamera();
    void    debug();
    void    tick(float pDelta);
            Game(HWND pWindow, int width, int height, Timer* pTimer);
            ~Game();
};

