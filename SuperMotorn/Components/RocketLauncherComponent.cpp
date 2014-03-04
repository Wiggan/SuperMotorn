#include "RocketLauncherComponent.h"


RocketLauncherComponent::RocketLauncherComponent() {
    
}
void
RocketLauncherComponent::keyDown(unsigned int key) {
    if ( ' ' == (char)key ) {
        for ( auto it = mRockets.begin(); it != mRockets.end(); ++it ) {
            if ( (*it)->isFree() ) {
                (*it)->fire(getWorldPosition(), getWorldDirection());
                break;
            }
        }
    }
}
RocketLauncherComponent::~RocketLauncherComponent() {
}
