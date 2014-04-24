#pragma once
#include "Component.h"
#include "InputInterface.h"
class Client;
class InputComponent : public Component, public InputInterface {
    Client* mClient;
public:
    static std::vector<InputInterface*>  inputs;

                    InputComponent(Client* pClient);
                    InputComponent();
    virtual void    init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    virtual void    keyDown(unsigned int key);
    virtual void    keyUp(unsigned int key);
    virtual void    mouseMoved(int x, int y, int winX, int winY);
    virtual void    mouseWheel(int delta) {}
    virtual void    mouseDown(unsigned int pButton) {}
    void            lerpTo(Vector3 pPosition, Vector3 pRotation, Matrix pRotationMatrix);
    virtual void    update(float pDelta) {}
                    ~InputComponent();
};

