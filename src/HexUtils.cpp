#include "HexUtils.h"
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <math.h>

int NeighborI(int i, int j, Directions D) {
  if (D == EAST || D == NORTHEAST) {
    return i + 1;
  } else if (D == NORTHWEST || D == SOUTHEAST) {
    return i;
  } else if (D == SOUTHWEST || D == WEST) {
    return i - 1;
  }
  return i;
}

int NeighborJ(int i, int j, Directions D) {
  if (D == SOUTHEAST || D == SOUTHWEST) {
    return j + 1;
  } else if (D == WEST || D == EAST) {
    return j;
  } else if (D == NORTHWEST || D == NORTHEAST) {
    return j - 1;
  }
  return j;
}

void Rotate(int *i, int *j, bool clockwise) {
  int tempi, tempj, k;
  k = -(*i) - (*j);
  if (!clockwise) {
    tempi = -k;
    tempj = -(*i);
    *i = tempi;
    *j = tempj;
  } else {
    tempi = -(*j);
    tempj = -k;
    *i = tempi;
    *j = tempj;
  }
}
int getDistance(int i1, int j1, int i2, int j2) {
  return (std::abs(i1 - i2) + std::abs(j1 - j2) + std::abs(i1 + j1 - i2 - j2)) /
         2;
}

int getAngle(int i1, int j1, int i2, int j2) {
  return 60 * (int)getDirection(i1, j1, i2, j2);
}

Directions getDirection(int i1, int j1, int i2, int j2) {
  int tempI = i2 - i1;
  int tempJ = j2 - j1;

  if (tempI >= -2 * tempJ && tempI >= tempJ) {
    return EAST;
  } else if (tempI <= -2 * tempJ && tempI <= tempJ) {
    return WEST;
  } else if (tempI > tempJ && -2 * tempI >= tempJ) {
    return NORTHWEST;
  } else if (tempI < -2 * tempJ && -2 * tempI < tempJ) {
    return NORTHEAST;
  } else if (tempI > -2 * tempJ && -2 * tempI >= tempJ) {
    return SOUTHWEST;
  } else if (-2 * tempI < tempJ && tempI < tempJ) {
    return SOUTHEAST;
  }
  return NONE;
}

int HexCoordinate2D::getI() const { return I; }
int HexCoordinate2D::getJ() const { return J; }
float HexCoordinate2D::getX() const { return X; }
float HexCoordinate2D::getY() const { return Y; }

HexCoordinate2D::HexCoordinate2D() {}

HexCoordinate2D::HexCoordinate2D(int i, int j) {
  I = i;
  J = j;
  IJtoXY();
}

HexCoordinate2D::HexCoordinate2D(float x, float y) {
  X = x;
  Y = y;
  XYtoIJ();
}

HexCoordinate2D::~HexCoordinate2D() {}

void HexCoordinate2D::setI(int value) {
  I = value;
  IJtoXY();
}

void HexCoordinate2D::setJ(int value) {
  J = value;
  IJtoXY();
}

void HexCoordinate2D::setX(float value) {
  X = value;
  XYtoIJ();
}

void HexCoordinate2D::setY(float value) {
  Y = value;
  XYtoIJ();
}

void HexCoordinate2D::XYtoIJ() {
  // set hex coords
  double q = (1. / 3. * sqrt(3.) * X - 1. / 3. * Y) * 1. / TILERADIUS;
  double r = 2. / 3. * Y * 1. / TILERADIUS;

  // initial batch of rounding
  I = floor(q + .5);
  // r + J + I = 0 so J = -I-r
  J = floor(r + .5);

  // now round to nearest hex:
  double dI = fabs(I - q);
  double dJ = fabs(J - r);
  double K = -q - r;
  double dK = fabs(K - floor(K + .5));
  if (dI > dJ && dI > dK) {
    I = (int)(-floor(K + .5) - J);
  } else if (dJ > dK) {
    J = (int)(-floor(K + .5) - I);
  }
}

void HexCoordinate2D::IJtoXY() {
  X = TILERADIUS * sqrt(3.0) * (I * 1.0 + J / 2.0);
  Y = TILERADIUS * 3.0 / 2.0 * J;
}

HexCoordinate3D::HexCoordinate3D() {}

HexCoordinate3D::HexCoordinate3D(float x, float y, float z)
    : HexCoordinate2D(x, y) {
  Z = z;
  Level = Z / TILETHICKNESS;
}

HexCoordinate3D::HexCoordinate3D(int i, int j, int level)
    : HexCoordinate2D(i, j) {
  Level = level;
  Z = Level * TILETHICKNESS;
}

void HexCoordinate3D::setZ(float value) {
  Z = value;
  Level = Z / TILETHICKNESS;
}

void HexCoordinate3D::setLevel(int value) {
  Level = value;
  Z = Level * TILETHICKNESS;
}

int HexCoordinate3D::getLevel() { return Level; }

float HexCoordinate3D::getZ() { return Z; }

HexCoordinate3D::~HexCoordinate3D() {}

HexCoordinate2D Neighbor(int i, int j, Directions D) {
  HexCoordinate2D neighbor(NeighborI(i, j, D), NeighborJ(i, j, D));
  return neighbor;
}

/*
HexCoordinate3D convertScreenPositionToScene(float angle, int centerI, int
centerJ, int windowX, int windowY, int Z)
{

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble sceneX, sceneY, sceneZ;


    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)windowX;
    winY = (float)viewport[3]-(float)windowY;

    glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,
&winZ);

    gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, modelview,
projection, viewport, &sceneX, &sceneY, &sceneZ);
    GLdouble sceneX2, sceneY2, sceneZ2;
    gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)10., modelview,
projection, viewport, &sceneX2, &sceneY2, &sceneZ2);

    // the scene coordinates now need to be projected into the z=0 plane
    // the line z = sceneZ + t(sceneZ - sceneZ2) gives
    // z = 0 when t = -sceneZ/(sceneZ - sceneZ2)
    // z = winZ when t = ( winZ - sceneZ ) / ( sceneZ - sceneZ2 )

    //
    // so we let
    double t = (Z - sceneZ )/ ( sceneZ - sceneZ2 );
    sceneZ = sceneZ + t * (sceneZ - sceneZ2);
    sceneX = sceneX + t * (sceneX - sceneX2);
    sceneY = sceneY + t * (sceneY - sceneY2);
    //
    //
    // now we have are ready to calculate coordinates if the map weren't
rotated.
    // we need to rotate sceneX and sceneY about I,J by -angle

    // first, translate
    sceneX -=centerI;
    sceneY -= centerJ;
    // now, rotate
    // being certain to convert angle to radians
    angle *= 3.1415926535 / 180;
    float tempsceneX = cos(-angle)*sceneX - sin(-angle)*sceneY;
    float tempsceneY = sin(-angle)*sceneX + cos(-angle)*sceneY;
    // now reset and translate back
    sceneX = tempsceneX + centerI;
    sceneY = tempsceneY + centerJ;
    HexCoordinate3D position((float)sceneX,(float)sceneY,(float)sceneZ);
    return position;
}
*/
HexMapIterator::HexMapIterator(int isize, int jsize) {
  Isize = isize;
  Jsize = jsize;
  Count = 0;
}

void HexMapIterator::Reset(int &i, int &j) {
  j = 0;
  i = -(j - j % 2) / 2 - 1;
  Count = 0;
}

int HexMapIterator::getCount() { return Count; }

bool HexMapIterator::Iterate(int &i, int &j) {
  bool proceed = false;
  if (i < Isize - (j - j % 2) / 2 - 1) {
    i++;
    proceed = true;
  } else {
    j++;
    i = -(j - j % 2) / 2;
    j < Jsize - 1 ? proceed = true : proceed = false;
  }
  // printf("Count: %d - Calculated: %d\n", Count, j*Isize + i + (j - j%2)/2);
  Count++;
  return proceed;
}
