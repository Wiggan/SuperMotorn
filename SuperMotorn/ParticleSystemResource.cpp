#include "ParticleSystemResource.h"
#include "tinyxml2.h"
#include "Util.h"

ParticleSystemResource::ParticleSystemResource(const std::wstring& pFileName, TimeStamp pTimeStamp, LoadingToolsInterface* pLoadingTools) : 
AbstractResource(pFileName, pTimeStamp, pLoadingTools) {
}
const tinyxml2::XMLElement* 
ParticleSystemResource::getPreviousWithType(const tinyxml2::XMLElement* pStart, std::string pType) {
    for ( const tinyxml2::XMLElement* child = pStart; child != NULL; child = child->PreviousSiblingElement() ) {
        if ( child->FirstChildElement(pType.c_str()) ) {
            return child;
        }
    }
}
const tinyxml2::XMLElement* 
ParticleSystemResource::getNextWithType(const tinyxml2::XMLElement* pStart, std::string pType) {
    for ( const tinyxml2::XMLElement* child = pStart; child != NULL; child = child->NextSiblingElement() ) {
        if ( child->FirstChildElement(pType.c_str()) ) {
            return child;
        }
    }
}
void
ParticleSystemResource::load() {
    using namespace std;
    using namespace tinyxml2;
    mControlPoints.clear();
    tinyxml2::XMLDocument configuration;
    configuration.LoadFile(Util::wstring2string(mFileName).c_str());
    if ( configuration.ErrorID() == 0 ) {
        XMLElement* system = configuration.FirstChildElement("particleSystem");
        XMLElement* controls = system->FirstChildElement("controlPoints");
        mRate = system->FloatAttribute("rate");
        for ( const tinyxml2::XMLElement* child = controls->FirstChildElement(); child != NULL; child = child->NextSiblingElement() ) {
            ControlPoint p;
            p.time = child->FloatAttribute("t");
            const XMLElement* color = child->FirstChildElement("color");
            if ( color ) {
                p.color = Vector4(color);
            } else {
                const tinyxml2::XMLElement* prev = getPreviousWithType(child, "color");
                const tinyxml2::XMLElement* next = getNextWithType(child, "color");
                Vector4 c0 = prev->FirstChildElement("color");
                Vector4 c1 = next->FirstChildElement("color");
                float t0 = prev->FloatAttribute("t");
                float t1 = next->FloatAttribute("t");
                p.color = Vector4::lerp(c0, c1, (p.time - t0) / (t1 - t0));
            }
            const XMLElement* scale = child->FirstChildElement("scale");
            if ( scale ) {
                p.scale = Vector3(scale);
            } else {
                const tinyxml2::XMLElement* prev = getPreviousWithType(child, "scale");
                const tinyxml2::XMLElement* next = getNextWithType(child, "scale");
                Vector3 s0 = prev->FirstChildElement("scale");
                Vector3 s1 = next->FirstChildElement("scale");
                float t0 = prev->FloatAttribute("t");
                float t1 = next->FloatAttribute("t");
                p.scale = Vector3::lerp(s0, s1, (p.time - t0) / (t1 - t0));
            }
            mControlPoints.push_back(p);
        }
    }
}
std::vector<ControlPoint>*  
ParticleSystemResource::getControlPoints() {
    return &mControlPoints;
}
ParticleSystemResource::~ParticleSystemResource() {
}
