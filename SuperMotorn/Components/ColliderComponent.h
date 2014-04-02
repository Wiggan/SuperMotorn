#pragma once
#include "Component.h"
#include <DirectXCollision.h>



class ColliderComponent : public Component {
    static bool                             mLayers[4][4];
    static std::vector<ColliderComponent*>  mColliders;
    DirectX::BoundingBox*                   mAABB = NULL;
    DirectX::BoundingOrientedBox*           mBOB = NULL;
    int                                     mLayer;
public:
                                    ColliderComponent(Vector3 pExtents, int pLayer = 0);
                                    ColliderComponent(Vector3 pExtents, Vector3 pOrientation, int pLayer = 0);
    DirectX::BoundingBox*           getAABB() const;
    DirectX::BoundingOrientedBox*   getBOB() const;
    int                             getLayer() const;
    virtual void                    update(float pDelta);
                                    ~ColliderComponent();
};


