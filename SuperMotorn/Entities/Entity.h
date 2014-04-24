#pragma once
#include "GameObject.h"
#include <vector>
#include <Component.h>
#include "ResourceLoader.h"
#include "Renderer.h"
#include <DirectXCollision.h>
#include "CollisionListener.h"
class BaseCamera;
class ColliderComponent;
class Entity : public GameObject, public CollisionListener {
protected:
    std::vector<CollisionListener*> mCollisionListeners;
    std::vector<Component*>         mComponents;
    Entity*                         mParent;
    std::vector<Entity*>            mChildren;
    //Matrix                          mDesiredRotation;
    //Vector3                         mDesiredPosition;
    //float                           mTimeRotated;
    //float                           mTimeMoved;
    //bool                            mMoveToDesired = false;
    //bool                            mRotateToDesired = false;
public:
    enum Events {
        ROCKET_HIT,
        DRONE_LANDED,
    };
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void    update(float pDelta);
    virtual Matrix  calculateLocalTransform();
    virtual void    draw();
    void            add(Component* pComponent);
    void            add(Entity* pEntity);
    void            setParent(Entity* pParent);
    virtual void    keyDown(unsigned int key) {}
    virtual void    keyUp(unsigned int key) {}
    virtual void    mouseMoved(int x, int y, int winX, int winY) {}
    virtual void    onEvent(Events pType, void* pObject) {}
    void            setDesiredRotation(Matrix pRotation);
    void            setDesiredPosition(Vector3 pPosition);
    Matrix          getRotationMatrix();
    virtual void    onCollision(const ColliderComponent& pOther);
                    Entity();
    virtual         ~Entity();
};

