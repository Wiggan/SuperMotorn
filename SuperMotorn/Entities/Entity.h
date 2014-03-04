#pragma once
#include "GameObject.h"
#include <vector>
#include <Component.h>
#include "ResourceLoader.h"
#include "Renderer.h"
#include <DirectXCollision.h>
class BaseCamera;
class ColliderComponent;
class Entity : public GameObject {
    Entity*                 mParent;
    std::vector<Component*> mComponents;
    Vector3                 mPreviousPosition;
protected:
    std::vector<Entity*>    mChildren;
public:
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void    update(float pDelta);
    virtual Matrix  calculateLocalTransform();
    virtual void    draw();
    void            add(Component* pComponent);
    void            add(Entity* pEntity);
    void            setParent(Entity* pParent);
    virtual void    keyDown(unsigned int key) {}
    virtual void    keyUp(unsigned int key) {}
    virtual void    onCollision(const ColliderComponent& pOther) {}
    virtual void    setPosition(const Vector3& pPosition);
    Vector3         getPreviousPosition();
                    Entity();
    virtual         ~Entity();
};

