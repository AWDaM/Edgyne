#pragma once

#include "Module.h"
#include "Globals.h"
#include <vector>

class ResourceShaderProgram;

class ModuleShaders :
	public Module
{
public:
	ModuleShaders(Application* app, bool start_enabled = true);
	~ModuleShaders();

	bool Init(rapidjson::Value& node);

	bool CreateDefaultProgram();
	bool CompileShader(char* shader, bool is_vertex_shader, uint* shader_index);
	bool CreateShaderProgram(std::vector<uint>& shaders, uint* program_index);

	char* CreateNewShaderObject(const char* shaderName, bool fragment);

	bool SaveShader(std::string& name, char* content, bool fragment);

	char* FindShaderObjectFromUID(uint uid, bool& isVertex);
	ResourceShaderProgram* CreateNewProgram(const char * fileName);
	char* GetShaderDataFromFile(const char* fileName, bool& isVertex);

	void RecompilePrograms(uint new_shader_uuid);
	uint GetShaderUidFromName(std::string& name);

public:
	std::string defaultVertexFile = "Assets\\Shaders\\DefaultVertexShader.edgyvertex";
	std::string defaultPixelFile = "Assets\\Shaders\\DefaultPixelShader.edgypixel";
	uint defaultVertexUID = 0;
	uint defaultPixelUID = 0;

	char* vertex_shader;
	char* pixel_shader;

	ResourceShaderProgram* defaultProgram = nullptr;
};

