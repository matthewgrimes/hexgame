#ifndef HEXMAP_H
#define HEXMAP_H

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "Tileset.h"

#include "Cursor.h"
#include "FileLoader.h"
#include "GLOBALS.h"
#include "HexUtils.h"
#include "Timer.h"

#include "LightingSetup.h"

class HexMap {
  friend class HexMapFileLoader;

public:
  /** Default constructor */
  HexMap();
  /** Default destructor */
  virtual ~HexMap();

  bool loadFromFile(const std::string &fileName);
  //! Draw routine
  void Draw();

  //! Method for working with back layer
  void setCurrentData(int i, int j, int value) { Data.front()[i][j] = value; }
  //! Method for working with back layer
  int getCurrentData(int i, int j) { return Data.front()[i][j]; }

  //! Method for working with any layer
  void setCurrentData(int i, int j, int layer, int value) {
    Data[layer][i][j] = value;
  }
  //! Method for working with any layer
  int getCurrentData(int i, int j, int layer) { return Data[layer][i][j]; }

  //! Get the height at a position
  unsigned int getHeight(int i, int j);

  std::map<std::pair<int, int>, std::vector<Directions>>
  drawMoveCircle(int i, int j, int r, int maxjump);

  void Init();

  HexCoordinate3D getMouseTile();

  void toggleGrid() { gridOn = !gridOn; }

  Cursor mainCursor;

  std::map<std::pair<int, int>, int>
  drawFilledCircle(int i0, int j0, int radius, bool actorsok, int maxheightdiff,
                   int color, bool colorGradient = true, bool terrain = true);
  void drawRing(int i0, int j0, int radius0, int radius1, int color);

  float getCursorData(int i, int j) { return cursorData[i][j]; }
  std::unordered_map<int, std::unordered_map<int, float>> getCursorData() {
    return cursorData;
  }

  void setCursorData(int i, int j, int value) { cursorData[i][j] = value; }

  void
  setCursorData(std::unordered_map<int, std::unordered_map<int, float>> value) {
    cursorData = value;
  }

  void clearDrawings();

  void addActorLocation(int i, int j) { actorLocations[i][j] = 1; }
  void clearActorLocations() { actorLocations.clear(); }

  int getCurrentTileSetSize();
  Tileset *getTileset();

  int getNumLayers() { return Data.size(); }

  void Save();
  void getSize(int &isize, int &jsize);

protected:
private:
  LightingSetup m_LightingSetup;

  Tileset currentTileset;
  std::string tileSetString;

  std::vector<std::unordered_map<int, std::unordered_map<int, int>>> Data;
  std::unordered_map<int, std::unordered_map<int, int>> decorationData;
  std::unordered_map<int, std::unordered_map<int, int>> obstacleData;
  std::unordered_map<int, std::unordered_map<int, float>> cursorData;
  std::unordered_map<int, std::unordered_map<int, int>> actorLocations;

  std::unordered_map<int, std::unordered_map<int, int>>
      tileTransitionData; /**< the content of (i,j) is a bit flag corresponding
                             to which neighbors are DIFFERENT */
  void initTileTransitionData(); /**< initialize the transition data once the
                                    map is loaded.*/

  void moveViewToPosition(const int i, const int j, const unsigned int layer);
  void drawTile(const int i, const int j, const unsigned int layer);
  void drawDecorationAndObstacle(const int i, const int j,
                                 const unsigned int layer);

  bool gridOn;

  int Isize;
  int Jsize;

  int mapCenterI;
  int mapCenterJ;

  Timer m_TileAnimationTimer;
};

class HexMapFileLoader : public FileLoader {
public:
  HexMapFileLoader(const std::string &name) : FileLoader(name) {
    hexmap = NULL;
  }
  ~HexMapFileLoader() {}

  void setHexMap(HexMap *value) { hexmap = value; }

private:
  HexMap *hexmap;
  void parseLine(const std::string &field, const std::string &data);
};

#endif // HEXMAP_H
