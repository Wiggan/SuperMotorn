#pragma once
#include "AbstractResource.h"
#include "Vector4.h"
#include "Vector3.h"
class Shader;
class Texture;
class Material :
    public AbstractResource {
    Vector4     mAmbient;
    Vector4     mDiffuse;
    Vector4     mSpecular;
    Vector4     mReflect;
    Vector3     mColor;
    Shader*     mShader;
    Texture*    mDiffuseMap;
    Texture*    mSpecularMap;
    Texture*    mGlossMap;
public:
                    Material(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void    load();
    Vector4&        getAmbient();
    Vector4&        getDiffuse();
    Vector4&        getSpecular();
    Vector4&        getReflect();
    Vector3&        getColor();
    Shader*         getShader();
    Texture*        getDiffuseMap();
    Texture*        getSpecularMap();
    Texture*        getGlossMap();
                    ~Material();
};

