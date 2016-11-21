/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "Texture.h"

Texture::Texture()
{

}

void Texture::LoadFromFile(const std::string& fileName)
{
    Init(GL_TEXTURE_2D, fileName);
    Load();
}

bool Texture::Init(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
    mSurface = NULL;
    return true;
}

bool Texture::Load()
{
    glEnable(GL_TEXTURE_2D);
    mSurface = IMG_Load(m_fileName.c_str());

    if (mSurface == NULL)
    {
        printf("Surface is NULL; error loading texture: %s\n",m_fileName.c_str());
        return false;
    }

    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);

    int Mode = GL_RGB;
    if (mSurface->format->BytesPerPixel==4)
    {
        Mode = GL_RGBA;
    }

    glTexImage2D(m_textureTarget, 0, Mode, mSurface->w, mSurface->h, 0, Mode, GL_UNSIGNED_BYTE, mSurface->pixels);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(mSurface);
    glDisable(GL_TEXTURE_2D);
    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}

Texture* TexturePool::LoadResourceFromFile(const std::string& fileName)
{
    addResourceFromFile(fileName,fileName);
    return getResource(fileName);
}
