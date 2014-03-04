#include "RocketLauncher.h"


RocketLauncher::RocketLauncher(int pTeam) {
    for ( int i = 0; i < 5; i++ ) {
        Rocket* rocket = new Rocket(pTeam);
        mRockets.push_back(rocket);
        mChildren.push_back(rocket);
    }
}
void
RocketLauncher::keyDown(unsigned int key) {
    if ( ' ' == (char)key ) {
        for ( auto it = mRockets.begin(); it != mRockets.end(); ++it ) {
            if ( (*it)->isFree() ) {
                (*it)->fire(getWorldPosition(), getWorldDirection());
                break;
            }
        }
    }
}
RocketLauncher::~RocketLauncher() {
}
