#include "ResourceManager.h"

ResourceManager *ResourceManager::m_Singleton = 0;

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

ResourceManager *ResourceManager::getSingleton() {
  if (!m_Singleton) {
    m_Singleton = new ResourceManager;
  }
  return m_Singleton;
}

void ResourceManager::Init() {}

Texture *ResourceManager::addTexture(const std::string &fileName) {
  return m_TexturePool.LoadResourceFromFile(fileName);
}
Texture *ResourceManager::getTexture(const std::string &textureHandle) {
  return m_TexturePool.getResource(textureHandle);
}

void ResourceManager::addMesh(const std::string &fileName) {
  m_MeshPool.addResourceFromFile(fileName, fileName);
}
Mesh *ResourceManager::getMesh(const std::string &meshHandle) {
  return m_MeshPool.getResource(meshHandle);
}
