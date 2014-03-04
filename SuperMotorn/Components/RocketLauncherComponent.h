#pragma once
#include "Entity.h"
#include "RocketComponent.h"
#include <vector>

class RocketLauncherComponent : public Entity {
    std::vector<RocketComponent*> mRockets;
public:
                    RocketLauncherComponent();
    virtual void    keyDown(unsigned int key);
                    ~RocketLauncherComponent();
};

