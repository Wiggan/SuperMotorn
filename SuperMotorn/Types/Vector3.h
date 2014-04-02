#pragma once
#include <DirectXMath.h>
#include <string>
namespace tinyxml2 {
    class XMLElement;
}
class Matrix;
class Vector3 {
    DirectX::XMFLOAT3 mVector;
public:
    Vector3();
    Vector3(float pX, float pY, float pZ);
    Vector3(const DirectX::XMVECTOR& pVector);
    Vector3(const DirectX::XMFLOAT3& pVector);
    Vector3(float pVector[3]);
    Vector3(const tinyxml2::XMLElement* element);
    operator                DirectX::XMVECTOR();
    operator                DirectX::XMVECTOR() const;
    operator                DirectX::XMFLOAT3();
    Vector3                 operator+(const Vector3 & pOther);
    Vector3                 operator-(const Vector3 & pOther);
    Vector3                 operator*(const Matrix & pOther);
    Vector3                 operator*(float pScale);
    Vector3                 cross(const Vector3 & pOther);
    void                    rotate(const Vector3 & pAxis, float pAngle);
    Vector3                 dir2Rot() const;
    Vector3                 normalized() const;
    float                   getLengthEst();
    float                   getX();
    float                   getY();
    float                   getZ();
    void                    setX(float x);
    void                    setY(float y);
    void                    setZ(float z);
    std::string             toString() const;
    ~Vector3();
};

