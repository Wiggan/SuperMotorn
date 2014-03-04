#pragma once
#include <winsock2.h>
#include "Client.h"
#include "Timer.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "World.h"
#include "InputComponent.h"
#include "DroneEntity.h"
class Configuration;
class Game : public NetworkListener {
    HWND                                mWindow;
    Renderer                            mRenderer;
    Configuration*                      mConfig;
    ResourceLoader                      mResourceLoader;
    World                               mWorld;
    std::vector<BaseCamera*>*           mCameras;
    std::vector<InputComponent*>        mInputComponents;
    std::vector<DroneEntity*>           mDrones;
    Client                              mClient;
    bool                                mConnected = false;
    Timer*                              mTimer;
    float                               mLastFpsTime = 0;
    int                                 mFrameCount = 0;
    bool                                mShowFps;
    int                                 mCurrentCamera = 0;

    void                            calcFps(float pDelta);
public:
    void                toggleFps();
    void                keyDown(UINT key);
    void                keyUp(UINT key);
    void                nextCamera();
    void                debug();
    void                tick(float pDelta);
    virtual Entity*     onSelfConnected(int pPlayerId, int pTeam);
    virtual void        onPlayerConnected(int pPlayerId, int pTeam, InputComponent* pInput);
    virtual void        onPlayerDisconnected(int pPlayerId);
                        Game(HWND pWindow, int width, int height, Timer* pTimer);
                        ~Game();
};

