#include "Game.h"
#include <iostream>
#include "MeshComponent.h"
#include "DroneEntity.h"
#include "CameraComponent.h"
#include "Shader.h"
#include "LevelLoader.h"
#include "Texture.h"
#include "DebugRenderer.h"
Game::Game(HWND pWindow, int width, int height, Timer* pTimer) : mWindow(pWindow),
mTimer(pTimer), mShowFps(false), mRenderer(pWindow, width, height), mWorld(&mRenderer) {
    mRenderer.init();
    mResourceLoader.init(mRenderer.getDevice(), mRenderer.getContext());
#ifdef _DEBUG
    /*Entity* entity = new Entity();
    MeshComponent* componentX = new MeshComponent(&mRenderer, mResourceLoader.getResource<Mesh>(L"box.dae"));
    componentX->setMaterial(mResourceLoader.getResource<Material>(L"default.xml"));
    componentX->setScale(Vector3(5.0f, 0.1f, 0.1f));
    componentX->setPosition(Vector3(5.0f, 0.0f, 0.0f));
    entity->add(componentX);
    MeshComponent* componentY = new MeshComponent(&mRenderer, mResourceLoader.getResource<Mesh>(L"box.dae"));
    componentY->setMaterial(mResourceLoader.getResource<Material>(L"default.xml"));
    componentY->setScale(Vector3(0.1f, 5.0f, 0.1f));
    componentY->setPosition(Vector3(0.0f, 5.0f, 0.0f));
    entity->add(componentY);
    MeshComponent* componentZ = new MeshComponent(&mRenderer, mResourceLoader.getResource<Mesh>(L"box.dae"));
    componentZ->setMaterial(mResourceLoader.getResource<Material>(L"default.xml"));
    componentZ->setScale(Vector3(0.1f, 0.1f, 5.0f));
    componentZ->setPosition(Vector3(0.0f, 0.0f, 5.0f));
    entity->add(componentZ);
    mWorld.add(entity);*/
    DebugRenderer::init(&mRenderer, &mResourceLoader);
#endif
    DroneEntity*        drone = new DroneEntity();
    drone->setPosition(Vector3(2.0f, 2.0f, -10.0f));
    InputComponent*     input = new InputComponent();
    drone->add(input);
    mInputComponents.push_back(input);
    mWorld.add(drone);
    LevelLoader ll(mResourceLoader, mRenderer);
    ll.loadLevel(L"level01.xml", &mWorld);
    mWorld.init(&mResourceLoader, &mCameras);
    mRenderer.setActiveCamera(mCameras[0]);
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
    mRenderer.setActiveCamera(mCameras[++mCurrentCamera % mCameras.size()]);
}
Game::~Game() {
    for ( auto it = mCameras.begin(); it != mCameras.end(); it++ ) {
        delete (*it);
    }
}