#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Renderer.h"

class MeshComponent : public Component {
protected:
    Renderer*                           mRenderer;
    Mesh*                               mMesh;
    Material*                           mMaterial;

public:
                            MeshComponent(Renderer* pRenderer, Mesh* pMesh);
    virtual void            draw();
    Material*               getMaterial();
    Mesh*                   getMesh();
    void                    setMaterial(Material* pMaterial);
                            ~MeshComponent();
};

