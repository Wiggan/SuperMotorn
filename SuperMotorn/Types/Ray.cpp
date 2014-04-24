#include "Ray.h"
#include "Matrix.h"
#include <DirectXCollision.h>
Ray::Ray(Vector3 pPosition, Vector3 pDirection) : mPosition(pPosition), mDirection(pDirection) {
}
Ray::Ray() {
}
Ray 
Ray::operator*(const Matrix& pMatrix) {
    Vector3 pos = mPosition*pMatrix;
    return Ray(pos, (mDirection*pMatrix - pos).normalized());
}
Vector3 
Ray::getPosition() {
    return mPosition;
}
Vector3 
Ray::getDirection() {
    return mDirection;
}
void 
Ray::setPosition(Vector3 pPosition) {
    mPosition = pPosition;
}
void 
Ray::setDirection(Vector3 pDirection) {
    mDirection = pDirection;
}
bool    
Ray::intersects(Vector3 pP1, Vector3 pP2, Vector3 pP3, float& pDistance) {
    return DirectX::TriangleTests::Intersects(mPosition, mDirection, pP1, pP2, pP3, pDistance);
}
bool    
Ray::intersects(DirectX::BoundingBox& pBox, float& pDistance) {
    return pBox.Intersects(mPosition, mDirection, pDistance);
}
Ray::~Ray() {
}
