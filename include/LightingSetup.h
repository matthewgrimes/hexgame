#ifndef LIGHTINGSETUP_H
#define LIGHTINGSETUP_H

#include "FileLoader.h"
#include "Lighting.h"
#include <vector>

class LightingSetup {
  friend class LightingSetupFileLoader;

public:
  /** Default constructor */
  LightingSetup();
  /** Default destructor */
  virtual ~LightingSetup();
  void Init();
  bool loadFromFile(const std::string &fileName);
  void Bind();

protected:
  LightingTechnique m_LightingTechnique;
  std::vector<PointLight> m_PointLights;
  std::vector<SpotLight> m_SpotLights;

private:
};

class LightingSetupFileLoader : public FileLoader {
public:
  LightingSetupFileLoader(const std::string &name) : FileLoader(name) {
    m_LSetup = NULL;
  }
  ~LightingSetupFileLoader() {}

  void setLightingSetup(LightingSetup *setup) { m_LSetup = setup; }

private:
  LightingSetup *m_LSetup;
  void parseLine(const std::string &field, const std::string &data);
};
#endif // LIGHTINGSETUP_H
