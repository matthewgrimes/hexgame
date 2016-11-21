#pragma once
#define MAX_NUM_ENTITIES 100

#include "Component.h"
#include "Mesh.h"

struct EntityManager
{
	unsigned int componentMasks[MAX_NUM_ENTITIES] = { 0 };

	Mesh Meshes[MAX_NUM_ENTITIES];
};

unsigned int findIndexForNewEntityIn(const EntityManager& manager);