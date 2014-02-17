#pragma once
#include "GameObject.h"
class Entity;
class Component : public GameObject {
protected:
    Entity* mOwner;
public:
                    Component();
    void            setOwner(Entity* pOwner);
    virtual void    draw();
    virtual void    update(float pDelta);
    virtual         ~Component();
};

