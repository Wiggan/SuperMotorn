#pragma once
#include <winsock2.h>
#include "Client.h"
#include "Timer.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "World.h"
#include "InputInterface.h"
#include "DroneEntity.h"
#ifdef _DEBUG
    #include "HexagonTool.h"
    #include "DebugCamera.h"
#endif
class Configuration;
class Game : public NetworkListener {
    HWND                                mWindow;
    Renderer                            mRenderer;
    Configuration*                      mConfig;
    ResourceLoader                      mResourceLoader;
    World                               mWorld;
    std::vector<InputInterface*>        mInputs;
    std::vector<DroneEntity*>           mDrones;
    Client                              mClient;
    bool                                mConnected = false;
    Timer*                              mTimer;
    float                               mLastFpsTime = 0;
    int                                 mFrameCount = 0;
    bool                                mShowFps;
    int                                 mCurrentCamera = 0;
#ifdef _DEBUG
    DebugCamera*                        mDebugCamera;
    std::vector<HexagonTool*>           mTools;
    HexagonTool*                        mCurrentTool;
#endif

    void                            calcFps(float pDelta);
public:
    void                toggleFps();
    void                keyDown(UINT key);
    void                keyUp(UINT key);
    void                mouseMoved(UINT x, UINT y, UINT winX, UINT winY);
    void                mouseWheel(int delta);
    void                mouseDown(UINT button);
    void                nextCamera();
    void                debug();
    void                tick(float pDelta);
    virtual Entity*     onSelfConnected(int pPlayerId, int pTeam);
    virtual void        onPlayerConnected(int pPlayerId, int pTeam, InputComponent* pInput);
    virtual void        onPlayerDisconnected(int pPlayerId);
                        Game(HWND pWindow, int width, int height, Timer* pTimer);
                        ~Game();
};

