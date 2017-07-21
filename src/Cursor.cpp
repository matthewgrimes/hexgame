#include "Cursor.h"
#include "GLOBALS.h"
#include <math.h>
Cursor::Cursor() {
  // ctor
  m_AnimTimer.Start();
  Active = true;
  radius = 0;
}

Cursor::~Cursor() {
  // dtor
}

void Cursor::Update(float angle, int I, int J) {
  if (m_AnimTimer.getTicksElapsed() > 200) {
    frame_count += m_AnimTimer.getSecondsElapsed() * 1. / 60.;
    while (frame_count > 360) {
      frame_count -= 360.0;
    }
    m_AnimTimer.Reset();
  }
}

void Cursor::Draw() {
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  const float DEG2RAD = 3.14159 / 180;

  glBegin(GL_TRIANGLE_FAN);
  // glColor4f( 10 * 1.f/255.f, 100 * 1.f/255.f, 255 * 1.f/255.f, 150 *
  // 1.f/255.f);
  GLfloat color[] = {10 * 1.f / 255.f, 100 * 1.f / 255.f, 255 * 1.f / 255.f,
                     150 * 1.f / 255.f};
  glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
  for (int i = 0; i < 360; i += 60) {
    float degInRad = (i + 30 + frame_count) * DEG2RAD;

    glVertex3f(cos(degInRad) * 100 * cos(30 * DEG2RAD) + 0,
               sin(degInRad) * 100 * cos(30 * DEG2RAD) + 0,
               Position.getZ() + .1f);
  }

  glEnd();
  glDisable(GL_BLEND);
  // reset materials
  GLfloat defaultcolor[] = {0.8, 0.8, 0.8, 1.0};
  glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultcolor);
  glEnable(GL_DEPTH_TEST);
}
#include <stdio.h>
void Cursor::setPosition(HexCoordinate3D value) { Position = value; }
