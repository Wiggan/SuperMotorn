#include "ResourceLoader.h"
#include <iostream>
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
ResourceLoader::ResourceLoader() {
}
ResourceLoader::~ResourceLoader() {
    for ( auto it = mResources.begin(); it != mResources.end(); ++it ) {
        delete it->second;
    }
}
void
ResourceLoader::lookForChanges() {
    for ( auto it = mResources.begin(); it != mResources.end(); ++it ) {
        AbstractResource* res = it->second;
        TimeStamp timestamp = getFileTimeStamp(res->getFileName());
        if ( timestamp != res->getTimeStamp() ) {
            std::wcout << L"Updating " << res->getFileName() << std::endl;
            res->load();
            res->setTimeStamp(timestamp);
        }
    }
}
void
ResourceLoader::init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {
    mDevice = pDevice;
    mContext = pContext;
}
ID3D11Device*           
ResourceLoader::getDevice() {
    return mDevice;
}
ID3D11DeviceContext*    
ResourceLoader::getContext() {
    return mContext;
}
AbstractResource*
ResourceLoader::getTexture(const std::wstring &pFileName) {
    return getResource<Texture>(pFileName);
}
AbstractResource*
ResourceLoader::getVertexShader(const std::wstring &pFileName) {
    return getResource<VertexShader>(pFileName);
}
AbstractResource*
ResourceLoader::getPixelShader(const std::wstring &pFileName) {
    return getResource<PixelShader>(pFileName);
}
AbstractResource*
ResourceLoader::getComputeShader(const std::wstring &pFileName) {
    return getResource<ComputeShader>(pFileName);
}
TimeStamp 
ResourceLoader::getFileTimeStamp(const std::wstring &pPath) {
    TimeStamp timestamp;
    timestamp.year = -1;
    timestamp.month = -1;
    timestamp.day = -1;
    timestamp.hour = -1;
    timestamp.minute = -1;
    timestamp.second = -1;
    HANDLE hFile = 0;
    FILETIME create, access, time;
    hFile = CreateFile(pPath.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
    if ( hFile != INVALID_HANDLE_VALUE ) {
        GetFileTime(hFile, &create, &access, &time);
        CloseHandle(hFile);
        SYSTEMTIME systemTime;
        FileTimeToSystemTime(&time, &systemTime);
        timestamp.year = (short)systemTime.wYear;
        timestamp.month = (short)systemTime.wMonth;
        timestamp.day = (short)systemTime.wDay;
        timestamp.hour = (short)systemTime.wHour;
        timestamp.minute = (short)systemTime.wMinute;
        timestamp.second = (short)systemTime.wSecond;
    }
    return timestamp;
}

