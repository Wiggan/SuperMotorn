#pragma once
#include "AbstractResource.h"
#include "Vector3.h"
#include "Vector4.h"
#include <vector>
struct ControlPoint {
    float time;
    Vector3 scale;
    Vector4 color;
};
class ParticleSystemResource : public AbstractResource {
    std::vector<ControlPoint>   mControlPoints;
    const tinyxml2::XMLElement* getPreviousWithType(const tinyxml2::XMLElement* pStart, std::string pType);
    const tinyxml2::XMLElement* getNextWithType(const tinyxml2::XMLElement* pStart, std::string pType);
public:
    float                       mRate = 0.01;
    ParticleSystemResource(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools);
    virtual void                load();
    std::vector<ControlPoint>*  getControlPoints();
    ~ParticleSystemResource();
};

