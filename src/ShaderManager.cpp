#include "ShaderManager.h"
/*

Copyright 2010 Etay Meiri

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

Tutorial 04 - shaders
*/

#include <stdio.h>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include "ShaderManager.h"
#include "math_utils.h"
#include "ServiceLocator.h"

#define OGLDEV_FILE_ERROR(FileError) OgldevFileError(__FILE__, __LINE__, FileError);

void OgldevFileError(const char* pFileName, unsigned int line, std::string pFileError)
{
#ifdef WIN32
    char msg[1000];
    snprintf(msg, sizeof(msg), "%s:%d: unable to open file `%s`", pFileName, line, pFileError.c_str());
    //MessageBoxA(NULL, msg, NULL, 0);
#else
    fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError.c_str());
#endif
}

bool ReadFile(const char* pFileName, std::string& outFile)
{
    std::ifstream f(pFileName);

    bool ret = false;

    if (f.is_open())
    {
        std::string line;
        while (getline(f, line))
        {
            outFile.append(line);
            outFile.append("\n");
        }

        f.close();

        ret = true;
    }
    else
    {
        OGLDEV_FILE_ERROR(pFileName);
    }

    return ret;
}

ShaderManager ShaderManager::m_ShaderManager;

void ShaderManager::AddShader(GLuint ShaderProgram, std::string pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0)
    {
        printf("Error creating shader type %d\n", ShaderType);
        //exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText.c_str();
    GLint Lengths[1];
    Lengths[0] = pShaderText.length();
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        printf("Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        //exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

void ShaderManager::ActivatePicking()
{
    Activate("picking");
    mPickingFBO.EnableWriting();
}
void ShaderManager::DisablePicking()
{
    mPickingFBO.DisableWriting();
}

void ShaderManager::CompileShaders(char const* fileName, std::string name)
{
    std::string vsFileName, gsFileName, fsFileName;
    vsFileName = std::string(fileName)+".vs";
    gsFileName = std::string(fileName)+".gs";
    fsFileName = std::string(fileName)+".fs";
    CompileShaders(vsFileName.c_str(),gsFileName.c_str(),fsFileName.c_str(),name);
}

void ShaderManager::CompileShaders(char const* vsFileName, char const* gsFileName, char const* fsFileName, std::string shaderName)
{
    Setup(mShaderPrograms[shaderName], vsFileName, gsFileName, fsFileName);
}

void ShaderManager::Setup(GLuint& mProgram, const char* pVSFileName, const char* pGSFileName, const char* pFSFileName)
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0)
    {
        printf("Error creating shader program\n");
        //exit(1);
    }

    std::string vs, gs, fs;

    if (!ReadFile(pVSFileName, vs))
    {
        //exit(1);
    };

    if (!ReadFile(pFSFileName, fs))
    {
        //exit(1);
    };

    ReadFile(pGSFileName, gs);

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, gs.c_str(), GL_GEOMETRY_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0)
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        printf("Error linking shader program: '%s'\n", ErrorLog);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        printf("Invalid shader program: '%s'\n", ErrorLog);
    }
    mProgram = ShaderProgram;
}

void ShaderManager::Activate(std::string shaderName)
{
    glUseProgram(mShaderPrograms[shaderName]);
    mCurrentProgram = mShaderPrograms[shaderName];
}

void ShaderManager::Disable()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    mCurrentProgram = 0;
}

void ShaderManager::Cleanup()
{
    for (const auto& program : mShaderPrograms)
    {
        glDeleteProgram(program.second);
    }
}

GLuint ShaderManager::getProgram(const std::string shaderName)
{
    return mShaderPrograms[shaderName];
}

void ShaderManager::initFBO(const unsigned int width, const unsigned int height)
{
    GLenum error = mPickingFBO.Init(width, height);
    if (error != GL_NO_ERROR)
    {
        printf("Error initializing FBO\n");
    }
}

PickingFBO::PixelInfo ShaderManager::readFBO(const unsigned int x, const unsigned int y)
{
    return mPickingFBO.readPixel(x, y);
}
void ShaderManager::ClearBuffers()
{
    /* Set the background black */
    glClearColor(0.5f, .5f, .5f, 0.0f);
    /* Clear The Screen And The Depth Buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mPickingFBO.EnableWriting();
    glClearColor(0.f, 0.f, 0.f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mPickingFBO.DisableWriting();
}


void ShaderManager::setUniform(const Matrix4f& uniform, std::string name)
{
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), name.c_str()), 1, GL_TRUE, (const GLfloat*)uniform);
}
void ShaderManager::setUniform(const Vector3f& uniform, std::string name)
{
    glUniform3f(glGetUniformLocation(getProgram(), name.c_str()), uniform.x, uniform.y, uniform.z);
}
void ShaderManager::setUniform(const int& uniform, std::string name)
{
    glUniform1i(glGetUniformLocation(getProgram(), name.c_str()), uniform);
}

void ShaderManager::setUniform(const bool& uniform, std::string name)
{
    glUniform1i(glGetUniformLocation(getProgram(), name.c_str()),uniform);
}

ShaderManager* ShaderManager::getInstance()
{
    return ServiceLocator::getMainDisplay().getShaderManager();
}

void ShaderManager::incrementPickingObjectCount()
{
    mPickingObjectCount++;
    setUniform(mPickingObjectCount,"gObjectCount");
}

void ShaderManager::resetPickingObjectCount()
{
    mPickingObjectCount = 0;
    setUniform(mPickingObjectCount,"gObjectCount");
}
