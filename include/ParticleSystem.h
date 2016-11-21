#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include "ShaderManager.h"
#include "Texture.h"
#include <vector>

const unsigned int MAX_PARTICLES = 100;


class ParticleSystem
{
    public:
        ParticleSystem(std::string shaderProgram, Texture* texture, unsigned int numParticles);
        virtual ~ParticleSystem();

        void Update(float deltaT);

        void Draw();

        int firstUnusedParticle();

        void Cleanup();

    protected:

    private:
        Particle ParticlesContainer[MAX_PARTICLES];

        unsigned int lastUsedParticle;

        unsigned int numberOfParticles;

        Texture* particleTexture;
        std::string Shader;

        GLuint VAO;

        void Init();

        void respawnParticle(Particle& particle, const Vector3f& offset);

        std::vector<Particle> Particles;
};

#endif // PARTICLESYSTEM_H
