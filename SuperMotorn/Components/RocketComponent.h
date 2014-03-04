#pragma once
#include "MeshComponent.h"
class RocketComponent : public MeshComponent {
    bool        mFree;
    Vector3     mVelocity;
public:
                    RocketComponent(Renderer* pRenderer, Mesh* pMesh);
    virtual void    update(float pDelta);
    virtual void    draw();
    bool            isFree();
    void            fire(Vector3 pPosition, Vector3 pDirection);
                    ~RocketComponent();
};

