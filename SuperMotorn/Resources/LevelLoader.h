#pragma once
#include "World.h"
#include "ResourceLoader.h"
#include "PointLightComponent.h"
#include "MeshComponent.h"
#include "ColliderComponent.h"
#include "CameraComponent.h"
#include "FlagPlatform.h"
#include "tinyxml2.h"
#include "StartPoint.h"
class LevelLoader {
    ResourceLoader* mResourceLoader;
    Renderer*       mRenderer;

    PointLightComponent*        createPointLight(const tinyxml2::XMLNode* pNode);
    DirectionalLight*           createDirectionalLight(const tinyxml2::XMLNode* pNode);
    MeshComponent*              createMeshComponent(const tinyxml2::XMLElement* pNode);
    ColliderComponent*          createColliderComponent(const tinyxml2::XMLElement* pNode);
    CameraComponent*            createCameraComponent(const tinyxml2::XMLElement* pNode);
    Entity*                     createEntity(const tinyxml2::XMLNode* pNode);
    StartPoint*                 createStartPoint(const tinyxml2::XMLElement* pNode);
    FlagPlatform*               createFlagPlatform(const tinyxml2::XMLElement* pNode);
    Vector4                     createColor(const tinyxml2::XMLElement* pElement);
    Vector3                     createVector(const tinyxml2::XMLElement* pElement);

    void                        fillComponent(const tinyxml2::XMLNode* pNode, Component* pComponent);
    void                        fillEntity(const tinyxml2::XMLNode* pNode, Entity* pComponent);
    void                        fillGameObject(const tinyxml2::XMLNode* pNode, GameObject* pComponent);
public:
            LevelLoader(ResourceLoader& pResourceLoader, Renderer& pRenderer);
    void    loadLevel(const std::wstring& pFileName, World* pWorld);
            ~LevelLoader();
};

