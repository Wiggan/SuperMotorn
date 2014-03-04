#include "Configuration.h"
#include "tinyxml2.h"
#include "Util.h"

Configuration::Configuration(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : 
AbstractResource(pFileName, pTimeStamp, pLoadingTools), mServerIp("0.0.0.0"), mServerPort("27015") {
}
void
Configuration::load() {
    using namespace std;
    using namespace tinyxml2;
    tinyxml2::XMLDocument configuration;
    configuration.LoadFile(Util::wstring2string(mFileName).c_str());
    if ( configuration.ErrorID() == 0 ) {
        XMLElement* config = configuration.FirstChildElement("config");
        XMLElement* server = config->FirstChildElement("server");
        mServerPort = server->Attribute("port");
        mServerIp = server->Attribute("ip");
    }
}
std::string 
Configuration::getServerIp() {
    return mServerIp;
}
std::string 
Configuration::getServerPort() {
    return mServerPort;
}
Configuration::~Configuration() {
}
