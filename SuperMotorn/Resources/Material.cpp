#include "Material.h"
#include "Texture.h"
#include "tinyxml2.h"
#include "Util.h"
Material::Material(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : 
AbstractResource(pFileName, pTimeStamp, pLoadingTools),
mAmbient(0.1f, 0.1f, 0.1f, 1.0f),
mDiffuse(0.1f, 0.1f, 0.1f, 1.0f),
mSpecular(0.1f, 0.1f, 0.1f, 1.0f),
mReflect(0.1f, 0.1f, 0.1f, 1.0f), 
mColor(1.0f, 0.0f, 0.0f), mVertexShader(NULL), mPixelShader(NULL), mComputeShader(NULL), mDiffuseMap(NULL), mSpecularMap(NULL) {
}
void
Material::load() {
    using namespace std;
    using namespace tinyxml2;
    tinyxml2::XMLDocument material;
    material.LoadFile(Util::wstring2string(mFileName).c_str());
    if ( material.ErrorID() == 0 ) {
        XMLElement* materialElement = material.FirstChildElement("material");
        XMLElement* verteShaderName = materialElement->FirstChildElement("vertexShader");
        XMLElement* pixelShaderName = materialElement->FirstChildElement("pixelShader");
        XMLElement* computeShaderName = materialElement->FirstChildElement("computeShader");
        if ( verteShaderName != NULL ) {
            mVertexShader = (VertexShader*)mLoadingTools->getVertexShader(Util::string2wstring(verteShaderName->GetText()));
        } else {
            mVertexShader = (VertexShader*)mLoadingTools->getVertexShader(L"defaultVertex.fx");
        }
        if ( mVertexShader == NULL ) {
            wcout << L"Error loading vertex shader in material : " << mFileName << endl;
        }
        if ( pixelShaderName != NULL ) {
            mPixelShader = (PixelShader*)mLoadingTools->getPixelShader(Util::string2wstring(pixelShaderName->GetText()));
        } else {
            mPixelShader = (PixelShader*)mLoadingTools->getPixelShader(L"defaultPixel.fx");
        }
        if ( mPixelShader == NULL ) {
            wcout << L"Error loading shader in material : " << mFileName << endl;
        }
        if ( computeShaderName != NULL ) {
            mComputeShader = (ComputeShader*)mLoadingTools->getComputeShader(Util::string2wstring(computeShaderName->GetText()));
        }
        XMLElement* diffuseMapName = materialElement->FirstChildElement("diffuseMap");
        if ( diffuseMapName != NULL ) {
            mDiffuseMap = (Texture*)mLoadingTools->getTexture(Util::string2wstring(diffuseMapName->GetText()));
        } else {
            mDiffuseMap = (Texture*)mLoadingTools->getTexture(L"default.png");
        }
        if ( mDiffuseMap == NULL ) {
            wcout << L"Error loading texture in material : " << mFileName << endl;
        }
        XMLElement* specularMapName = materialElement->FirstChildElement("specularMap");
        if ( specularMapName != NULL ) {
            mSpecularMap = (Texture*)mLoadingTools->getTexture(Util::string2wstring(specularMapName->GetText()));
        }
        XMLElement* glossMapName = materialElement->FirstChildElement("glossMap");
        if ( glossMapName != NULL ) {
            mGlossMap = (Texture*)mLoadingTools->getTexture(Util::string2wstring(glossMapName->GetText()));
        }
        mAmbient = Vector4(materialElement->FirstChildElement("ambient"));
        mDiffuse = Vector4(materialElement->FirstChildElement("diffuse"));
        mSpecular = Vector4(materialElement->FirstChildElement("specular"));
        mReflect = Vector4(materialElement->FirstChildElement("reflect"));
        mColor = Vector3(materialElement->FirstChildElement("color"));
    }
}
Vector4&        
Material::getAmbient() {
    return mAmbient;
}
Vector4&        
Material::getDiffuse() {
    return mDiffuse;
}
Vector4&        
Material::getSpecular() {
    return mSpecular;
}
Vector4&        
Material::getReflect() {
    return mReflect;
}
Vector3&        
Material::getColor() {
    return mColor;
}
VertexShader*
Material::getVertexShader() {
    return mVertexShader;
}
PixelShader*
Material::getPixelShader() {
    return mPixelShader;
}
Texture*
Material::getDiffuseMap() {
    return mDiffuseMap;
}
Texture*
Material::getSpecularMap() {
    return mSpecularMap;
}
Texture*
Material::getGlossMap() {
    return mGlossMap;
}
Material::~Material() {
}
