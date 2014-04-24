#ifndef VERTEXXXX_H
#define VERTEXXXX_H
#include <DirectXMath.h>
#include <d3d11.h>
struct Vertex {
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT3 Color;
    DirectX::XMFLOAT2 Tex0;
    DirectX::XMFLOAT2 Tex1;
};



#endif