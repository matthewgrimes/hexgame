#include "Particle.h"

Particle::Particle() : Speed(0.f,0.f,0.f), Position(0.f,0.f,0.f), Color(0.f,0.f,0.f), Lifespan(0.f), distanceToCamera(0.f), textureName("white.png")
{
    //ctor
}

Particle::~Particle()
{
    //dtor
}

void Particle::Update(float deltaT, Vector3f Force)
{
    Lifespan -= deltaT;
    Speed += Force * deltaT;
    Position += Speed * deltaT;
}
