#ifndef HEXTILEMESH_H
#define HEXTILEMESH_H

#include "HexTile.h"
#include "Mesh.h"
#include <string>

class HexTileMesh : public HexTile, public Mesh {
public:
  /** Default constructor */
  HexTileMesh();
  /** Default destructor */
  virtual ~HexTileMesh();

  void Draw();

  virtual void drawGrid();
  void LoadNormal(const std::string &fileName);

protected:
private:
};

#endif // HEXTILEMESH_H
