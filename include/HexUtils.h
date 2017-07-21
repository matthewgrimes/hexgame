#ifndef HEXUTILS_H
#define HEXUTILS_H

#include "GLOBALS.h"

/** utilities generic enough to not need any map data
 * only includes methods for working in hex coordinates, etc */

int NeighborI(int i, int j, Directions D);
int NeighborJ(int i, int j, Directions D);

//! get distance between two points
int getDistance(int i1, int j1, int i2, int j2);

//! rotate (i,j) about the origin
void Rotate(int *i, int *j, bool clockwise = true);

//! get direction between two points
Directions getDirection(int i1, int j1, int i2, int j2);
int getAngle(int i1, int j1, int i2, int j2);

class HexCoordinate2D {
public:
  HexCoordinate2D();
  HexCoordinate2D(int i, int j);
  HexCoordinate2D(float x, float y);
  ~HexCoordinate2D();

  int getI() const;
  int getJ() const;

  float getX() const;
  float getY() const;

  void setI(int value);
  void setJ(int value);

  void setX(float value);
  void setY(float value);

  bool operator==(const HexCoordinate2D &r) {
    return (I == r.getI()) && (J == r.getJ());
  }

private:
  int I, J;
  float X, Y;

  void IJtoXY();
  void XYtoIJ();
};

class HexCoordinate3D : public HexCoordinate2D {
public:
  HexCoordinate3D();
  HexCoordinate3D(int i, int j, int level);
  HexCoordinate3D(float x, float y, float z);
  ~HexCoordinate3D();

  float getZ();
  void setZ(float value);

  int getLevel();
  void setLevel(int value);

private:
  float Z;
  int Level;
};

HexCoordinate2D Neighbor(int i, int j, Directions D);

// HexCoordinate3D convertScreenPositionToScene(float angle, int centerI, int
// centerJ, int screenX, int screenY, int Z = 0);

class HexMapIterator {
public:
  HexMapIterator(int isize, int jsize);
  ~HexMapIterator() {}

  void Reset(int &i, int &j);
  bool Iterate(int &i, int &j);
  int getCount();

private:
  int Isize, Jsize;
  int Count;
};

#endif
