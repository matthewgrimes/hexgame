#ifndef ACTOR_H
#define ACTOR_H

#include "AnimatedMesh.h"
#include <SDL2/SDL.h>
#include "GLOBALS.h"
#include "HexUtils.h"
#include "Timer.h"
#include <vector>

#define DELAYPERTILE 1.0f
#define BASEATTACKDELAY 1.0f

//! Class for managing moving objects on the map
class Actor : public AnimatedMesh
{
public:
    /** Default constructor */
    Actor();
    /** Default destructor */
    virtual ~Actor();

    /** Draw to screen */
    virtual void Render();
    void setPos(float x, float y);

    void setLevel(int level)
    {
        Position.setLevel(level);
    }

    /** Get hex I coordinate */
    int getI();
    /** Get hex J coordinate */
    int getJ();
    HexCoordinate2D getHexCoordIJ() const
    {
        return HexCoordinate2D(Position.getI(), Position.getJ());
    }
    void setPosition(int i, int j);

    /** Update things like timers, loops, etc */
    void Update();

    /** Motion queue: stores a sequence of directions to move */
    std::vector<Directions> moveList;
    /** Check for motion.  Equivalent to !moveList.empty() */
    bool isMoving()
    {
        return Moving;
    }

    /** Set the actor's angle. Facing = 60*Angle */
    void setAngle(float angle)
    {
        Angle = angle;
    }
    int getAngle()
    {
        return Angle;
    }
    int getDirection()
    {
        return Angle/60;
    }
    void setDirection(int value)
    {
        Angle = 60*value;
    }

    int getLevel()
    {
        return Position.getLevel();
    }

    double getTurnCountdown()
    {
        return turnCountdown;
    }
    void incrementTurnCountdown(double v)
    {
        turnCountdown += v;
    }
    void scaleTurnCountdown(double v)
    {
        turnCountdown *= v;
    }
    void setTurnCountdown(double v)
    {
        turnCountdown = v;
    }

    static bool turnOrderCompareFunction( const Actor *a, const Actor *b);
    virtual void RenderPortrait();
protected:
    Timer m_AnimationTimer;

    bool Moving;
    Timer m_MotionTimer;

    float Angle;

    HexCoordinate3D Position;

    double turnCountdown;

    Mesh m_Portrait;
    Mesh m_PortraitHealthBar;

private:
};

#endif // ACTOR_H
