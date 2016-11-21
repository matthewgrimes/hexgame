#ifndef CURSOR_H
#define CURSOR_H


#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "HexUtils.h"
#include "Timer.h"
class Cursor
{
public:
    /** Default constructor */
    Cursor();
    /** Default destructor */
    virtual ~Cursor();

    void Update(float angle, int I, int J);

    void Draw();

    int getHexI()
    {
        return Position.getI();
    }
    int getHexJ()
    {
        return Position.getJ();
    }
    int getLevel()
    {
        return Position.getLevel();
    }
    HexCoordinate2D getHexCoordIJ() const
    {
        return HexCoordinate2D(Position.getI(), Position.getJ());
    }

    void setPosition(HexCoordinate3D value);

    bool isActive()
    {
        return Active;
    }
    void Activate()
    {
        Active = true;
    }
    void Deactivate()
    {
        Active = false;
    }

    int getRadius()
    {
        return radius;
    }
    void setRadius( int value )
    {
        radius = value;
    }
protected:
private:

    HexCoordinate3D Position;

    Timer m_AnimTimer;
    float frame_count;

    bool Active;

    int radius;
};

#endif // CURSOR_H
