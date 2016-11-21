#include "HexMap.h"
#include "HexUtils.h"
#include "GLOBALS.h"
#include "Pipeline.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

#include <GL/glew.h>
#include <deque>
#include <utility>
#include <fstream>
#include <sstream>

void setUniforms(int i, int j, int layer)
{

}

void HexMapFileLoader::parseLine(const std::string& field, const std::string& data)
{
    int i, j, layer, layers, mapdata;

    if ( field == "size" )
    {
        std::istringstream iss(data);
        iss >> hexmap->Isize;
        iss >> hexmap->Jsize;
    }
    else if ( field.compare("layers") == 0 )
    {
        layers = ParseInt(data);
        std::unordered_map< int, std::unordered_map<int, int> > Layer;
        for (int i = 0; i < layers; i ++)
        {
            hexmap->Data.push_back(Layer);
        }
    }
    else if ( field.compare("tileset") == 0 )
    {
        hexmap->tileSetString = ParseString(data);
    }
    else if (field.compare("data") == 0)
    {
        std::istringstream iss(data);
        iss >> i;
        iss >> j;
        iss >> layer;
        iss >> mapdata;
        hexmap->setCurrentData(i,j,layer,mapdata);
    }
    else if (field.compare("obstacle") == 0 )
    {
        std::istringstream iss(data);
        iss >> i;
        iss >> j;
        iss >> mapdata;
        hexmap->obstacleData[i][j] = mapdata;
    }
    else if (field.compare("decoration") == 0 )
    {
        std::istringstream iss(data);
        iss >> i;
        iss >> j;
        iss >> mapdata;
        hexmap->decorationData[i][j] = mapdata;
    }
    else if (field == "lighting")
    {
        hexmap->m_LightingSetup.loadFromFile(std::string("data/maps/lighting/")+data);
    }

}

HexMap::HexMap()
{
    //ctor
    gridOn = true;
    clearDrawings();
}

HexMap::~HexMap()
{
    clearDrawings();
}

bool HexMap::loadFromFile(const std::string& fileName)
{
    HexMapFileLoader loader(fileName);
    loader.setHexMap(this);
    loader.parseFile();
    return 1;
}

unsigned int HexMap::getHeight(int i, int j)
{
    //! default to maximum height and work down
    int height = Data.size()-1;
    while ( height >= 0 && getCurrentData(i, j, height) == 0 )
    {
        height--;
    }
    return height;
}

void HexMap::Init()
{
    currentTileset.Init();
    m_LightingSetup.Init();
    currentTileset.loadFromFile(tileSetString.c_str());
    clearDrawings();
    m_TileAnimationTimer.Start();
    initTileTransitionData();
}

void HexMap::moveViewToPosition(int i, int j, unsigned int layer)
{
    Pipeline::getInstance()->setObjectPos( Vector3f(2*HORZDIST*i + HORZDIST * j, TILETHICKNESS*1.f*layer, VERTDIST * j ));
}

void HexMap::drawTile(int i, int j, unsigned int layer)
{
    int tileType;

    moveViewToPosition(i, j, layer);

    tileType = getCurrentData( i, j, layer);

    glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(), "gI"), i + (j - j % 2) / 2);
    glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(), "gJ"), j);
    glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(), "gLayer"), layer + 1);

    //ShaderManager::getInstance()->ActivatePicking();

    //static_cast<HexTileMesh*>(currentTileset.getMember( tileType ))->Render();

    //ShaderManager::getInstance()->DisablePicking();

    glUniform1f( glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"waveTime"), m_TileAnimationTimer.getSecondsElapsed());
    glUniform1i( glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"isWater"), currentTileset.getMember(tileType)->getImpassable());
    glUniform1i( glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"drawGrid"), gridOn && (tileType != 0) && layer == (unsigned int)getHeight(i,j));
    //
    // if the tile is set to a special color, draw that
    if (cursorData[i][j] != 0 && layer == (unsigned int)getHeight(i,j) &&
            ( ! ( getDistance(i,j,mainCursor.getHexI(),mainCursor.getHexJ()) <= mainCursor.getRadius() ) ||
              (getDistance(i,j,mainCursor.getHexI(),mainCursor.getHexJ()) <= mainCursor.getRadius() &&
               abs( getHeight(i,j) - getHeight(mainCursor.getHexI(), mainCursor.getHexJ()) ) >= 2 ) )
       )
    {

        //glUniform1i( glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"isWater"), 0);
        ShaderManager::getInstance()->setUniform(true, "gOverrideTexture");
        ShaderManager::getInstance()->setUniform(2,"gOverrideSampler");
        ResourceManager::getSingleton()->getTexture(currentTileset.getAuxTileTexture(floor(cursorData[i][j])))->Bind(GL_TEXTURE2);
        //currentTileset.getAuxTile(floor(cursorData[i][j]))->getTexture()->Bind(GL_TEXTURE2);
        currentTileset.getMember( tileType )->Draw();
        //currentTileset.getAuxTile(floor(cursorData[i][j]))->Draw();
        ShaderManager::getInstance()->setUniform(false, "gOverrideTexture");


    }
    // if the tile is occupied by the cursor, draw that
    else if ( mainCursor.getLevel() >= 0 && getDistance(i,j,mainCursor.getHexI(),mainCursor.getHexJ()) <= mainCursor.getRadius() &&
              mainCursor.isActive() && layer == (unsigned int)getHeight(i,j) &&
              abs( getHeight(i,j) - getHeight(mainCursor.getHexI(), mainCursor.getHexJ()) ) < 2
            )
    {
        //glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(), "drawGrid"), 1);
        //glUniform1i( glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"isWater"), 0);
        //currentTileset.getAuxTile(0)->Draw();
        ShaderManager::getInstance()->setUniform(true, "gOverrideTexture");
        ShaderManager::getInstance()->setUniform(2,"gOverrideSampler");
        ResourceManager::getSingleton()->getTexture(currentTileset.getAuxTileTexture(0))->Bind(GL_TEXTURE2);
        currentTileset.getMember( tileType )->Draw();
        ShaderManager::getInstance()->setUniform(false, "gOverrideTexture");
    }
    // if all else fails, and there's map data, draw that
    else if (tileType != 0)
    {
        currentTileset.getMember( tileType )->Draw();
    }
    glUniform1i( glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"isWater"), 0);
    glUniform1i( glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"drawGrid"), 0);

}

void HexMap::Draw()
{
    int i, j;
    HexMapIterator iterator(Isize,Jsize);
    iterator.Reset(i,j);

    m_LightingSetup.Bind();
    while( iterator.Iterate(i,j) )
    {
        for (unsigned int layer = 0; layer <= (unsigned int)getHeight(i,j); layer++)
        {
            drawTile(i, j, layer);
        }
        drawDecorationAndObstacle(i, j, (unsigned int)getHeight(i,j) );
    }

}

void HexMap::drawDecorationAndObstacle(int i, int j, unsigned int layer)
{
    moveViewToPosition(i, j, layer);

    if ( obstacleData[i][j]!= 0 )
    {
        currentTileset.getObstacle(obstacleData[i][j])->Render();
    }
    if ( decorationData[i][j]!= 0)
    {
        currentTileset.getDecoration(decorationData[i][j])->Render();
    }

}

std::map< std::pair<int, int>, std::vector<Directions> > HexMap::drawMoveCircle(int i, int j, int r, int maxjump)
{
    std::map< std::pair<int, int>, std::vector<Directions> > Return;
    std::deque< std::pair<int, int> > frontier;
    frontier.push_back(std::make_pair(i,j));

    return Return;
}

std::map<std::pair<int, int>, int > HexMap::drawFilledCircle(int i0, int j0, int radius, bool actorsok, int maxheightdiff, int color, bool colorGradient, bool terrain)
{
    std::deque<std::pair<int, int> > Frontier;
    std::map< std::pair<int, int>, int> distance;
    int newDistance;

    Frontier.push_back(std::make_pair(i0,j0));
    cursorData[i0][j0] = color;
    distance[Frontier.front()] = 0;

    std::map< std::pair<int, int>, int > cameFrom;

    int newI, newJ;
    int heightDistanceModifier;
    while (!Frontier.empty())
    {
        int i = Frontier.front().first;
        int j = Frontier.front().second;
        for (int d = 0; d < 6; d ++)
        {
            newI = NeighborI(i, j, (Directions)d);
            newJ = NeighborJ(i, j, (Directions)d);
            // set newdistance to previous
            newDistance = distance[Frontier.front()];
            // add 1
            newDistance++;
            // add height difference - maxheightdifference (agility)
            heightDistanceModifier=0;//abs(getHeight(newI,newJ) - getHeight(i,j));

            newDistance+=heightDistanceModifier;

            if (   ( floor(cursorData[newI][newJ]) != color ||
                     newDistance < distance[std::make_pair(newI,newJ)]) &&
                    newDistance <= radius &&
                    getCurrentData(newI,newJ)!=0 &&
                    abs( getHeight(i,j) - getHeight(newI,newJ) ) <= maxheightdiff &&
                    ( actorsok || ( actorLocations[newI][newJ]!=1 && obstacleData[newI][newJ] == 0 ) ) &&
                    (!terrain || !(currentTileset.getMember(getCurrentData(newI,newJ))->getImpassable()) )
               )
            {
                Frontier.push_back(std::make_pair(newI,newJ));
                cameFrom[Frontier.back()] = getDirection(i, j, newI, newJ);
                distance[Frontier.back()] = newDistance;
                cursorData[newI][newJ] = color + (int)colorGradient * newDistance * 1.f/(float)(radius+1);
            }
        }
        Frontier.pop_front();
    }
    return cameFrom;
}

void HexMap::drawRing(int i0, int j0, int radius0, int radius1, int color)
{
    drawFilledCircle(i0,j0,radius1,true, 5, color,false, false);
    if ( radius0 > 0 )
    {
        drawFilledCircle(i0,j0,radius0 - 1, true, 5, 0,false, false);
    }
}

void HexMap::clearDrawings()
{
    cursorData.clear();
}

HexCoordinate3D HexMap::getMouseTile()
{
    int x,y;
    GLint Viewport[4];

    PickingFBO::PixelInfo Pixel;

    glGetIntegerv(GL_VIEWPORT, Viewport);
    SDL_GetMouseState(&x,&y);
    Pixel = ShaderManager::getInstance()->readFBO(x,Viewport[3]-y);
    HexCoordinate3D Return(-1,-1,-1);
    if ( Pixel.gLayer > 0 )
    {
        Return.setJ( floor(Pixel.gJ * 20 + 0.5) );
        Return.setI( floor(Pixel.gI * 20  - ( Return.getJ() - Return.getJ()%2 )/2 + 0.5) );
        Return.setLevel( Pixel.gLayer * 10 - 1 );
    }
    return Return;
}

void HexMap::Save()
{
    std::ofstream file;
    file.open( "data/maps/newmap.map" );
    file << "layers " << Data.size() << "\n";
    file << "tileset data/tilesets/tileset001.ts" << "\n";
    for ( int j = 0; j < Isize; j ++)
    {
        for (int i = -(j - j%2 )/2 ; i < Jsize - (j - j%2) / 2; i ++)
        {
            for (int l = 0; l <= getHeight(i,j); l++)
            {
                file << "data " << i << " " << j << " " << l << " " << getCurrentData(i,j,l) << "\n";
            }
            if ( obstacleData[i][j]!= 0 )
            {
                file << "obstacle " << i << " " << j << " " << obstacleData[i][j] << "\n";
            }
            if ( decorationData[i][j]!= 0 )
            {
                file << "decoration " << i << " " << j << " " << decorationData[i][j] << "\n";
            }

        }
    }

    file.close();
}

void HexMap::getSize(int &isize, int &jsize)
{
    isize = Isize;
    jsize = Jsize;
}

int HexMap::getCurrentTileSetSize()
{
    return currentTileset.getSize();
}

Tileset* HexMap::getTileset()
{
    return &currentTileset;
}

void HexMap::initTileTransitionData()
{
    int i, j, layer;
    HexMapIterator iterator(Isize,Jsize);
    iterator.Reset(i,j);


    while( iterator.Iterate(i,j) )
    {
        layer = (unsigned int)getHeight(i,j);
        int Data = getCurrentData(i,j,layer);
        for (unsigned int d = 0; d < 6; d++)
        {
            int newI = NeighborI(i,j,(Directions)d);
            int newJ = NeighborJ(i,j,(Directions)d);

            if ( getCurrentData(newI,newJ,layer) !=  Data )
            {
                tileTransitionData[i][j] = tileTransitionData[i][j] | DirectionsBitFlag(d);
            }
        }
    }
}
