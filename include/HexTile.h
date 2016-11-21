#ifndef HEXTILE_H
#define HEXTILE_H


#include <GL/glew.h>
#include <math.h>

#include <SDL2/SDL.h>

class HexTile
{
public:
    /** Default constructor */
    HexTile();
    /** Default destructor */
    virtual ~HexTile();
    void setColor(int r, int g, int b, int a=255);
    virtual void Draw();
    virtual void DrawMask(int Reverse=0);
    float getRadius()
    {
        return Radius;
    }


    virtual void drawGrid();
    void setImpassable(bool value)
    {
        impassable = value;
    }
    bool getImpassable()
    {
        return impassable;
    }
protected:
    SDL_Color Color;
    float Radius;


    float gridScale;
    SDL_Color gridColor;
    bool gridOutline;

    bool impassable;
};

#endif // HEXTILE_H
