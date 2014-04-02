#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
#include <string>
namespace tinyxml2 {
    class XMLElement;
}
class Matrix {
    DirectX::XMFLOAT4X4 mMatrix;
public:
                Matrix();
                Matrix(const Vector3& pScale, const Vector3& pRotation, const Vector3& pPosition);
                Matrix(const Vector3& pScale, const Matrix& pRotation, const Vector3& pPosition);
                Matrix(const DirectX::XMMATRIX& pMatrix);
                Matrix(const DirectX::XMFLOAT4X4& pMatrix);
                Matrix(const float pMatrix[4][4]);
                Matrix(const tinyxml2::XMLElement* element);
                operator DirectX::XMMATRIX();
                operator DirectX::XMMATRIX() const;
                operator DirectX::XMFLOAT4X4();
                operator DirectX::XMFLOAT4X4() const;
                operator char*();
    Matrix      operator*(const Matrix & pOther);
    Matrix      transposed();
    Matrix      transposed() const;
    Vector3     getDirection() const;
    Vector3     getRotation() const;
    Vector3     getPosition() const;
    Vector3     getScale() const;
    Vector3     getUp() const;
    void        rotate(const Vector3 & pAxis, float pAngle);
    void        reset();
    std::string toString() const;
                ~Matrix();
};

