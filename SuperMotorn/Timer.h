#pragma once
class Timer {
    double  mSecondsPerCount;
    double  mDeltaTime;
    __int64 mCurrTime;
    __int64 mPrevTime;
    __int64 mBaseTime;
    __int64 mPausedTime;
    __int64 mStopTime;
    bool    mStopped;
public:
            Timer();
            ~Timer();
    float   getTotalTime() const;
    float   getTotalTimeNow() const;
    float   getDeltaTime() const;
    void    reset();
    void    start();
    void    stop();
    void    tick();
};

