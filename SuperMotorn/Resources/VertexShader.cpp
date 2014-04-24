#include "VertexShader.h"
#include <fstream>
#include <iostream>
#include <d3dcompiler.h>
#include "Util.h"
D3D11_INPUT_ELEMENT_DESC inputElementDescription[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEX", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
VertexShader::VertexShader(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : 
AbstractResource(pFileName, pTimeStamp, pLoadingTools), mVertexShader(NULL) {
    mDevice = pLoadingTools->getDevice();
    mContext = pLoadingTools->getContext();
}
void
VertexShader::load() {
    
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
        MessageBox(0, L"VertexShader is empty!", 0, 0);
        if ( mVertexShader != NULL ) {
            return;
        }
        exit(-1);
    }
    ID3DBlob *vs, *error = NULL;
    HRESULT hr = D3DCompile(shader, size, Util::wstring2string( mFileName ).c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", 0, 0, &vs, &error);
    if ( FAILED(hr) ) {
        char* err = (char*)malloc(error->GetBufferSize());
        memcpy(err, error->GetBufferPointer(), error->GetBufferSize());
        cout << err << endl;
        free(err);
        MessageBox(0, L"D3DCompile failed", 0, 0);
        error->Release();
        if ( mVertexShader != NULL ) {
            return;
        }
        exit(-1);
    }
    if ( mVertexShader != NULL ) {
        mVertexShader->Release();
    }
    hr = mDevice->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &mVertexShader);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateVertexShader failed", 0, 0);
        exit(-1);
    }
    UINT bufferSize = vs->GetBufferSize();
    hr = mDevice->CreateInputLayout(inputElementDescription, 
        sizeof(inputElementDescription) / sizeof(*inputElementDescription), vs->GetBufferPointer(), bufferSize, &mInputLayout);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateInputLayout failed", 0, 0);
        exit(-1);
    }
    vs->Release();
}
ID3D11VertexShader*     
VertexShader::getVertexShader() {
    return mVertexShader;
}   
ID3D11InputLayout*      
VertexShader::getInputLayout() {
    return mInputLayout;
}
VertexShader::~VertexShader() {
    mVertexShader->Release();
    mInputLayout->Release();
}
