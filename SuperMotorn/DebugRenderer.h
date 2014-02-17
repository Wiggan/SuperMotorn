#pragma once
#include "Renderer.h"
#include "ResourceLoader.h"
class DebugRenderer {
    static DebugRenderer*   mInstance;
    Renderer*               mRenderer;
    Mesh*                   mSphere;
    Mesh*                   mCube;
    Mesh*                   mArrow;
    Material*               mMaterial;
    bool                    mActive;

    DebugRenderer();
    DebugRenderer(DebugRenderer&);
    ~DebugRenderer();
public:
    static void             init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    void                    renderSphere(Vector3 pPosition, Vector3 pScale = Vector3(1.0f,1.0f,1.0f));
    void                    renderCube(Vector3 pPosition, Vector3 pRotation, Vector3 pScale = Vector3(1.0f,1.0f,1.0f));
    void                    renderArrow(Vector3 pPosition, Vector3 pRotation, Vector3 pScale = Vector3(1.0f,1.0f,1.0f));
    static DebugRenderer*   instance();
    void                    toggleActive();
};

