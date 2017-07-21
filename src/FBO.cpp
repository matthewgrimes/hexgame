#include "FBO.h"
#include <stdio.h>

FBO::FBO() {
  m_fbo = 0;
  m_Texture = 0;
}

FBO::~FBO() {
  if (m_fbo != 0) {
    glDeleteFramebuffers(1, &m_fbo);
  }
  if (m_Texture != 0) {
    glDeleteTextures(1, &m_Texture);
  }
}

bool FBO::Init(const unsigned int winWidth, const unsigned int winHeight) {

  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  setupTextures(winWidth, winHeight);

  glReadBuffer(GL_NONE);
  glDrawBuffer(GL_NONE);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE) {
    printf("FB error, status: 0x%x\n", status);
    return false;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return glGetError();
}

void FBO::setupTextures(unsigned int winWidth, unsigned int winHeight) {
  glGenTextures(1, &m_Texture);
  glBindTexture(GL_TEXTURE_2D, m_Texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, winWidth, winHeight, 0, GL_RGB,
               GL_FLOAT, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D, m_Texture, 0);
}

void FBO::EnableWriting() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void FBO::DisableWriting() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); }

void FBO::BindTexture(GLenum TextureUnit) {
  glActiveTexture(TextureUnit);
  glBindTexture(GL_TEXTURE_2D, m_Texture);
}
