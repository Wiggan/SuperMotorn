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
    Vector3*                mStaticSpherePos;
    Vector3*                mStaticRayPos;
    Vector3*                mStaticRayDir;

    DebugRenderer();
    DebugRenderer(DebugRenderer&);
    ~DebugRenderer();
public:
    static void             init(Renderer* pRenderer, ResourceLoader* pResourceLoader);
    void                    renderSphere(Vector3 pPosition, Vector3 pScale = Vector3(1.0f,1.0f,1.0f));
    void                    renderCube(Vector3 pPosition, Vector3 pRotation, Vector3 pScale = Vector3(1.0f,1.0f,1.0f));
    void                    renderCube(Vector3 pPosition, Matrix pRotation, Vector3 pScale = Vector3(1.0f,1.0f,1.0f));
    void                    renderCube(Vector3 pPosition, Vector4 pQuaternion, Vector3 pScale = Vector3(1.0f,1.0f,1.0f));
    void                    renderArrow(Vector3 pPosition, Vector3 pRotation, Vector3 pScale = Vector3(1.0f,1.0f,1.0f));
    static DebugRenderer*   instance();
    void                    setStaticSphere(Vector3 pPosition);
    void                    setStaticRay(Vector3 pPosition, Vector3 pDirection);
    void                    renderMesh(Mesh* pMesh, Matrix pTransform);
    void                    renderMesh(Mesh* pMesh, Matrix pTransform, Material* pMaterial);
    void                    draw();
    void                    toggleActive();
};

