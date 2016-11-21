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

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "ResourcePool.h"


#include <GL/glew.h>

class TexturePool : public ResourcePool<class Texture>
    {
    public:
        Texture* LoadResourceFromFile(const std::string& fileName);
    };

class Texture
{
    friend class TexturePool;
public:
    void Bind(GLenum TextureUnit);
    void LoadFromFile(const std::string& fileName);
    Texture();
protected:
    bool Init(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    SDL_Surface* mSurface;
};

#endif	/* TEXTURE_H */

