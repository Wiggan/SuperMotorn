#pragma once
#include "MeshComponent.h"
class PropellerComponent : public MeshComponent {
    bool    mSpinning;
    float   mRotationSpeed;
    char    mInput;
public:
                    PropellerComponent(Renderer* pRenderer, Mesh* pMesh);
    void            keyDown(unsigned int key);
    void            keyUp(unsigned int key);
    virtual void    update(float pDelta);
    Vector3         getForce();
    void            setInput(char pInpunt);
                    ~PropellerComponent();
};

