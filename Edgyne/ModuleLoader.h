#pragma once
#include "Module.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#pragma comment(lib,"Assimpl/libx86/assimp.lib")

class ModuleLoader : public Module
{
public:
	ModuleLoader(Application* app, bool start_enabled = true);
	~ModuleLoader();
};

