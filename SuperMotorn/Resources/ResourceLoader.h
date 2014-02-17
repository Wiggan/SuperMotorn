#pragma once
#include <unordered_map>
#include "AbstractResource.h"
#include <d3d11.h>
#include "Util.h"
class Texture;
class ResourceLoader : public LoadingToolsInterface {
private:
    ID3D11Device*                                       mDevice;
    ID3D11DeviceContext*                                mContext;
    std::unordered_map<std::wstring, AbstractResource*> mResources;
    TimeStamp                                           getFileTimeStamp(const std::wstring &pPath);
public:
                        ResourceLoader();
    void                init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    
    template<typename T>
    T* getResource(const std::wstring &pFileName) {
        using namespace std;
        auto it = mResources.find(pFileName);
        if ( it == mResources.end() ) {
            wstring completePath = Util::searchDirectory(L"..", pFileName);
            if ( !completePath.empty() ) {
                T* resource = new T(completePath, getFileTimeStamp(completePath), this);
                resource->load();
                mResources.insert(pair<std::wstring, AbstractResource*>(pFileName, resource));
                return resource;
            } else {
                std::wcout << "Error loading resource: " << pFileName << std::endl;
                return NULL;
            }
        }
        return (T*)it->second;
    }

    virtual ID3D11Device*           getDevice();
    virtual ID3D11DeviceContext*    getContext();
    virtual AbstractResource*       getTexture(const std::wstring &pFileName);
    virtual AbstractResource*       getShader(const std::wstring &pFileName);
    void                            lookForChanges();
                                    ~ResourceLoader();
};
