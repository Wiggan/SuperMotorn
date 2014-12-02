#pragma once
#include "Component.h"
#include "ParticleSystemResource.h"
#include "ParticleComponent.h"
class EmitterComponent : public Component {
    Renderer*                       mRenderer;
    bool                            mEnabled;
    ParticleSystemResource*         mConfig;
    std::list<ParticleComponent*>   mParticlePool;
    std::list<ParticleComponent*>   mLivingParticles;
    float                           mTimeSinceLastEmit = 0.0f;
public:
                        EmitterComponent();
    virtual void        init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void        update(float pDelta);
    virtual void        draw();
    void                reset();
    void                start();
    void                pause();
                        ~EmitterComponent();
};

