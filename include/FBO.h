#ifndef FBO_H
#define FBO_H

#include <GL/glew.h>

class FBO {
public:
  FBO();
  ~FBO();

  bool Init(const unsigned int winWidth, const unsigned int winHeight);

  void EnableWriting();
  void DisableWriting();

  void BindTexture(GLenum TextureUnit);

protected:
  virtual void setupTextures(unsigned int winWidth, unsigned int winHeight);
  GLuint m_fbo;
  GLuint m_Texture;
};

#endif
