#include "Vector3.h"
#include "Matrix.h"
#include "tinyxml2.h"
#include <sstream>
#include <iostream>
Vector3::Vector3() : mVector(0.0f, 0.0f, 0.0f) {
}
Vector3::Vector3(float pX, float pY, float pZ) : mVector(pX, pY, pZ){
}
Vector3::Vector3(const DirectX::XMVECTOR& pVector) {
    DirectX::XMStoreFloat3(&mVector, pVector);
}
Vector3::Vector3(const tinyxml2::XMLElement* element) : mVector(0.0f, 0.0f, 0.0f)  {
    if ( element != NULL ) {
        element->QueryFloatAttribute("x", &mVector.x);
        element->QueryFloatAttribute("y", &mVector.y);
        element->QueryFloatAttribute("z", &mVector.z);
    }
}
Vector3::operator DirectX::XMVECTOR() {
    return DirectX::XMLoadFloat3(&mVector);
}
Vector3::operator DirectX::XMVECTOR() const {
    return DirectX::XMLoadFloat3(&mVector);
}
Vector3::operator DirectX::XMFLOAT3() {
    return mVector;
}
Vector3 
Vector3::operator+(const Vector3 & pOther) {
    return DirectX::XMVectorAdd(*this, pOther);
}
Vector3 
Vector3::operator-(const Vector3 & pOther) {
    return DirectX::XMVectorSubtract(*this, pOther);
}
Vector3 
Vector3::operator*(float pScale) {
    return DirectX::XMVectorScale(*this, pScale);
}
Vector3  
Vector3::operator*(const Matrix & pTransform) {
    return DirectX::XMVector3Transform(*this, pTransform);
}
float
Vector3::getLengthEst() {
    return Vector3(DirectX::XMVector3LengthEst(*this)).getX();
}
float   
Vector3::getX() {
    return mVector.x;
}
float   
Vector3::getY() {
    return mVector.y;
}
float   
Vector3::getZ() {
    return mVector.z;
}
void   
Vector3::setX(float x) {
    mVector.x = x;
}
void   
Vector3::setY(float y) {
    mVector.y = y;
}
void   
Vector3::setZ(float z) {
    mVector.z = z;
}
Vector3     
Vector3::cross(const Vector3 & pOther) {
    return Vector3(DirectX::XMVector3Cross(*this, pOther));
}
Vector3        
Vector3::normalized() const {
    return DirectX::XMVector3Normalize(*this);
}
void        
Vector3::rotate(const Vector3 & pAxis, float pAngle) {
    std::cout << pAxis.toString() << std::endl;
    DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationAxis(pAxis.normalized(), pAngle);
    *this = *this * DirectX::XMMatrixRotationQuaternion(quaternion);
}
Vector3 
Vector3::dir2Rot() {
    float yaw = -atan2(mVector.x, -mVector.z) + DirectX::XM_PI;
    float pitch = -atan2(mVector.y, sqrt(mVector.x*mVector.x + mVector.z*mVector.z));
    return Vector3(pitch, yaw, 0.0f);
}
std::string
Vector3::toString() const {
    std::ostringstream buf;
    buf << "x:" << mVector.x << "   y:" << mVector.y << "    z:" << mVector.z << std::endl;
    return buf.str();
}
Vector3::~Vector3() {
}
