#include "PropellerComponent.h"
#include <algorithm>

PropellerComponent::PropellerComponent(Renderer* pRenderer, Mesh* pMesh) : MeshComponent(pRenderer, pMesh), 
mSpinning(false), mRotationSpeed(0.0f) {

}
void
PropellerComponent::update(float pDelta) {
    if ( mSpinning ) {
        mRotationSpeed = min(mRotationSpeed + pDelta*15, 30.0f);
    } else {
        mRotationSpeed = max(mRotationSpeed - pDelta*15, 0.0f);
    }
    float y = pDelta*mRotationSpeed;
    if ( y > 0.00000001f ) {
        setRotation(getLocalRotation() + Vector3(0.0f, y, 0.0f));
    }
    MeshComponent::update(pDelta);
}
void
PropellerComponent::keyDown(unsigned int key) {
    if ( mInput == (char)key ) {
        mSpinning = true;
    }
}
void    
PropellerComponent::keyUp(unsigned int key) {
    if ( mInput == (char)key ) {
        mSpinning = false;
    }
}
void            
PropellerComponent::setInput(char pInpunt) {
    mInput = pInpunt;
}
float          
PropellerComponent::getForce() {
    //return (Vector3(0.0f, -1.0f, 0.0f) + mLocalPosition)*-mRotationSpeed;
    return mRotationSpeed * 0.05f;
}
PropellerComponent::~PropellerComponent() {
}
