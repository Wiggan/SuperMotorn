#include "AbstractResource.h"


AbstractResource::AbstractResource(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : 
mFileName(pFileName), mTimeStamp(pTimeStamp), mLoadingTools(pLoadingTools) {
}
TimeStamp&      
AbstractResource::getTimeStamp() {
    return mTimeStamp;
}
void                
AbstractResource::setTimeStamp(TimeStamp& pTimeStamp) {
    mTimeStamp = pTimeStamp;
}
const std::wstring&  
AbstractResource::getFileName() {
    return mFileName;
}
AbstractResource::~AbstractResource() {
}
