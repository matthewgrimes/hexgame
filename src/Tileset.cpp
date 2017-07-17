#include "Tileset.h"
#include "ResourceManager.h"
#include <fstream>

TilesetLoader::TilesetLoader(const std::string &name) : FileLoader(name) {
  tileset = NULL;
}
TilesetLoader::~TilesetLoader() {}

void TilesetLoader::parseLine(const std::string &field,
                              const std::string &data) {
  std::size_t spacemarker;
  std::string mesh, normal, texture;

  if (field == "obstacle") {
    spacemarker = data.find_first_of(" ");

    mesh = data.substr(0, spacemarker);
    texture = data.substr(spacemarker + 1);

    tileset->obstacleSet.push_back(new Mesh);
    tileset->obstacleSet.back()->LoadMesh(mesh.c_str());
    tileset->obstacleSet.back()->setNormalMap(texture.c_str());
  } else if (field == "decoration") {
    spacemarker = data.find_first_of(" ");

    mesh = data.substr(0, spacemarker);
    texture = data.substr(spacemarker + 1);

    tileset->decorationSet.push_back(new Mesh);
    tileset->decorationSet.back()->LoadMesh(mesh.c_str(), texture.c_str());
  } else {
    mesh = field;
    spacemarker = data.find_first_of(" ");
    normal = data.substr(0, spacemarker);

    tileset->currentTileSet.push_back(new HexTileMesh);
    tileset->currentTileSet.back()->setColor(255, 0, 0);
    static_cast<HexTileMesh *>(tileset->currentTileSet.back())
        ->LoadMesh(mesh.c_str());
    static_cast<HexTileMesh *>(tileset->currentTileSet.back())
        ->LoadNormal(normal.c_str());

    tileset->currentTileSet.back()->setImpassable(
        atoi(data.substr(spacemarker + 1).c_str()));
  }
}

Tileset::Tileset() {
  // ctor
}

Tileset::~Tileset() {
  // dtor
  for (auto tile : currentTileSet) {
    delete tile;
  }
  for (auto tile : auxTiles) {
    delete tile;
  }
}

void Tileset::Init() {
  currentTileSet.push_back(new HexTile);
  currentTileSet.back()->setColor(0, 0, 0);

  obstacleSet.push_back(NULL);
  decorationSet.push_back(NULL);

  HexTileMesh *whiteTile = new HexTileMesh;
  HexTileMesh *blueTile = new HexTileMesh;
  HexTileMesh *redTile = new HexTileMesh;
  HexTileMesh *greenTile = new HexTileMesh;

  auxTileTextures = {"data/tiletextures/blue.png", "data/tiletextures/red.png",
                     "data/tiletextures/green.png",
                     "data/tiletextures/white.png"};

  blueTile->LoadMesh("data/tiletextures/hextile-thick-template.obj");
  blueTile->setTexture(auxTileTextures[0]);
  auxTiles.push_back(blueTile);

  redTile->LoadMesh("data/tiletextures/hextile-thick-template.obj");
  redTile->setTexture(auxTileTextures[1]);
  auxTiles.push_back(redTile);

  greenTile->LoadMesh("data/tiletextures/hextile-thick-template.obj");
  greenTile->setTexture(auxTileTextures[2]);
  auxTiles.push_back(greenTile);

  whiteTile->LoadMesh("data/tiletextures/hextile-thick-template.obj");
  whiteTile->setTexture(auxTileTextures[3]);
  auxTiles.push_back(whiteTile);
}

HexTile *Tileset::getMember(int index) {
  // check that a valid member is requested
  if (index < 0 || index > (int)currentTileSet.size()) {
    return NULL;
  }
  return currentTileSet[index];
}

HexTileMesh *Tileset::getAuxTile(const unsigned int index) {
  if (index < 0 || index > auxTiles.size()) {
    return NULL;
  }
  return auxTiles[index];
}

bool Tileset::loadFromFile(const std::string &fileName) {
  TilesetLoader loader(fileName);
  loader.setTileset(this);
  loader.parseFile();
  return 1;
}

Mesh *Tileset::getObstacle(int index) { return obstacleSet[index]; }

Mesh *Tileset::getDecoration(int index) { return decorationSet[index]; }

std::string Tileset::getAuxTileTexture(const unsigned int &index) {
  return auxTileTextures[index];
}
