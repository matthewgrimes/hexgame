#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "ResourcePool.h"
#include "Mesh.h"
#include "Texture.h"
#include "ShaderManager.h"

class ResourceManager
{
public:
    static void Destroy();
    static ResourceManager* getSingleton();

    Texture* addTexture(const std::string& fileName);
    Texture* getTexture(const std::string& textureHandle);

    void addMesh(const std::string& fileName);
    Mesh* getMesh(const std::string& meshHandle);

    ShaderManager* getShaderManager();

private:
    ResourceManager();
    ~ResourceManager();
    static void Init();
    static ResourceManager *m_Singleton;

    ResourcePool<Mesh> m_MeshPool;
    TexturePool m_TexturePool;
};

#endif
