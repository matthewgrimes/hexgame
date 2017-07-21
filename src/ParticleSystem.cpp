#include "ParticleSystem.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"

ParticleSystem::ParticleSystem(std::string shaderProgram, Texture *texture,
                               unsigned int numParticles)
    : numberOfParticles(std::min(MAX_PARTICLES, numParticles)),
      particleTexture(texture), Shader(shaderProgram) {
  this->Init();
}

ParticleSystem::~ParticleSystem() {
  // dtor
}
/** \brief Update particle system at each time step
 *
 * \param deltaT - amount of time elapsed since last update
 *
 */

void ParticleSystem::Update(float deltaT) {
  for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
    if (Particles[i].Lifespan < 0) {
      lastUsedParticle = i;
    } else {
      Particles[i].Update(deltaT);
    }
  }
}
/**< Returns the index of the first available particle slot. */
int ParticleSystem::firstUnusedParticle() {
  // first check starting at last used particle
  for (unsigned int i = lastUsedParticle; i < MAX_PARTICLES; ++i) {
    if (Particles[i].Lifespan < 0) {
      lastUsedParticle = i;
      return i;
    }
  }
  // otherwise start from scratch
  for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
    if (Particles[i].Lifespan < 0) {
      lastUsedParticle = i;
      return i;
    }
  }

  // if all else fails, return 0
  lastUsedParticle = 0;
  return 0;
}

void ParticleSystem::Init() {
  // Set up mesh and attribute properties
  GLuint VBO;
  GLfloat particle_quad[] = {
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(this->VAO);
  // Fill mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad,
               GL_STATIC_DRAW);
  // Set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)0);
  glBindVertexArray(0);

  for (unsigned int i = 0; i < numberOfParticles; ++i) {
    Particles.push_back(Particle());
  }
}

void ParticleSystem::Draw() {
  // Use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  ServiceLocator::getMainDisplay().getShaderManager()->Activate(Shader);

  for (Particle particle : this->Particles) {
    if (particle.Lifespan > 0.0f) {
      ServiceLocator::getMainDisplay().getShaderManager()->setUniform(
          particle.Position, "gOffset");
      ServiceLocator::getMainDisplay().getShaderManager()->setUniform(
          particle.Color, "gColor");

      ResourceManager::getSingleton()
          ->getTexture(particle.textureName)
          ->Bind(GL_TEXTURE0);

      glBindVertexArray(this->VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  // Don't forget to reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
