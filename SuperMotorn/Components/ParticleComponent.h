#pragma once
#include "Component.h"
#include "ParticleSystemResource.h"
class ParticleComponent {
    Renderer*                       mRenderer;
    Mesh*                           mMesh;
    Material                        mMaterial;
    Matrix                          mWorldTransform;
    Vector3                         mWorldPosition;
    Vector3                         mScale;
    Vector3                         mDirection;
    std::vector<ControlPoint>*      mControlPoints;
    float                           mLifeTime;
    bool                            mAlive;
public:
    ParticleComponent(Renderer* pRenderer, Mesh* pMesh, Material* pMaterial);
    void        update(float pDelta);
    void        draw();
    void        start(Vector3 pPosition, Vector3 pDirection, std::vector<ControlPoint>* pControlPoints);
    bool        isAlive();
    ~ParticleComponent();
};

