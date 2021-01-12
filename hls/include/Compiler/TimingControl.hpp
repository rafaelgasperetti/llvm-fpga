#ifndef TIMINGCONTROL_H
#define	TIMINGCONTROL_H

class TimingControl
{
    public:
        void SetStartClock(int);
        int GetStartClock();
        void SetCycleClockCount(int);
        int GetCycleClockCount();

    private:
        int StartClock = 0;
        int CycleClockCount = 0;
};

#endif
