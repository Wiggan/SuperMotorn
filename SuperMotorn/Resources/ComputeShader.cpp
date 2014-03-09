#include "ComputeShader.h"
#include <fstream>
#include <iostream>
#include <d3dcompiler.h>
#include "Util.h"

ComputeShader::ComputeShader(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : 
AbstractResource(pFileName, pTimeStamp, pLoadingTools), mComputeShader(NULL) {
    mDevice = pLoadingTools->getDevice();
    mContext = pLoadingTools->getContext();
}
void
ComputeShader::load() {
    
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
        MessageBox(0, L"ComputeShader is empty!", 0, 0);
        if ( mComputeShader != NULL ) {
            return;
        }
        exit(-1);
    }
    ID3DBlob *cs, *error = NULL;
    HRESULT hr = D3DCompile(shader, size, Util::wstring2string( mFileName ).c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CSMain", "cs_5_0", 0, 0, &cs, &error);
    if ( FAILED(hr) ) {
        char* err = (char*)malloc(error->GetBufferSize());
        memcpy(err, error->GetBufferPointer(), error->GetBufferSize());
        cout << err << endl;
        free(err);
        MessageBox(0, L"D3DCompile failed", 0, 0);
        error->Release();
        if ( mComputeShader != NULL ) {
            return;
        }
        exit(-1);
    }
    if ( mComputeShader != NULL ) {
        mComputeShader->Release();
    }
    hr = mDevice->CreateComputeShader(cs->GetBufferPointer(), cs->GetBufferSize(), NULL, &mComputeShader);
    if ( FAILED(hr) ) {
        MessageBox(0, L"CreateComputeShader failed", 0, 0);
        exit(-1);
    }
    cs->Release();
}
ID3D11ComputeShader*    
ComputeShader::getComputeShader() {
    return mComputeShader;
}
ComputeShader::~ComputeShader() {
    mComputeShader->Release();
}
