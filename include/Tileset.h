#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <vector>

#include "FileLoader.h"
#include "HexTileMesh.h"
#include "HexTileTexture.h"

class Tileset {
  friend class TilesetLoader;

public:
  /** Default constructor */
  Tileset();
  /** Default destructor */
  virtual ~Tileset();
  /** Method for accessing members of the tileset */
  HexTile *getMember(int index);
  HexTileMesh *getAuxTile(const unsigned int index);
  std::string getAuxTileTexture(const unsigned int &index);
  Mesh *getObstacle(int index);
  Mesh *getDecoration(int index);
  void Init();

  bool loadFromFile(const std::string &fileName);

  int getSize() { return currentTileSet.size(); }

protected:
private:
  std::vector<HexTile *> currentTileSet;
  std::vector<HexTileMesh *> auxTiles;
  std::vector<std::string> auxTileTextures;
  std::vector<Mesh *> obstacleSet;
  std::vector<Mesh *> decorationSet;
};

class TilesetLoader : public FileLoader {
public:
  TilesetLoader(const std::string &name);
  ~TilesetLoader();

  void setTileset(Tileset *value) { tileset = value; }

protected:
  Tileset *tileset;
  void parseLine(const std::string &field, const std::string &data);
};

#endif // TILESET_H
