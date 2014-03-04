#pragma once
#include "Entity.h"
class Rocket : public Entity {
    bool        mFree;
    int         mTeam;
    float       mFlyTime;
    Vector3     mVelocity;
    Matrix      mRotationMatrix;
public:
                    Rocket(int pTeam);
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void    update(float pDelta);
    virtual void    draw();
    bool            isFree();
    void            fire(Vector3 pPosition, Vector3 pDirection);
    virtual void    onCollision(const ColliderComponent& pOther);
    void            reset();
    virtual Matrix  calculateLocalTransform();
                    ~Rocket();
};

