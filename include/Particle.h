#ifndef PARTICLE_H
#define PARTICLE_H

#include "math_utils.h"
#include <string>

class Particle
{
    public:
        Particle();
        virtual ~Particle();

        Vector3f Speed;
        Vector3f Position;
        Vector3f Color;

        float Lifespan;

        float distanceToCamera;

        bool operator<(Particle& that)
        {
            return this->distanceToCamera > that.distanceToCamera;
        }

        void Update(float deltaT, Vector3f Force=Vector3f(0.f,0.f,0.f));

        std::string textureName;

    protected:

    private:
};


#endif // PARTICLE_H
