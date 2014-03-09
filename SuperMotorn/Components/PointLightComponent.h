#pragma once
#include "Component.h"
#include "PointLight.h"
#include "Renderer.h"
class PointLightComponent : public Component {
    PointLight      mPointLight;
    Renderer*       mRenderer;
public:
                    PointLightComponent(Renderer* pRenderer, PointLight pPointLight);
    virtual void    draw();
    virtual void    update(float pDelta);
    void            setLight(PointLight pPointLight);
                    ~PointLightComponent();
};

