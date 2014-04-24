#include "Matrix.h"
#include "tinyxml2.h"
#include <sstream>
#include <iostream>
#include <vector>
Matrix::Matrix() {
    DirectX::XMStoreFloat4x4(&mMatrix, DirectX::XMMatrixIdentity());
}
Matrix::Matrix(const Vector3& pScale, const Vector3& pRotation, const Vector3& pPosition) {
    using namespace DirectX;
    XMStoreFloat4x4(&mMatrix,XMMatrixScalingFromVector(pScale)*
        XMMatrixRotationRollPitchYawFromVector(pRotation)*
        XMMatrixTranslationFromVector(pPosition));
}
Matrix::Matrix(const Vector3& pScale, const Matrix& pRotation, const Vector3& pPosition) {
    using namespace DirectX;
    XMStoreFloat4x4(&mMatrix,XMMatrixScalingFromVector(pScale)*
        pRotation*
        XMMatrixTranslationFromVector(pPosition));
}
Matrix::Matrix(const tinyxml2::XMLElement* element) {
    using namespace std;
    if ( element != NULL ) {
        istringstream iss(element->GetText());
        vector<string> tokens;
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));  
        for ( int i = 0; i < 4; i++ ) {
            for ( int j = 0; j < 4; j++ ) {
                mMatrix.m[j][i] = atof(tokens[i * 4 + j].c_str());
            }
        }
    }
}
Matrix::Matrix(const DirectX::XMMATRIX& pMatrix) {
    DirectX::XMStoreFloat4x4(&mMatrix, pMatrix);
}
Matrix::Matrix(const DirectX::XMFLOAT4X4& pMatrix) {
    mMatrix = pMatrix;
}
Matrix::Matrix(const float pMatrix[4][4]) {
    memcpy(&mMatrix.m, pMatrix, 64);
}
Matrix::operator DirectX::XMMATRIX() {
    return DirectX::XMLoadFloat4x4(&mMatrix);
}
Matrix::operator DirectX::XMMATRIX() const {
    return DirectX::XMLoadFloat4x4(&mMatrix);
}
Matrix::operator DirectX::XMFLOAT4X4() {
    return mMatrix;
}
Matrix::operator DirectX::XMFLOAT4X4() const {
    return mMatrix;
}
Matrix::operator char*() {
    char buf[64];
    memcpy(buf, &mMatrix, 64);
    return buf;
}
Matrix 
Matrix::operator*(const Matrix & pOther) {
    return DirectX::XMMatrixMultiply(*this, pOther);
}
Matrix 
Matrix::transposed() {
    using namespace DirectX;
    return XMMatrixTranspose(XMLoadFloat4x4(&mMatrix));
}
Matrix 
Matrix::transposed() const {
    using namespace DirectX;
    return XMMatrixTranspose(XMLoadFloat4x4(&mMatrix));
}
Matrix 
Matrix::inversed() {
    using namespace DirectX;
    DirectX::XMVECTOR det;
    return XMMatrixInverse(&det, *this);
}
Vector3      
Matrix::getDirection() const {
    return Vector3(mMatrix._31, mMatrix._32, mMatrix._33);
}
Vector3  
Matrix::getRotation() const {
    return getDirection().normalized().dir2Rot();
}
Vector3  
Matrix::getPosition() const {
    return Vector3(mMatrix._41, mMatrix._42, mMatrix._43);
}
Vector3  
Matrix::getScale() const {
    return Vector3(mMatrix._11, mMatrix._22, mMatrix._33); // FEL!!!
}     
Vector3
Matrix::getUp() const {
    return Vector3(mMatrix._21, mMatrix._22, mMatrix._23);
}
void
Matrix::reset() {
    *this = DirectX::XMMatrixIdentity();
}
void        
Matrix::rotate(const Vector3 & pAxis, float pAngle) {
    DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationAxis(pAxis.normalized(), pAngle);
    *this = *this * DirectX::XMMatrixRotationQuaternion( DirectX::XMQuaternionNormalize (quaternion));
}
std::string
Matrix::toString() const {
    std::ostringstream buf;
    buf << "[ " << mMatrix._11 << ", " << mMatrix._12 << ", " << mMatrix._13 << ", " << mMatrix._14 << " ]" << std::endl
        << "[ " << mMatrix._21 << ", " << mMatrix._22 << ", " << mMatrix._23 << ", " << mMatrix._24 << " ]" << std::endl
        << "[ " << mMatrix._31 << ", " << mMatrix._32 << ", " << mMatrix._33 << ", " << mMatrix._34 << " ]" << std::endl
        << "[ " << mMatrix._41 << ", " << mMatrix._42 << ", " << mMatrix._43 << ", " << mMatrix._44 << " ]";
    return buf.str();
}
Matrix::~Matrix() {
}
