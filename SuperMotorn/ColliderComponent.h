#pragma once
#include "Component.h"
#include <DirectXCollision.h>



class ColliderComponent : public Component {
    static bool                             mLayers[4][4];
    static std::vector<ColliderComponent*>  mColliders;
    DirectX::BoundingBox*                   mAABB = NULL;
    int                                     mLayer;
public:
                            ColliderComponent(Vector3 pExtents, int pLayer = 0);
    DirectX::BoundingBox*   getAABB();
    int                     getLayer() const;
    virtual void            update(float pDelta);
                            ~ColliderComponent();
};


