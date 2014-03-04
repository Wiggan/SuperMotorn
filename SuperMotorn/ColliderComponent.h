#pragma once
#include "Component.h"
#include <DirectXCollision.h>


class ColliderComponent : public Component {
    static std::vector<ColliderComponent*>  mColliders;
    DirectX::BoundingBox*                   mAABB = NULL;
public:
                            ColliderComponent(Vector3 pExtents);
    DirectX::BoundingBox*   getAABB();
    virtual void            update(float pDelta);
                            ~ColliderComponent();
};


