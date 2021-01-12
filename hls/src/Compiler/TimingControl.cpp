#include<Compiler/TimingControl.hpp>

void TimingControl::SetStartClock(int startClock)
{
    if(startClock >= 0)
    {
        StartClock = startClock;
    }
    else
    {
        StartClock = 0;
    }
}

int TimingControl::GetStartClock()
{
    return StartClock;
}

void TimingControl::SetCycleClockCount(int cycleClockCount)
{
    if(cycleClockCount >= -1)
    {
        CycleClockCount = cycleClockCount;
    }
    else
    {
        CycleClockCount = -1;
    }
}

int TimingControl::GetCycleClockCount()
{
    return CycleClockCount;
}
