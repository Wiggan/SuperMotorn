#include "PointLightComponent.h"
#include "DebugRenderer.h"
PointLightComponent::PointLightComponent(Renderer* pRenderer, PointLight pPointLight) : mRenderer(pRenderer), mPointLight(pPointLight) {
}
void
PointLightComponent::update(float pDelta) {
    Component::update(pDelta);
    mPointLight.position = getWorldPosition();
}
void
PointLightComponent::draw() {
    mRenderer->drawPointLight(&mPointLight);
    DebugRenderer::instance()->renderSphere(getWorldPosition());
}
void            
PointLightComponent::setLight(PointLight pPointLight) {
    mPointLight = pPointLight;
}
PointLightComponent::~PointLightComponent() {
}
