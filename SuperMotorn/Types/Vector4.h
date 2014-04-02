#pragma once
#include <DirectXMath.h>
namespace tinyxml2 {
    class XMLElement;
}
class Matrix;
class Vector4 {
    DirectX::XMFLOAT4 mVector;
public:
    Vector4();
    Vector4(float pX, float pY, float pZ, float pW);
    Vector4(const DirectX::XMVECTOR& pVector);
    Vector4(const DirectX::XMFLOAT4& pVector);
    Vector4(const tinyxml2::XMLElement* element);
    operator DirectX::XMVECTOR();
    operator DirectX::XMVECTOR() const;
    operator DirectX::XMFLOAT4();
    Vector4  operator+(const Vector4 & pOther);
    Vector4  operator-(const Vector4 & pOther);
    Vector4  operator*(const Matrix & pOther);
    Vector4  operator*(float pScale);
    float   getX();
    float   getY();
    float   getZ();
    float   getW();
    void   setX(float x);
    void   setY(float y);
    void   setZ(float z);
    void   setW(float w);
    ~Vector4();
};

