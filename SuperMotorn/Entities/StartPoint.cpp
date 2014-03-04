#include "StartPoint.h"
#include "MeshComponent.h"
#include "ColliderComponent.h"
StartPoint::StartPoint(int pTeam) : mTeam(pTeam) {
    update(0.0f);
}
void
StartPoint::init(Renderer* pRenderer, ResourceLoader* pResourceLoader) {
    MeshComponent* platform = new MeshComponent(pRenderer, pResourceLoader->getResource<Mesh>(L"startpoint.dae"));
    platform->setPosition(Vector3(0.0f, -3.0f, 0.0f));
    platform->setMaterial(pResourceLoader->getResource<Material>(L"concrete.xml"));
    ColliderComponent* collider = new ColliderComponent(Vector3(4.0f, 1.0f, 4.0f));
    collider->setPosition(Vector3(0.f, -3.0f, 0.0f));
    add(collider);
    add(platform);
    Entity::init(pRenderer, pResourceLoader);
}
void 
StartPoint::setUsed() {
    mUsed = true;
}
bool 
StartPoint::isUsed() {
    return mUsed;
}
int 
StartPoint::getTeam() {
    return mTeam;
}

StartPoint::~StartPoint() {
}
