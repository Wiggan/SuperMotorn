#pragma once
#include "AbstractResource.h"
#include "Vector4.h"
#include "Vector3.h"
class VertexShader;
class PixelShader;
class ComputeShader;
class Texture;
class Material :
    public AbstractResource {
    Vector4         mAmbient;
    Vector4         mDiffuse;
    Vector4         mSpecular;
    Vector4         mReflect;
    Vector3         mColor;
    VertexShader*   mVertexShader;
    PixelShader*    mPixelShader;
    ComputeShader*  mComputeShader;
    Texture*        mDiffuseMap;
    Texture*        mSpecularMap;
    Texture*        mGlossMap;
    Texture*        mGlowMap;
public:
                    Material(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void    load();
    Vector4&        getAmbient();
    Vector4&        getDiffuse();
    Vector4&        getSpecular();
    Vector4&        getReflect();
    Vector3&        getColor();
    VertexShader*   getVertexShader();
    PixelShader*    getPixelShader();
    ComputeShader*  getComputeShader();
    Texture*        getDiffuseMap();
    Texture*        getSpecularMap();
    Texture*        getGlossMap();
    Texture*        getGlowMap();
                    ~Material();
};

