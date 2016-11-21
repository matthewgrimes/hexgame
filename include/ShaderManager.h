#pragma once
#include <string>
#include <map>
#include "PickingFBO.h"
#include "math_utils.h"

class ShaderManager
{
public:

    ~ShaderManager() {};

    void Cleanup();
    virtual void CompileShaders(const char* fileName, std::string name);
    virtual void CompileShaders(char const* vs, char const* gs, char const* fs, std::string name);

    void Activate(const std::string shaderName);
    void Disable();

    void ActivatePicking();
    void DisablePicking();

    void ClearBuffers();

    GLuint getProgram(const std::string shaderName);
    GLuint getProgram() const
    {
        return mCurrentProgram;
    }


    void initFBO(const unsigned int width, const unsigned int height);
    PickingFBO::PixelInfo readFBO(const unsigned int x, const unsigned int y);

    void setUniform(const Matrix4f& uniform, std::string name);
    void setUniform(const Vector3f& uniform, std::string name);
    void setUniform(const int& uniform, std::string name);
    void setUniform(const bool& uniform, std::string name);

    static ShaderManager* getInstance();

    ShaderManager() {};

    void incrementPickingObjectCount();
    void resetPickingObjectCount();
protected:
    void AddShader(GLuint ShaderProgram, std::string pShaderText, GLenum ShaderType);
    void Setup(GLuint& mProgram, const char* pVSFileName, const char* pGSFileName, const char* pFSFileName);

    std::map<std::string, GLuint> mShaderPrograms;
    GLuint mCurrentProgram;

    PickingFBO mPickingFBO;
    int mPickingObjectCount;

    static ShaderManager m_ShaderManager;
};

