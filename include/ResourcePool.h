#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H

#include <map>
#include <string>

template <typename T> class ResourcePool {
public:
  ResourcePool();
  ~ResourcePool();
  void Init();
  T *getResource(const std::string &resourceHandle);

  void addResource(const std::string &newHandle, const T &newResource);
  void addResourceFromFile(const std::string &fileName,
                           const std::string &resourceHandle);

private:
  std::map<std::string, T> m_Resources;
};

template <typename T> ResourcePool<T>::ResourcePool() {}

template <typename T> ResourcePool<T>::~ResourcePool() {}

template <typename T> void ResourcePool<T>::Init() {}

template <typename T>
T *ResourcePool<T>::getResource(const std::string &resourceHandle) {
  return &m_Resources[resourceHandle];
}

template <typename T>
void ResourcePool<T>::addResource(const std::string &newHandle,
                                  const T &newResource) {
  if (m_Resources.count(newHandle) == 0) {
    m_Resources[newHandle] = newResource;
  }
}

template <typename T>
void ResourcePool<T>::addResourceFromFile(const std::string &fileName,
                                          const std::string &resourceHandle) {
  if (m_Resources.count(resourceHandle) == 0) {
    m_Resources[resourceHandle].LoadFromFile(fileName);
  }
}
#endif
