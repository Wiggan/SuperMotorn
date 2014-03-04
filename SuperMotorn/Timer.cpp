#include "Timer.h"
#include <windows.h>

Timer::Timer() : mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
                mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false) {
    __int64 countsPerSecond;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
    mSecondsPerCount = 1.0 / (double)countsPerSecond;
}
void
Timer::tick() {
    if ( mStopped ) {
        mDeltaTime = 0.0;
        return;
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);
    mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
    mPrevTime = mCurrTime;
    mDeltaTime = max(mDeltaTime, 0.0);
}
float
Timer::getDeltaTime() const {
    return (float)mDeltaTime;
}
float
Timer::getTotalTime() const {
    if ( mStopped ) {
        return (float)((mStopTime - mPausedTime - mBaseTime) * mSecondsPerCount);
    } else {
        return (float)((mCurrTime - mPausedTime - mBaseTime) * mSecondsPerCount);
    }
}
float
Timer::getTotalTimeNow() const {
    if ( mStopped ) {
        return (float)((mStopTime - mPausedTime - mBaseTime) * mSecondsPerCount);
    } else {
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        return (float)((currTime - mPausedTime - mBaseTime) * mSecondsPerCount);
    }
}
void
Timer::start() {
    if ( mStopped ) {
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        mPausedTime += currTime - mStopTime;
        mPrevTime = currTime;
        mStopTime = 0;
        mStopped = false;
    }
}
void
Timer::reset() {
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    mBaseTime = currTime;
    mPrevTime = currTime;
    mStopTime = 0;
    mStopped = false;
}
void
Timer::stop() {
    if ( !mStopped ) {
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        mStopTime = currTime;
        mStopped = true;
    }
}
Timer::~Timer() {
}
