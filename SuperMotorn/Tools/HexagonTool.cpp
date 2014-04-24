#include "HexagonTool.h"
#include "HexaTerrainResource.h"
#include "DebugRenderer.h"
#include "Matrix.h"
#include "Ray.h"
HexagonTool::HexagonTool(HexaTerrain* pHexa) : mHexa(pHexa){
}
void
HexagonTool::keyDown(unsigned int key) {}
void
HexagonTool::keyUp(unsigned int key) {}
void    
HexagonTool::mouseMoved(int pX, int pY, int winX, int winY) {
    static long iterations = 0;
    static long boxes = 0;
    static long faces = 0;
    iterations++;
    if ( iterations % 100 == 0 ) {
        std::cout << "boxes mean: " << boxes / iterations << " faces mean: " << faces / iterations << std::endl;
    }
    int screenX = pX - winX - 8;
    int screenY = pY - winY - 21;

    HexaTerrainResource* resource = mHexa->getTerrainResource();
    Mesh* hexa = resource->getMeshes()->front();
    

    Ray ray;
    float p00 = ((DirectX::XMFLOAT4X4)mProjection)(0, 0);
    float p11 = ((DirectX::XMFLOAT4X4)mProjection)(1, 1);
    float x = ((2.0f * screenX) / mWidth - 1.0f) / p00;
    float y = ((-2.0f * screenY) / mHeight + 1.0f) / p11;
    ray.setDirection(Vector3(x, y, 1.0f).normalized());
    Matrix toLocal = mCamera->getViewTransform().inversed() * mHexa->getTerrainResource()->getTransforms()->front().inversed();
    ray = ray*toLocal;
    float dist;
    for ( auto it = resource->getOctreeBoxes()->begin(); it != resource->getOctreeBoxes()->end(); ++it ) {
        boxes++;
        if ( ray.intersects((*it)->box, dist) ) {
            std::vector<int>* indices = &(*it)->indices;
            int faceCount = indices->size() / 3;
            std::vector<Vector3>* pos = hexa->getPositions();
            for ( int i = 0; i < faceCount; i++ ) {
                faces++;
                if ( ray.intersects((*pos)[(*indices)[i * 3]], (*pos)[(*indices)[i * 3 + 1]], (*pos)[(*indices)[i * 3 + 2]], dist) ) {
                    Vector3 middle = ((*pos)[(*indices)[i * 3]] + (*pos)[(*indices)[i * 3 + 1]] + (*pos)[(*indices)[i * 3 + 2]]) * 0.333333f;
                    mPickedHexa = resource->getHexaCoordFromLocalCoord(middle);
                    mouseOver(mPickedHexa);
                    //DebugRenderer::instance()->renderCube((*it)->box.Center, Vector3(0.0f, 0.0f, 0.0f), Vector3((*it)->box.Extents));
                    return;
                }
            }
        }
    }
}
void
HexagonTool::mouseDown(UINT button) {
    if ( button == 0 ) {
        leftAction(mPickedHexa);
    } else {
        rightAction(mPickedHexa);
    }
}
void    
HexagonTool::mouseWheel(int delta) {

}
void
HexagonTool::setCamera(BaseCamera*  pCamera) {
    mCamera = pCamera;
}
void            
HexagonTool::setProjection(const Matrix& pProjection, int pWidth, int pHeight) {
    mProjection = pProjection;
    mWidth = pWidth;
    mHeight = pHeight;
}
HexagonTool::~HexagonTool() {
}
