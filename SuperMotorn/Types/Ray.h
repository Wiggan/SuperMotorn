#pragma once
#include "Vector3.h"
#include <DirectXCollision.h>
class Ray {
private:
    Vector3 mPosition;
    Vector3 mDirection;
public:
    Ray(Vector3 pPosition, Vector3 pDirection);
    Ray();
    Ray operator*(const Matrix& pMatrix);
    Vector3 getPosition();
    Vector3 getDirection();
    void    setPosition(Vector3 pPosition);
    void    setDirection(Vector3 pDirection);
    bool    intersects(Vector3 pP1, Vector3 pP2, Vector3 pP3, float& pDistance);
    bool    intersects(DirectX::BoundingBox&, float& pDistance);
    ~Ray();
};

