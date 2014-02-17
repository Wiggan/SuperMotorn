#pragma once
#include "Component.h"
class InputComponent : public Component {
public:
    InputComponent();
    void    keyDown(unsigned int key);
    void    keyUp(unsigned int key);
    virtual void update(float pDelta) {}
    ~InputComponent();
};

