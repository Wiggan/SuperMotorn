#pragma once
#include <DirectXMath.h>
#include <Windows.h>
struct PointLight {
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;
    DirectX::XMFLOAT3 position;
    float pad;
    DirectX::XMFLOAT3 attenuation;
	float range;
    PointLight() {
        ZeroMemory(this, sizeof(*this));
        range = 500.0f;
        attenuation.z = 1.0f;
    }

    ~PointLight() {
    }
};

