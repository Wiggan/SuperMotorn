#include "World.h"
#include <DirectXMath.h>
#include "DebugRenderer.h"
World::World(Renderer* pRenderer) : mRenderer(pRenderer) {
}
void
World::init(ResourceLoader* pResourceLoader) {
    for ( auto it = mEntities.begin(); it != mEntities.end(); ++it ) {
        (*it)->init(mRenderer, pResourceLoader);
    } 
    //SeldomConstants sc;
    //sc.fogColor = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    //sc.fogStart = 1.0f;
    //sc.fogRange = 20.0f;
    //sc.directionalLight.ambient     = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
    //sc.directionalLight.diffuse     = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
    //sc.directionalLight.specular    = DirectX::XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);
    //sc.directionalLight.direction   = DirectX::XMFLOAT3(-1.1f, -1.0f, 0.1f);
    mRenderer->setSeldomConstants(mSeldomConstants);
}
void    
World::setDirectionalLight(DirectionalLight* pDirectionalLight) {
    mSeldomConstants.directionalLight = *pDirectionalLight;
}
void    
World::setFog(float pStart, float pRange, Vector4 pColor, bool pUseSkyColor) {
    mSeldomConstants.fogStart       = pStart;
    mSeldomConstants.fogRange       = pRange;
    mSeldomConstants.fogColor       = pColor;
    mSeldomConstants.useSkyColor    = pUseSkyColor;
}
void 
World::add(Entity* pEntity) {
    mEntities.push_back(pEntity);
}
void 
World::addStartPoint(StartPoint* pEntity) {
    if ( pEntity->getTeam() == 1 ) {
        mStartPointsTeam1.push_back(pEntity);
    } else {
        mStartPointsTeam2.push_back(pEntity);
    }
    mEntities.push_back(pEntity);
}
StartPoint* 
World::getStartPoint(int pTeam, int pPlayerId) {
    if ( pTeam == 1 ) {
        if ( pPlayerId < mStartPointsTeam1.size() ) {
            return mStartPointsTeam1[pPlayerId];
        } else {
            std::cout << "Error: not enough start points for team 1 in level!" << std::endl;
            return NULL;
        }
    } else {
        if ( pPlayerId < mStartPointsTeam2.size() ) {
            return mStartPointsTeam2[pPlayerId];
        } else {
            std::cout << "Error: not enough start points for team 1 in level!" << std::endl;
            return NULL;
        }
    }
}
void 
World::remove(Entity* pEntity) {
    for ( auto it = mEntities.begin(); it != mEntities.end(); ++it ) {
        if ( pEntity == (*it) ) {
            mEntities.erase(it);
            break;
        }
    }
}
void 
World::tick(float pDelta, const Timer* pTimer) {
    for ( auto it = mEntities.begin(); it != mEntities.end(); it++ ) {
        (*it)->update(pDelta);
    }
    //Vector3 dir(0.0f, 0.0f, 1.0f);
   // Vector3 axis(10.0f, 0.0f, 10.0f);
    //dir.rotate(axis, pTimer->getTotalTime());
    //DebugRenderer::instance()->renderCube(Vector3(0.0f,0.0f,0.0f), dir.dir2Rot());
    //DebugRenderer::instance()->renderCube(dir, Vector3(0.0f,0.0f,0.0f));
    //DebugRenderer::instance()->renderArrow(Vector3(0.0f,0.0f,0.0f), dir.dir2Rot());
    //DebugRenderer::instance()->renderArrow(Vector3(0.0f,0.0f,0.0f), axis.dir2Rot());
}
void 
World::draw() {
    for ( auto it = mEntities.begin(); it != mEntities.end(); it++ ) {
        (*it)->draw();
    }
    //DebugRenderer::instance()->renderCube(Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,0.0f,0.0f));
    //DebugRenderer::instance()->renderCube(Vector3(0.0f,0.0f,0.0f), Vector3(-DirectX::XM_PIDIV2,0.0f,0.0f));
    //DebugRenderer::instance()->renderCube(Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,DirectX::XM_PIDIV2,0.0f));

}

World::~World() {
}
