#pragma once
#include "Entity.h"
#include "Rocket.h"
#include <vector>

class RocketLauncher : public Entity {
    std::vector<Rocket*> mRockets;
public:
                    RocketLauncher(int pTeam);
    virtual void    keyDown(unsigned int key);
                    ~RocketLauncher();
};

