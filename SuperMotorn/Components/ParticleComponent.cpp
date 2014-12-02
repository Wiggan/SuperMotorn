#include "ParticleComponent.h"
#include "BaseCamera.h"
#include "DebugRenderer.h"
ParticleComponent::ParticleComponent(Renderer* pRenderer, Mesh* pMesh, Material* pMaterial) : mMaterial(*pMaterial), mScale(1.0f, 1.0f, 1.0f) {
    mRenderer = pRenderer;
    mMesh = pMesh;
}
void
ParticleComponent::update(float pDelta) {
    mLifeTime += pDelta;
    for ( auto it = mControlPoints->begin(); it != mControlPoints->end(); ++it ) {
        if ( mLifeTime < it->time ) {
            float progress = (mLifeTime - (it - 1)->time) / (it->time - (it - 1)->time);
            mScale = Vector3::lerp((it-1)->scale, it->scale, progress);
            mMaterial.setColor(Vector4::lerp((it-1)->color, it->color, progress));
            break;
        } else if( it == --mControlPoints->end() ) {
            mAlive = false;
        }
    }
    mWorldPosition = mWorldPosition + mDirection * pDelta;
    mWorldTransform = Matrix(mScale, (BaseCamera::gCurrentCamera->getPosition() - BaseCamera::gCurrentCamera->getLookAt()).dir2Rot(), mWorldPosition);
    //DebugRenderer::instance()->setStaticRay(mLocalPosition, mLocalPosition - (BaseCamera::gCurrentCamera->getLookAt() - BaseCamera::gCurrentCamera->getPosition()));
    //DebugRenderer::instance()->renderArrow(mLocalPosition, ( BaseCamera::gCurrentCamera->getPosition() - BaseCamera::gCurrentCamera->getLookAt()).dir2Rot());
}
void
ParticleComponent::draw() {
    mRenderer->drawTransparent(mMesh, mWorldTransform, &mMaterial);
    //DebugRenderer::instance()->renderArrow(getWorldPosition() + Vector3(0.5f, 0.0f, 0.0f), getWorldRotation());
}
void                                
ParticleComponent::start(Vector3 pPosition, Vector3 pDirection, std::vector<ControlPoint>* pControlPoints) {
    mDirection = pDirection;
    mWorldPosition = pPosition;
    mControlPoints = pControlPoints;
    mLifeTime = 0.0f;
    mAlive = true;
    update(0.0f);
}
bool               
ParticleComponent::isAlive() {
    return mAlive;
}
ParticleComponent::~ParticleComponent() {
}
