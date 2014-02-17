#include "LevelLoader.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Util.h"
#include "DirectionalLight.h"
LevelLoader::LevelLoader(ResourceLoader& pResourceLoader, Renderer& pRenderer) : mResourceLoader(&pResourceLoader), mRenderer(&pRenderer) {
}
void    
LevelLoader::loadLevel(const std::wstring& pFileName, World* pWorld) {
    using namespace tinyxml2;
    tinyxml2::XMLDocument level;
    level.LoadFile(Util::wstring2string(Util::searchDirectory(L"..", pFileName)).c_str());
    if ( level.ErrorID() == 0 ) {
        XMLElement* world = level.FirstChildElement("world");
        pWorld->setDirectionalLight(createDirectionalLight(world->FirstChildElement("directionalLight")));
        if ( world->FirstChildElement("skyBox") ) {
            pWorld->setSkyBox(mResourceLoader->getResource<Material>(Util::string2wstring(world->FirstChildElement("skyBox")->FirstChildElement("materialFile")->GetText())));
        }
        XMLElement* fog = world->FirstChildElement("fog");
        float start = 0.0f;
        float range = 0.0f;
        bool useSkyColor = false;
        fog->QueryFloatAttribute("start", &start);
        fog->QueryFloatAttribute("range", &range);
        fog->QueryBoolAttribute("useSky", &useSkyColor);
        pWorld->setFog(start, range, createColor(fog->FirstChildElement("color")), useSkyColor);
        for ( const XMLElement* child = world->FirstChildElement("entity"); child != NULL; child = child->NextSiblingElement("entity") ) {
            pWorld->add(createEntity(child));
        }
    } else {
        std::cout << "Error loading level!" << std::endl;
    }
}
MeshComponent*              
LevelLoader::createMeshComponent(const tinyxml2::XMLElement* pNode) {
    std::string meshFile = pNode->FirstChildElement("meshFile")->GetText();
    const tinyxml2::XMLElement* materialFile = pNode->FirstChildElement("materialFile");
    MeshComponent* mesh = new MeshComponent(mRenderer, mResourceLoader->getResource<Mesh>(Util::string2wstring(meshFile)));
    fillComponent(pNode, mesh);
    if ( materialFile != NULL ) {
        mesh->setMaterial(mResourceLoader->getResource<Material>(Util::string2wstring(materialFile->GetText())));
    } else {
        mesh->setMaterial(mResourceLoader->getResource<Material>(L"default.xml"));
    }
    return mesh;
}
CameraComponent*              
LevelLoader::createCameraComponent(const tinyxml2::XMLElement* pNode) {
    CameraComponent* camera = new CameraComponent();
    fillComponent(pNode, camera);
    return camera;
}
Entity* 
LevelLoader::createEntity(const tinyxml2::XMLNode* pNode) {
    Entity* entity = new Entity();
    fillEntity(pNode, entity);
    return entity;
}
DirectionalLight*
LevelLoader::createDirectionalLight(const tinyxml2::XMLNode* pNode) {
    using namespace tinyxml2;
    DirectionalLight* light = new DirectionalLight();
    if ( pNode != NULL ) {
        const XMLElement* directionNode = pNode->FirstChildElement("direction");
        const XMLElement* ambientNode = pNode->FirstChildElement("ambient");
        const XMLElement* diffuseNode = pNode->FirstChildElement("diffuse");
        const XMLElement* specularNode = pNode->FirstChildElement("specular");
        if ( directionNode != NULL ) {
            light->direction = createVector(directionNode).normalized();
        }
        if ( ambientNode != NULL ) {
            light->ambient = createColor(ambientNode);
        }
        if ( diffuseNode != NULL ) {
            light->diffuse = createColor(diffuseNode);
        }
        if ( specularNode != NULL ) {
            light->specular =createColor(specularNode);
        }
    }
    return light;
}
PointLightComponent* LevelLoader::createPointLight(const tinyxml2::XMLNode* pNode) {
    using namespace tinyxml2;
    using namespace DirectX;
    const XMLElement* positionNode = pNode->FirstChildElement("position");
    const XMLElement* ambientNode = pNode->FirstChildElement("ambient");
    const XMLElement* diffuseNode = pNode->FirstChildElement("diffuse");
    const XMLElement* specularNode = pNode->FirstChildElement("specular");
    const XMLElement* attenuationNode = pNode->FirstChildElement("attenuation");
    const XMLElement* rangeNode = pNode->FirstChildElement("range");
    PointLight pl;
    if ( rangeNode != NULL ) {
        rangeNode->QueryFloatText(&pl.range);
    }
    if ( attenuationNode != NULL ) {
        pl.attenuation = createVector(attenuationNode);
    }
    if ( ambientNode != NULL ) {
        pl.ambient = createColor(ambientNode);
    }
    if ( diffuseNode != NULL ) {
        pl.diffuse = createColor(diffuseNode);
    }
    if ( specularNode != NULL ) {
        pl.specular = createColor(specularNode);
    }
    PointLightComponent* light = new PointLightComponent(mRenderer, pl);
    fillComponent(pNode, light);
    return light;
}
void                        
LevelLoader::fillComponent(const tinyxml2::XMLNode* pNode, Component* pComponent) {
    fillGameObject(pNode, pComponent);
}
void                        
LevelLoader::fillEntity(const tinyxml2::XMLNode* pNode, Entity* pEntity) {
    fillGameObject(pNode, pEntity);
    for ( const tinyxml2::XMLElement* child = pNode->FirstChildElement(); child != NULL; child = child->NextSiblingElement() ) {
        if ( strcmp("entity", child->Name()) == 0 ) {
            pEntity->add(createEntity(child));
        } else if ( strcmp("mesh", child->Name()) == 0 ) {
            pEntity->add(createMeshComponent(child));
        } else if ( strcmp("camera", child->Name()) == 0 ) {
            pEntity->add(createCameraComponent(child));
        } else if ( strcmp("pointLight", child->Name()) == 0 ) {
            pEntity->add(createPointLight(child));
        }
    }
}
void                        
LevelLoader::fillGameObject(const tinyxml2::XMLNode* pNode, GameObject* pGameObject) {
    using namespace tinyxml2;
    const XMLElement* positionNode = pNode->FirstChildElement("position");
    const XMLElement* rotationNode = pNode->FirstChildElement("rotation");
    const XMLElement* scaleNode = pNode->FirstChildElement("scale");
    if ( positionNode != NULL ) {
        pGameObject->setPosition(createVector(positionNode));
    }
    if ( rotationNode != NULL ) {
        pGameObject->setRotation(createVector(rotationNode));
    }
    if ( scaleNode != NULL ) {
        pGameObject->setScale(createVector(scaleNode));
    }
}
Vector3 
LevelLoader::createVector(const tinyxml2::XMLElement* element) {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if ( element != NULL ) {
        element->QueryFloatAttribute("x", &x);
        element->QueryFloatAttribute("y", &y);
        element->QueryFloatAttribute("z", &z);
    }
    return Vector3(x, y, z);
}
Vector4 
LevelLoader::createColor(const tinyxml2::XMLElement* element) {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
    if ( element != NULL ) {
        element->QueryFloatAttribute("r", &r);
        element->QueryFloatAttribute("g", &g);
        element->QueryFloatAttribute("b", &b);
        element->QueryFloatAttribute("a", &a);
    }
    return Vector4(r, g, b, a);
}
LevelLoader::~LevelLoader() {
}
