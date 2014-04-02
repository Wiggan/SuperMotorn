#include "Game.h"
#include <iostream>
#include "MeshComponent.h"
#include "DroneEntity.h"
#include "CameraComponent.h"
#include "VertexShader.h"
#include "LevelLoader.h"
#include "Texture.h"
#include "DebugRenderer.h"
#include "Configuration.h"
#include "HexaTerrain.h"
Game::Game(HWND pWindow, int width, int height, Timer* pTimer) : mWindow(pWindow),
mTimer(pTimer), mShowFps(false), mRenderer(pWindow, width, height), mWorld(&mRenderer) {
    using namespace std;
    cout << "Starting game" << endl;
    mCameras = &BaseCamera::mCameras;
    cout << "Initializing renderer" << endl;
    mRenderer.init(&mResourceLoader);
    //mResourceLoader.init(mRenderer.getDevice(), mRenderer.getContext());
#ifdef _DEBUG
    DebugRenderer::init(&mRenderer, &mResourceLoader);
#endif
    cout << "Loading config" << endl;
    mConfig = mResourceLoader.getResource<Configuration>(L"config.xml");
    cout << "Creating level loader" << endl;
    LevelLoader ll(mResourceLoader, mRenderer);
    ll.loadLevel(L"level01.xml", &mWorld);
    cout << "Creating network client" << endl;
    mClient.setListener(this);
    mConnected = mClient.connectTo(mConfig->getServerIp(), mConfig->getServerPort());
    if ( !mConnected ) {
        cout << "Failed connecting to server, creating local drone." << endl;
        DroneEntity* drone = new DroneEntity(0, 1);
        InputComponent* input = new InputComponent(NULL);
        drone->setStartPoint(mWorld.getStartPoint(2, 0));
        drone->add(input);
        mInputComponents.push_back(input);
        mWorld.add(drone);
    } else {
        cout << "Connected!" << endl;
    }
    cout << "Initializing world" << endl;
    mWorld.add(new HexaTerrain(L"terrain1.xml"));
    mWorld.init(&mResourceLoader);
    mRenderer.setActiveCamera(mCameras->back());
}
void 
Game::tick(float pDelta) {
    float delta = min(pDelta, 0.2);
    float start = mTimer->getTotalTime();
    mWorld.tick(delta, mTimer);
    mWorld.draw();
    mRenderer.begin();
    mRenderer.renderSolids();
    //mRenderer.renderTransparents();
    mRenderer.renderToBackBuffer();
    mRenderer.end();
    calcFps(delta);
    mClient.receive(2);
    float usedTime = mTimer->getTotalTimeNow() - start;
    
    int sleep = (int)(16.6666f - usedTime*1000);
    if ( sleep > 0.0f ) {
        Sleep(sleep);
    }
}
void
Game::calcFps(float pDelta) {
    mFrameCount++;
    if ( mTimer->getTotalTime() - mLastFpsTime >= 1.0f ) {
        if ( mShowFps ) {
            std::cout << "FPS: " << mFrameCount << std::endl;
        }
        mFrameCount = 0;
        mLastFpsTime += 1.0f;
        mResourceLoader.lookForChanges();
    }
}
void    
Game::keyDown(UINT pKey) {
    for ( auto it = mInputComponents.begin(); it != mInputComponents.end(); ++it ) {
        (*it)->keyDown(pKey);
    }
}
void    
Game::keyUp(UINT pKey) {
    for ( auto it = mInputComponents.begin(); it != mInputComponents.end(); ++it ) {
        (*it)->keyUp(pKey);
    }
}
void
Game::toggleFps() {
    mShowFps = !mShowFps;
}
void
Game::debug() {
#ifdef _DEBUG
    DebugRenderer::instance()->toggleActive();
#endif
}
void
Game::nextCamera() {
    mRenderer.setActiveCamera((*mCameras)[++mCurrentCamera % mCameras->size()]);
}
Entity*    
Game::onSelfConnected(int pPlayerId, int pTeam) {
    DroneEntity* drone = new DroneEntity(pPlayerId, pTeam);
    StartPoint* start = mWorld.getStartPoint(pTeam, pPlayerId);
    if ( start ) {
        drone->setStartPoint(start);
    }
    InputComponent* input = new InputComponent(&mClient);
    drone->add(input);
    mInputComponents.push_back(input);
    mWorld.add(drone);
    mDrones.push_back(drone);
    drone->init(&mRenderer, &mResourceLoader);
    mRenderer.setActiveCamera(mCameras->back());
    return drone;
}
void    
Game::onPlayerConnected(int pPlayerId, int pTeam, InputComponent* pInput) {
    DroneEntity* drone = new DroneEntity(pPlayerId, pTeam);
    StartPoint* start = mWorld.getStartPoint(pTeam, pPlayerId);
    if ( start ) {
        drone->setStartPoint(start);
    }
    drone->add(pInput);
    drone->init(&mRenderer, &mResourceLoader);
    mWorld.add(drone);
    mDrones.push_back(drone);
}
void    
Game::onPlayerDisconnected(int pPlayerId) {
    for ( auto it = mDrones.begin(); it != mDrones.end(); ++it ) {
        if ( (*it)->getPlayerId() == pPlayerId ) {
            mWorld.remove(*it);
            delete (*it);
            mDrones.erase(it);
            return;
        }
    }
}
Game::~Game() {
    /*for ( auto it = mCameras.begin(); it != mCameras.end(); it++ ) {
        delete (*it);
    }*/
}