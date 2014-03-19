#pragma once
#include "Vector3.h"
#include "Matrix.h"
#include <string>
class GameObject {
protected:
    std::string mName;
    bool        mDirty;
    bool        mUpdatedThisFrame;
    Matrix      mWorldTransform;
    Matrix      mLocalTransform;
    Vector3      mLocalPosition;
    Vector3      mLocalRotation;
    Vector3      mLocalScale;
public:
    virtual void    update(float pDelta) = 0;
    virtual void    draw() = 0;
    Matrix          getWorldTransform();
    Vector3          getLocalPosition();
    Vector3          getLocalRotation();
    Vector3          getLocalScale();
    Vector3          getLocalDirection();
    Vector3          getLocalUp();
    Vector3          getWorldPosition();
    Vector3          getWorldRotation();
    Vector3          getWorldScale();
    Vector3          getWorldDirection();
    Vector3          getWorldUp();
    void            setScale(const Vector3& pScale);
    void            setPosition(const Vector3& pPosition);
    void            setRotation(const Vector3& pRotation);
    void            setRotation(const Matrix& pRotation);
    bool            isUpdatedThisFrame();
                    GameObject();
    virtual         ~GameObject();
};

