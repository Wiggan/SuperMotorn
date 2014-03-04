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
Game::Game(HWND pWindow, int width, int height, Timer* pTimer) : mWindow(pWindow),
mTimer(pTimer), mShowFps(false), mRenderer(pWindow, width, height), mWorld(&mRenderer) {
    mCameras = &BaseCamera::mCameras;
    mRenderer.init();
    mResourceLoader.init(mRenderer.getDevice(), mRenderer.getContext());
#ifdef _DEBUG
    DebugRenderer::init(&mRenderer, &mResourceLoader);
#endif
    mConfig = mResourceLoader.getResource<Configuration>(L"config.xml");
    mClient.setListener(this);
    mConnected = mClient.connectTo(mConfig->getServerIp(), mConfig->getServerPort());
    if ( !mConnected ) {
        DroneEntity* drone = new DroneEntity(0);
        drone->setPosition(Vector3(2.0f, 2.0f, -10.0f));
        InputComponent* input = new InputComponent(&mClient);
        drone->add(input);
        mInputComponents.push_back(input);
        mWorld.add(drone);
    } else {
        std::cout << "Failed connecting to server" << std::endl;
    }
    LevelLoader ll(mResourceLoader, mRenderer);
    ll.loadLevel(L"level01.xml", &mWorld);
    mWorld.init(&mResourceLoader);
    mRenderer.setActiveCamera(mCameras->back());
}
void 
Game::tick(float pDelta) {
    float start = mTimer->getTotalTime();
    mWorld.tick(pDelta, mTimer);
    mWorld.draw();
    mRenderer.begin();
    mRenderer.renderSolids();
    mRenderer.end();
    calcFps(pDelta);
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
    DebugRenderer::instance()->toggleActive();
}
void
Game::nextCamera() {
    mRenderer.setActiveCamera((*mCameras)[++mCurrentCamera % mCameras->size()]);
}
Entity*    
Game::onSelfConnected(int pPlayerId, int pTeam) {
    DroneEntity* drone = new DroneEntity(pPlayerId);
    StartPoint* start = mWorld.getStartPoint(pTeam, pPlayerId);
    if ( start ) {
        drone->setPosition(start->getWorldPosition());
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
    DroneEntity* drone = new DroneEntity(pPlayerId);
    StartPoint* start = mWorld.getStartPoint(pTeam, pPlayerId);
    if ( start ) {
        drone->setPosition(start->getWorldPosition());
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