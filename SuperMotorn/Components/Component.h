#pragma once
#include "GameObject.h"
#include <DirectXCollision.h>
#include <vector>
#include "Renderer.h"
#include "ResourceLoader.h"
class Entity;
class Component : public GameObject {
protected:
    Entity*                             mOwner;
    Vector3                             mPreviousPosition;
public:
                            Component();
    virtual void            init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    void                    setOwner(Entity* pOwner);
    Entity*                 getOwner() const;
    virtual void            draw();
    virtual void            update(float pDelta);
    virtual void            setPosition(const Vector3& pPosition);
    virtual                 ~Component();
};

