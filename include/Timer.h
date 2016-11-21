#ifndef TIMER_H
#define TIMER_H


class Timer
{
public:
    /** Default constructor */
    Timer();
    /** Default destructor */
    virtual ~Timer();

    void Start();
    void Reset();
    float getTicksElapsed();
    float getSecondsElapsed();
protected:
    float startTime;
private:
};

#endif // TIMER_H
