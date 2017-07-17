#include "HexTileTexture.h"
#include <stdio.h>
bool HexTileTexture::Init(char *fileName) {
  m_textureID = LoadTexture(fileName, &textureW, &textureH);

  // set color to white for masking
  return true;
}
HexTileTexture::HexTileTexture() {}
HexTileTexture::~HexTileTexture() { glDeleteTextures(1, &m_textureID); }

void HexTileTexture::Draw() {
  static float tileHeight = 2 * getRadius();
  static float tileWidth = sqrt(3.0) / 4 * tileHeight;

  glBindTexture(GL_TEXTURE_2D, m_textureID);
  glEnable(GL_TEXTURE_2D);

  glEnable(GL_STENCIL_TEST);

  HexTile::DrawMask();

  glBegin(GL_QUADS);
  glColor4f(1.f, 1.f, 1.f, 1.f);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-tileWidth, tileHeight / 2.f, 0.0f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(tileWidth, tileHeight / 2.f, 0.0f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(tileWidth, -tileHeight / 2.f, 0.0f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-tileWidth, -tileHeight / 2.f, 0.0f);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_STENCIL_TEST);
  // glDisable(GL_DEPTH_TEST);
  // glBlendFunc(GL_ONE, GL_ONE );

  /*glBegin(GL_TRIANGLE_FAN);
  glColor4f( Color.r * 1.f/255.f, Color.g * 1.f/255.f, 1.f, 1.f);
  for (int i=0; i <360; i+=60)
  {
      float degInRad = (i+30)*3.1415926535/180;
      glVertex3f(cos(degInRad)*Radius*.7,sin(degInRad)*Radius*.7, 140.f);
  }
  glEnd();*/

  // glEnable(GL_DEPTH_TEST);
}
