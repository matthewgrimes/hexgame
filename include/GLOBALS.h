#ifndef GLOBALS_H
#define GLOBALS_H

#include <cmath>

#define TILERADIUS 1
#define TILETHICKNESS (TILERADIUS / 2.0)
#define TILEHEIGHT 2 * TILERADIUS
#define TILEWIDTH (float)(sqrt(3.0) / 4 * TILEHEIGHT)
#define HORZDIST (float)TILEWIDTH
#define VERTDIST (float)(3.0 / 4.0 * TILEHEIGHT)

enum Directions {
  EAST,
  NORTHEAST,
  NORTHWEST,
  WEST,
  SOUTHWEST,
  SOUTHEAST,
  NONE
};
const char DirectionStrings[7][10] = {
    "EAST", "NORTHEAST", "NORTHWEST", "WEST", "SOUTHWEST", "SOUTHEAST", "NONE"};
const char AttackDirectionStrings[6][15] = {
    "REAR", "REAR FLANK", "FRONT FLANK", "FRONT", "FRONT FLANK", "REAR FLANK"};
enum DirectionsBitFlag {
  NONE_FLAG = 0,
  EAST_FLAG = 1 << EAST,
  NORTHEAST_FLAG = 1 << NORTHEAST,
  NORTHWEST_FLAG = 1 << NORTHWEST,
  WEST_FLAG = 1 << WEST,
  SOUTHWEST_FLAG = 1 << SOUTHWEST,
  SOUTHEAST_FLAG = 1 << SOUTHEAST
};
// opposite directions:
// EAST, NORTHEAST, NORTHWEST, WEST, SOUTHWEST, SOUTHEAST
//   0     1         2          3      4            5
// WEST SOUTHWEST SOUTHEAST EAST NORTHEAST, SOUTHWEST
//  3      4         5        0     1          2
#endif
