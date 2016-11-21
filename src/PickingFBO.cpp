#include "PickingFBO.h"

PickingFBO::PickingFBO()
{
    m_depthTexture = 0;
}

PickingFBO::~PickingFBO()
{
    if (m_depthTexture != 0)
    {
        glDeleteTextures(1, &m_depthTexture);
    }
}

PickingFBO::PixelInfo PickingFBO::readPixel(const unsigned int x, const unsigned int y)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    PixelInfo Pixel;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &Pixel);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return Pixel;
}

void PickingFBO::setupTextures(unsigned int winWidth,unsigned int winHeight)
{
    FBO::setupTextures(winWidth,winHeight);

    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, winWidth, winHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

}
