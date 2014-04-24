#pragma once
class InputInterface {
public:
    virtual void    keyDown(unsigned int pKey) = 0;
    virtual void    keyUp(unsigned int pKey) = 0;
    virtual void    mouseMoved(int pX, int pY, int winX, int winY) = 0;
    virtual void    mouseWheel(int pDelta) = 0;
    virtual void    mouseDown(unsigned int pButton) = 0;
    virtual ~InputInterface() {}
};

