#pragma once
#include "AbstractResource.h"
class Configuration : public AbstractResource {
    std::string mServerIp;
    std::string mServerPort;
public:
    Configuration(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void    load();
    std::string getServerIp();
    std::string getServerPort();
    ~Configuration();
};

