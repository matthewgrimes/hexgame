#include "HexTile.h"
#include "GLOBALS.h"

HexTile::HexTile() {
  // ctor
  Radius = TILERADIUS;

  gridColor.r = 0;
  gridColor.g = 0;
  gridColor.b = 0;
  gridColor.a = 155;

  gridScale = .95;

  gridOutline = false;

  impassable = false;
}

HexTile::~HexTile() {
  // dtor
}
void HexTile::setColor(int r, int g, int b, int a) {
  Color.r = r;
  Color.g = g;
  Color.b = b;
  Color.a = a;
}
void HexTile::Draw() {
  const float DEG2RAD = 3.14159 / 180;

  glBegin(GL_TRIANGLE_FAN);
  glColor4f(Color.r * 1.f / 255.f, Color.g * 1.f / 255.f, Color.b * 1.f / 255.f,
            Color.a * 1.f / 255.f);
  for (int i = 0; i < 360; i += 60) {
    float degInRad = (i + 30) * DEG2RAD;
    glVertex3f(cos(degInRad) * Radius, sin(degInRad) * Radius, 0.f);
  }

  glEnd();
}

void HexTile::drawGrid() {
  /*    const float DEG2RAD = 3.14159/180;

      glEnable(GL_BLEND); //Enable blending.
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending
     function.
      glEnable(GL_COLOR_MATERIAL);

      // draw thick gridlines
      glBegin(GL_QUADS);
      glColor4f( gridColor.r * 1.f/255.f, gridColor.g * 1.f/255.f, gridColor.b *
     1.f/255.f, gridColor.a * 1.f/255.f);
      for (int i=0; i <360; i+=60)
      {
          float degInRad = (i+30)*DEG2RAD;
          float degInRad2 =(i + 90)*DEG2RAD;
          glVertex3f(cos(degInRad)*Radius,sin(degInRad)*Radius,0.1f);
          glVertex3f(cos(degInRad)*Radius*gridScale,sin(degInRad)*Radius*gridScale,0.1f);
          glVertex3f(cos(degInRad2)*Radius*gridScale,sin(degInRad2)*Radius*gridScale,0.1f);
          glVertex3f(cos(degInRad2)*Radius,sin(degInRad2)*Radius,0.1f);
      }

      glEnd();
      glDisable(GL_STENCIL_TEST);
      glDisable(GL_BLEND);
      glColor4f(1.f,1.f,1.f,1.f);
      glDisable(GL_COLOR_MATERIAL);*/
}

void HexTile::DrawMask(int Reverse) {
  const float DEG2RAD = 3.14159 / 180;

  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);
  glStencilFunc(GL_NEVER, 1, 0xFF);
  glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)

  // draw stencil pattern
  glStencilMask(0xFF);
  glClear(GL_STENCIL_BUFFER_BIT); // needs mask=0xFF

  glBegin(GL_TRIANGLE_FAN);
  glColor4f(Color.r * 1.f / 255.f, Color.g * 1.f / 255.f, Color.b * 1.f / 255.f,
            Color.a * 1.f / 255.f);
  for (int i = 0; i < 360; i += 60) {
    float degInRad = (i + 30) * DEG2RAD;
    glVertex3f(cos(degInRad) * Radius, sin(degInRad) * Radius, 0.f);
  }

  glEnd();

  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDepthMask(GL_TRUE);
  glStencilMask(0x00);
  // draw where stencil's value is 0
  glStencilFunc(GL_EQUAL, 1 - Reverse, 0xFF);
  /* (nothing to draw) */
  // draw only where stencil's value is 1
  // glStencilFunc(GL_EQUAL, 1, 0xFF);
}
