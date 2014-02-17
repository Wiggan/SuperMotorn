#include "DebugRenderer.h"
#include "Matrix.h"
DebugRenderer* DebugRenderer::mInstance = NULL;

DebugRenderer::DebugRenderer() : mActive(true) {
}
void                    
DebugRenderer::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    mInstance = new DebugRenderer;
    mInstance->mRenderer = pRenderer;
    mInstance->mSphere = pResourceLoader->getResource<Mesh>(L"debugsphere.dae");
    mInstance->mCube = pResourceLoader->getResource<Mesh>(L"debugcube.dae");
    mInstance->mArrow = pResourceLoader->getResource<Mesh>(L"debugarrow.dae");
    mInstance->mMaterial = pResourceLoader->getResource<Material>(L"default.xml");
}
void                    
DebugRenderer::renderSphere(Vector3 pPosition, Vector3 pScale) {
#ifdef _DEBUG
    if ( mActive ) {
        Matrix transform(pScale, Vector3(), pPosition);
        mRenderer->drawSolid(mSphere, transform, mMaterial);
    }
#endif
}
void                    
DebugRenderer::renderCube(Vector3 pPosition, Vector3 pRotation, Vector3 pScale) {
#ifdef _DEBUG
    if ( mActive ) {
        Matrix transform(pScale, pRotation, pPosition);
        mRenderer->drawSolid(mCube, transform, mMaterial);
    }
#endif
}
void                    
DebugRenderer::renderArrow(Vector3 pPosition, Vector3 pRotation, Vector3 pScale) {
#ifdef _DEBUG
    if ( mActive ) {
        Matrix transform(pScale, pRotation, pPosition);
        mRenderer->drawSolid(mArrow, transform, mMaterial);
    }
#endif
}
DebugRenderer*   
DebugRenderer::instance() {
    return mInstance;
}
void
DebugRenderer::toggleActive() {
    mActive = !mActive;
}
DebugRenderer::~DebugRenderer() {
}
