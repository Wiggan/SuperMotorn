#pragma once
#include "GameObject.h"
#include <vector>
#include <Component.h>
#include "ResourceLoader.h"
#include "Renderer.h"
class BaseCamera;
class Entity : public GameObject {
    Entity*                 mParent;
    std::vector<Entity*>    mChildren;
    std::vector<Component*> mComponents;
public:
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader, std::vector<BaseCamera*>* pCameras);
    virtual void    update(float pDelta);
    virtual Matrix  calculateLocalTransform();
    virtual void    draw();
    void            add(Component* pComponent);
    void            add(Entity* pEntity);
    void            setParent(Entity* pParent);
    virtual void    keyDown(unsigned int key) {}
    virtual void    keyUp(unsigned int key) {}
                    Entity();
    virtual         ~Entity();
};

