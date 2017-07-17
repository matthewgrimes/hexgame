#include "HexTileMesh.h"
#include "GLOBALS.h"
#include "ResourceManager.h"
HexTileMesh::HexTileMesh() {
  // ctor
}

HexTileMesh::~HexTileMesh() {
  // dtor
}

void HexTileMesh::Draw() { Render(); }

void HexTileMesh::drawGrid() {}

void HexTileMesh::LoadNormal(const std::string &fileName) {
  normalMap = fileName;
  ResourceManager::getSingleton()->addTexture(normalMap);
}
