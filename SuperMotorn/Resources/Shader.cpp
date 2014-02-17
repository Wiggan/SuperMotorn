#include "Shader.h"
#include <fstream>
#include <iostream>
#include <d3dcompiler.h>
D3D11_INPUT_ELEMENT_DESC inputElementDescription[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEX", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
Shader::Shader(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : 
AbstractResource(pFileName, pTimeStamp, pLoadingTools), mPixelShader(NULL), mVertexShader(NULL) {
    mDevice = pLoadingTools->getDevice();
    mContext = pLoadingTools->getContext();
}
void
Shader::load() {
    
    using namespace std;
    ifstream ifs(mFileName, std::ios::binary);
    ifs.seekg(0, std::ios_base::end);
    int size = (unsigned int)(1 + ifs.tellg());
    ifs.seekg(0, std::ios_base::beg);
    char* shader = new char[size];
    for ( int i = 0; i < size; i++ ) {
        shader[i] = 0;
    }
    shader[size - 1] = '\0';
    ifs.read(shader, size);
    if ( size == 0 ) {
        cout << "File is empty!" << endl;
        MessageBox(0, L"Shader is empty!", 0, 0);
        if ( mVertexShader != NULL && mPixelShader != NULL ) {
            return;
        }
        exit(-1);
    }
    ID3DBlob *vs, *ps, *error = NULL;
    HRESULT hr = D3DCompile(shader, size, NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &vs, &error);
    if ( FAILED(hr) ) {
        char* err = (char*)malloc(error->GetBufferSize());
        memcpy(err, error->GetBufferPointer(), error->GetBufferSize());
        cout << err << endl;
        free(err);
        MessageBox(0, L"D3DCompile failed", 0, 0);
        error->Release();
        if ( mVertexShader != NULL && mPixelShader != NULL ) {
            return;
        }
        exit(-1);
    }
    hr = D3DCompile(shader, size, NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &ps, &error);
    if ( FAILED(hr) ) {
        char* err = (char*)malloc(error->GetBufferSize());
        memcpy(err, error->GetBufferPointer(), error->GetBufferSize());
        cout << err << endl;
        free(err);
        error->Release();
        MessageBox(0, L"D3DCompile failed", 0, 0);
        if ( mVertexShader != NULL && mPixelShader != NULL ) {
            return;
        }
        exit(-1);
    }
    if ( mVertexShader != NULL ) {
        mVertexShader->Release();
    }
    if ( mPixelShader != NULL ) {
        mPixelShader->Release();
    }
    hr = mDevice->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &mVertexShader);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateVertexShader failed", 0, 0);
        exit(-1);
    }
    hr = mDevice->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &mPixelShader);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreatePixelShader failed", 0, 0);
        exit(-1);
    }
    UINT bufferSize = vs->GetBufferSize();
    hr = mDevice->CreateInputLayout(inputElementDescription, 
        sizeof(inputElementDescription) / sizeof(*inputElementDescription), vs->GetBufferPointer(), bufferSize, &mInputLayout);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateInputLayout failed", 0, 0);
        exit(-1);
    }
    ps->Release();
    vs->Release();
    
}
ID3D11VertexShader*     
Shader::getVertexShader() {
    return mVertexShader;
}
ID3D11PixelShader*      
Shader::getPixelShader() {
    return mPixelShader;
}                       
ID3D11InputLayout*      
Shader::getInputLayout() {
    return mInputLayout;
}
Shader::~Shader() {
    mVertexShader->Release();
    mPixelShader->Release();
    mInputLayout->Release();
}
