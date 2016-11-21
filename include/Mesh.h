#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "math_utils.h"

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;
    Vector3f m_tangent;

    Vertex() {}

    Vertex(const Vector3f& pos,
           const Vector2f& tex,
           const Vector3f& normal,
           const Vector3f& tangent) : m_pos(pos), m_tex(tex), m_normal(normal), m_tangent(tangent)
    {
    }
};

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

    virtual bool LoadMesh(const std::string& Filename);
    virtual void LoadFromFile(const std::string& fileName);
    virtual bool LoadMesh(const std::string& Filename, const std::string& textureName);

    virtual void Render(bool withOutline = false);
    virtual bool RenderWithOutline(GLfloat red, GLfloat green, GLfloat blue, GLfloat thickness);

    bool setTexture(const std::string& Filename);
    bool isTextureOverridden();
    void clearOverrideTexture();

    void setNormalMap(const std::string& Filename);

    const Vector3f getMinPos()
    {
        return minPos;
    }
    const Vector3f getMaxPos()
    {
        return maxPos;
    }
    const float getHeight()
    {
        return maxPos.y - minPos.y;
    }
protected:
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(const aiMesh* paiMesh, std::vector<Vector3f>& Positions,
                  std::vector<Vector3f>& Normals,
                  std::vector<Vector2f>& TexCoord,
                  std::vector<unsigned int>& Indices,
                  std::vector<Vector3f>& Tangents);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();
    enum VB_TYPES
    {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        TANGENT_VB,
        BONE_VB,
        NUM_VBs
    };

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

#define INVALID_MATERIAL 0xFFFFFFFF
    struct MeshEntry
    {
        MeshEntry()
        {
            baseIndex = 0;
            baseVertex = 0;
            NumIndices = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        ~MeshEntry();

        unsigned int baseIndex;
        unsigned int baseVertex;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<std::string> m_Textures;

    std::string overrideTexture;
    std::string normalMap;
    bool hasNormalMap();

    /*Assimp::Importer m_Importer;
    const aiScene* m_pScene;
    Matrix4f m_GlobalInverseTransform;*/
    Vector3f minPos;
    Vector3f maxPos;
    void adjustPositionBounds(const Vector3f& newPos);
};

#endif
