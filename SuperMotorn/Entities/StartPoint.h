#pragma once
#include "Entity.h"
class StartPoint : public Entity {
    int     mTeam;
    bool    mUsed;
public:
    StartPoint(int pTeam);
    virtual void init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    void setUsed();
    bool isUsed();
    int getTeam();
    ~StartPoint();
};

