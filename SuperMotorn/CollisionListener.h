#pragma once
#include "ColliderComponent.h"
class CollisionListener {
public:
    virtual void onCollision(const ColliderComponent& pOther) = 0;
    virtual ~CollisionListener() {}
};

