#ifndef LIGHTING_H
#define LIGHTING_H

#include "math_utils.h"
#include <GL/glew.h>
#include <vector>

struct BaseLight {
  Vector3f Color;
  float AmbientIntensity;
  float DiffuseIntensity;
  BaseLight() {
    Color = Vector3f(0, 0, 0);
    AmbientIntensity = 0.f;
    DiffuseIntensity = 0.f;
  }
};

struct DirectionalLight : public BaseLight {
  Vector3f Direction;

  DirectionalLight() { Direction = Vector3f(0.f, 0.f, 0.f); }
};
struct PointLight : public BaseLight {
  Vector3f Position;
  struct {
    float Constant;
    float Linear;
    float Exp;
  } Attenuation;

  PointLight() {
    Position = Vector3f(0.f, 0.f, 0.f);
    Attenuation.Constant = 0.f;
    Attenuation.Linear = 0.f;
    Attenuation.Exp = 0.f;
  }
};

struct SpotLight : public PointLight {
  Vector3f Direction;
  float Cutoff;

  SpotLight() {
    Direction = Vector3f(0.f, 0.f, 0.f);
    Cutoff = 0.f;
  }
};
class LightingTechnique {
public:
  static const unsigned int MAX_POINT_LIGHTS = 3;
  static const unsigned int MAX_SPOT_LIGHTS = 3;

  LightingTechnique();

  virtual bool Init();

  void bindDirectionalLight(const DirectionalLight &light);
  void bindPointLights(unsigned int numLights, const PointLight *pointLights);
  void bindSpotLights(unsigned int numLights, const SpotLight *spotLights);
  void bindPointLights(const std::vector<PointLight> &pointLights);
  void bindSpotLights(const std::vector<SpotLight> &spotLights);
  void bindPointLight(unsigned int i, const PointLight *pointLight);
  void bindSpotLight(unsigned int i, const SpotLight *spotLight);
  void setNumPointLights(unsigned int numPointLights);
  void setNumSpotLights(unsigned int numSpotLights);

private:
  GLuint m_numPointLightsLocation;
  GLuint m_numSpotLightsLocation;

  struct {
    GLuint Color;
    GLuint AmbientIntensity;
    GLuint Direction;
    GLuint DiffuseIntensity;
  } m_dirLightLocation;

  struct {
    GLuint Color;
    GLuint AmbientIntensity;
    GLuint DiffuseIntensity;
    GLuint Position;
    struct {
      GLuint Constant;
      GLuint Linear;
      GLuint Exp;
    } Atten;
  } m_pointLightsLocation[MAX_POINT_LIGHTS];

  struct {
    GLuint Color;
    GLuint AmbientIntensity;
    GLuint DiffuseIntensity;
    GLuint Position;
    GLuint Direction;
    GLuint Cutoff;
    struct {
      GLuint Constant;
      GLuint Linear;
      GLuint Exp;
    } Atten;
  } m_spotLightsLocation[MAX_SPOT_LIGHTS];
  GLuint GetUniformLocation(const char *Name);
};

#endif
