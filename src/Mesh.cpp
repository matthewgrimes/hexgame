#include "Mesh.h"
#include "Pipeline.h"
#include "ResourceManager.h"

//#include <assert.h>
#define INVALID_OGL_VALUE 0xffffffff
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

Mesh::Mesh()
{
    maxPos = Vector3f(0,0,0);
    minPos = Vector3f(0,0,0);
    overrideTexture = "";
    normalMap = "";
}

Mesh::~Mesh()
{
    Clear();
}

bool Mesh::RenderWithOutline(GLfloat red, GLfloat green, GLfloat blue, GLfloat thickness)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    Render();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);
    glDepthMask(GL_TRUE);
    glDisable(GL_LIGHTING);
    Render(1);
    glPopAttrib();

    return true;
}

Mesh::MeshEntry::~MeshEntry()
{
}

void Mesh::Clear()
{
    clearOverrideTexture();
    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO=0;
    }
}

bool Mesh::LoadMesh(const std::string& Filename, const std::string& textureName)
{
    return LoadMesh(Filename) && setTexture(textureName);
}

void Mesh::LoadFromFile(const std::string& fileName)
{
    LoadMesh(fileName);
}

bool Mesh::LoadMesh(const std::string& Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();

    bool Ret = false;

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(sizeof(m_Buffers)/sizeof(m_Buffers[0]), m_Buffers);

    Assimp::Importer m_Importer;
    const aiScene* m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (m_pScene)
    {
        Ret = InitFromScene(m_pScene, Filename);
    }
    else
    {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
    }

    glBindVertexArray(0);
    return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);


    std::vector<Vector3f> Positions;
    std::vector<Vector3f> Normals;
    std::vector<Vector3f> Tangents;
    std::vector<Vector2f> TexCoords;
    std::vector<unsigned int> Indices;

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    for (unsigned int i = 0; i < m_Entries.size(); i++)
    {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].baseVertex = NumVertices;
        m_Entries[i].baseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices += m_Entries[i].NumIndices;
    }

    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Tangents.reserve(NumVertices);
    Indices.reserve(NumVertices);


    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_Entries.size(); i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(paiMesh, Positions, Normals, TexCoords, Indices, Tangents);
    }
    if (!InitMaterials(pScene, Filename))
    {
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[TANGENT_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Tangents[0]) * Tangents.size(), &Tangents[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0],
                 GL_STATIC_DRAW);


    return true;
}

void Mesh::adjustPositionBounds(const Vector3f& newPos)
{
    if ( newPos.x > maxPos.x )
    {
        maxPos.x = newPos.x;
    }
    if ( newPos.y > maxPos.y )
    {
        maxPos.y = newPos.y;
    }
    if ( newPos.z > maxPos.z )
    {
        maxPos.z = newPos.z;
    }

    if ( newPos.x < minPos.x )
    {
        minPos.x = newPos.x;
    }
    if ( newPos.y < minPos.y )
    {
        minPos.y = newPos.y;
    }
    if ( newPos.z < minPos.z )
    {
        minPos.z = newPos.z;
    }

}

void Mesh::InitMesh(const aiMesh* paiMesh, std::vector<Vector3f>& Positions,
                    std::vector<Vector3f>& Normals,
                    std::vector<Vector2f>& TexCoord,
                    std::vector<unsigned int>& Indices,
                    std::vector<Vector3f>& Tangents)
{
    if (!paiMesh)
    {
        printf("Error: paiMesh is NULL in InitMesh!\n");
        return;
    }

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
    {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        const aiVector3D* pTangent = &(paiMesh->mTangents[i]);

        Positions.push_back(Vector3f(Vector3f(pPos->x, pPos->y, pPos->z)));
        adjustPositionBounds(Positions.back());

        TexCoord.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
        Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
        Tangents.push_back(Vector3f(pTangent->x, pTangent->y, pTangent->z));
    }

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
    {
        const aiFace& Face = paiMesh->mFaces[i];
        if (Face.mNumIndices != 3)
        {
            printf("Number of faces ought to be 3...\n");
        }
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos)
    {
        Dir = ".";
    }
    else if (SlashIndex == 0)
    {
        Dir = "/";
    }
    else
    {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = "";

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                std::string FullPath = Dir + "/" + Path.data;
                //m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath);
                //m_Textures[i]->Load();
                ResourceManager::getSingleton()->addTexture(FullPath);
                m_Textures[i] = FullPath;
            }
        }

        // Load a white texture in case the model does not include its own texture
        if (m_Textures[i]=="")
        {
            ResourceManager::getSingleton()->addTexture("./data/tiletextures/white.png");
            m_Textures[i] = std::string("./data/tiletextures/white.png");
            Ret = true;

            //Ret = m_Textures[i]->Load();
        }
    }

    return Ret;
}

void Mesh::Render(bool withOutline)
{
    ShaderManager::getInstance()->incrementPickingObjectCount();
    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"gWVP"), 1, GL_TRUE, (const GLfloat*)Pipeline::getInstance()->GetWVPTrans());
    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"gObject"), 1, GL_TRUE, (const GLfloat*)Pipeline::getInstance()->GetObjectTrans());
    glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"withOutline"), withOutline);

    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_Entries.size(); i++)
    {
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        if ( isTextureOverridden() )
        {
            ResourceManager::getSingleton()->getTexture(overrideTexture)->Bind(GL_TEXTURE0);
        }
        else if (MaterialIndex < m_Textures.size())
        {
            ResourceManager::getSingleton()->getTexture(m_Textures[MaterialIndex])->Bind(GL_TEXTURE0);
        }

        if ( hasNormalMap() )
        {
            glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"bumpMap"), 1);
            ResourceManager::getSingleton()->getTexture(normalMap)->Bind(GL_TEXTURE1);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(unsigned int) * m_Entries[i].baseIndex),
                                 m_Entries[i].baseVertex);
    }
    glUniform1i(glGetUniformLocation(ShaderManager::getInstance()->getProgram(),"bumpMap"), 0);

    glBindVertexArray(0);
}

bool Mesh::setTexture(const std::string& Filename)
{
    //overrideTexture = new Texture(GL_TEXTURE_2D,Filename);
    //overrideTexture->Load();
    ResourceManager::getSingleton()->addTexture(Filename);
    overrideTexture = Filename;
    return true;
}

bool Mesh::isTextureOverridden()
{
    return (overrideTexture != std::string(""));
}

void Mesh::clearOverrideTexture()
{
    overrideTexture = std::string("");
}

void Mesh::setNormalMap(const std::string& Filename)
{
    ResourceManager::getSingleton()->addTexture(Filename);
    normalMap=Filename;
}

bool Mesh::hasNormalMap()
{
    return (normalMap != std::string(""));
}
