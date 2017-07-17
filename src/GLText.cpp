#include "GLText.h"
#include <stdio.h>
void DrawTexture(int x, int y, GLuint textureid, int textw, int texth) {

  // int textw,texth;
  // tell opengl to use the generated texture name
  glBindTexture(GL_TEXTURE_2D, textureid);
  glEnable(GL_TEXTURE_2D);
  //
  // Blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // make a rectangle
  glBegin(GL_QUADS);
  //
  // top left
  glTexCoord2i(0, 0);
  glVertex3f(x, y, 0);
  //
  // top right
  glTexCoord2i(1, 0);
  glVertex3f(x + textw, y, 0);
  //
  // bottom right
  glTexCoord2i(1, 1);
  glVertex3f(x + textw, y + texth, 0);
  //
  // bottom left
  glTexCoord2i(0, 1);
  glVertex3f(x, y + texth, 0);
  //
  glEnd();
  //
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
}

GLuint LoadTexture(char *File, int *w, int *h) {
  GLuint textureid;
  int mode;

  SDL_Surface *Surf_Temp = IMG_Load(File);
  if (!Surf_Temp) {
    printf("Couldn't load image\n");
    return 0;
  }

  // work out what format to tell glTexImage2D to use...
  if (Surf_Temp->format->BytesPerPixel == 3) // RGB 24bit
  {

    mode = GL_RGB;

  } else if (Surf_Temp->format->BytesPerPixel == 4) // RGBA 32bit
  {
    mode = GL_RGBA;

  } else {

    SDL_FreeSurface(Surf_Temp);
    return 0;
  }

  *w = Surf_Temp->w;
  *h = Surf_Temp->h;

  glGenTextures(1, &textureid);
  glBindTexture(GL_TEXTURE_2D, textureid);
  glTexImage2D(GL_TEXTURE_2D, 0, mode, Surf_Temp->w, Surf_Temp->h, 0, mode,
               GL_UNSIGNED_BYTE, Surf_Temp->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  SDL_FreeSurface(Surf_Temp);

  return textureid;
}
