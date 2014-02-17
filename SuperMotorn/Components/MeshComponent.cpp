#include "MeshComponent.h"


MeshComponent::MeshComponent(Renderer* pRenderer, Mesh* pMesh) : mRenderer(pRenderer), mMesh(pMesh) {
}
void
MeshComponent::setMaterial(Material* pMaterial) {
    mMaterial = pMaterial;
}
Material*                    
MeshComponent::getMaterial() {
    return mMaterial;
}
Mesh*                    
MeshComponent::getMesh() {
    return mMesh;
}
void
MeshComponent::draw() {
    mRenderer->drawSolid(mMesh, mWorldTransform, mMaterial);
}
MeshComponent::~MeshComponent() {
}
