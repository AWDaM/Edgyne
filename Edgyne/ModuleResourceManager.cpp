#include "ModuleResourceManager.h"
#include "Material.h"
#include "Mesh.h"


ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "ResourceManager";
}


ModuleResourceManager::~ModuleResourceManager()
{
}

void MaterialResource::GenerateMaterial(Material * buffer)
{

}

void MeshResource::GenerateMesh(Mesh * buffer)
{

}
