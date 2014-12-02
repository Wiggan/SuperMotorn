#include "EmitterComponent.h"
#include <stdlib.h>
#include <time.h>  

EmitterComponent::EmitterComponent() : mParticlePool(100) {
    srand (time(NULL));
}
void    
EmitterComponent::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    mRenderer = pRenderer;
    Mesh* mesh = (Mesh*)pResourceLoader->getMesh(L"particle.dae");
    Material* material = (Material*)pResourceLoader->getMaterial(L"rocketsmoke.xml");
    mConfig = pResourceLoader->getResource<ParticleSystemResource>(L"rocketParticles.xml");
    for ( auto it = mParticlePool.begin(); it != mParticlePool.end(); ++it ) {
        *it = new ParticleComponent(pRenderer, mesh, material);
    }
}
void
EmitterComponent::update(float pDelta) {
    if ( mEnabled ) {
        Component::update(pDelta);
        mTimeSinceLastEmit += pDelta;
        if ( mTimeSinceLastEmit > mConfig->mRate ) {
            if ( mParticlePool.size() > 0 ) {
                ParticleComponent* particle = mParticlePool.back();
                mParticlePool.pop_back();
                mLivingParticles.push_back(particle);
                particle->start(getWorldPosition(), Vector3(0.00008f*rand(), 0.00008f*rand(), 0.00008f*rand()), mConfig->getControlPoints());
            }
            mTimeSinceLastEmit = 0.0f;
        }
    }
    for ( auto it = mLivingParticles.begin(); it != mLivingParticles.end(); ++it ) {
        (*it)->update(pDelta);
        if ( !(*it)->isAlive() ) {
            mParticlePool.push_back(*it);
            it = mLivingParticles.erase(it);
        }
    }
}
void
EmitterComponent::draw() {
    for ( auto it = mLivingParticles.begin(); it != mLivingParticles.end(); ++it ) {
        (*it)->draw();
    }
}
void
EmitterComponent::reset() {
    mParticlePool.splice(mParticlePool.end(), mLivingParticles);
    mTimeSinceLastEmit = 0.0f;
}
void
EmitterComponent::start() {
    mEnabled = true;
}
void
EmitterComponent::pause() {
    mEnabled = false;
}
EmitterComponent::~EmitterComponent() {
}
