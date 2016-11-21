#ifndef HEXTILETEXTURE_H
#define HEXTILETEXTURE_H

#include "HexTile.h"
#include "GLText.h"

class HexTileTexture : public HexTile
{
public:
    /** Default constructor */
    HexTileTexture();

    bool Init(char fileName[]);
    /** Default destructor */
    virtual ~HexTileTexture();

    void Draw();
protected:
private:
    GLuint m_textureID;
    int textureW, textureH;
};

#endif // HEXTILETEXTURE_H
