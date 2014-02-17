#include "Vector4.h"
#include "Matrix.h"
#include "tinyxml2.h"
Vector4::Vector4() : mVector(0.0f, 0.0f, 0.0f, 0.0f){
}
Vector4::Vector4(float pX, float pY, float pZ, float pW) : mVector(pX, pY, pZ, pW){
}
Vector4::Vector4(const DirectX::XMVECTOR& pVector) {
    DirectX::XMStoreFloat4(&mVector, pVector);
}
Vector4::Vector4(const tinyxml2::XMLElement* element) {
    if ( element != NULL ) {
        element->QueryFloatAttribute("x", &mVector.x);
        element->QueryFloatAttribute("y", &mVector.y);
        element->QueryFloatAttribute("z", &mVector.z);
        element->QueryFloatAttribute("w", &mVector.w);
    }
}
Vector4::operator DirectX::XMVECTOR() {
    return DirectX::XMLoadFloat4(&mVector);
}
Vector4::operator DirectX::XMVECTOR() const {
    return DirectX::XMLoadFloat4(&mVector);
}
Vector4::operator DirectX::XMFLOAT4() {
    return mVector;
}
Vector4 
Vector4::operator+(const Vector4 & pOther) {
    return DirectX::XMVectorAdd(*this, pOther);
}
Vector4 
Vector4::operator-(const Vector4 & pOther) {
    return DirectX::XMVectorSubtract(*this, pOther);
}
Vector4 
Vector4::operator*(float pScale) {
    return DirectX::XMVectorScale(*this, pScale);
}
Vector4  
Vector4::operator*(const Matrix & pTransform) {
    return DirectX::XMVector4Transform(*this, pTransform);
}
float   
Vector4::getX() {
    return mVector.x;
}
float   
Vector4::getY() {
    return mVector.y;
}
float   
Vector4::getZ() {
    return mVector.z;
}
void   
Vector4::setX(float x) {
    mVector.x = x;
}
void   
Vector4::setY(float y) {
    mVector.y = y;
}
void   
Vector4::setZ(float z) {
    mVector.z = z;
}
Vector4::~Vector4() {
}
