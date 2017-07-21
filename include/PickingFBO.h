#ifndef PICKINGFBO_H
#define PICKINGFBO_H

#include "FBO.h"

class PickingFBO : public FBO {
public:
  PickingFBO();
  ~PickingFBO();

  struct PixelInfo {
    float gI;
    float gJ;
    float gLayer;
    PixelInfo() {
      gI = 0.0f;
      gJ = 0.0f;
      gLayer = 0.0f;
    }
  };

  PixelInfo readPixel(const unsigned int x, const unsigned int y);

private:
  void setupTextures(unsigned int winWidth, unsigned int winHeight);
  GLuint m_depthTexture;
};

#endif
