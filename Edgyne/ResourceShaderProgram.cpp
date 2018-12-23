#include "ResourceShaderProgram.h"
#include "Application.h"
#include "ModuleShaders.h"
#include "JSONManager.h"



ResourceShaderProgram::ResourceShaderProgram() : Resource(RES_SHADER)
{
}

ResourceShaderProgram::ResourceShaderProgram(std::string& file) : Resource(RES_SHADER, file)
{

}


ResourceShaderProgram::~ResourceShaderProgram()
{
}

bool ResourceShaderProgram::CompileShaderProgram()
{
	bool ret = false;
	uint tmp_program_index = 0;

	std::vector<uint> indexList;
	for (std::vector<uint>::iterator it = shaderObjects.begin(); it != shaderObjects.end(); it++)
	{
		bool isVertex = false;
		char* data = App->shaders->FindShaderObjectFromUID(*it, isVertex);
		uint index = 0;
		if (App->shaders->CompileShader(data, isVertex, &index))
			indexList.push_back(index);
	}
	if(App->shaders->CreateShaderProgram(indexList, &program));
	ret = true;

	return ret;
}

void ResourceShaderProgram::AddNewObjectToProgram(uint uuid)
{
	bool isVertex = false;
	if (App->shaders->FindShaderObjectFromUID(uuid, isVertex))
	{
		shaderObjects.push_back(uuid);
		JSON_File* hmm = App->JSON_manager->openWriteFile(("Library\\ShaderPrograms\\" + file).c_str());
		JSON_Value* val = hmm->createValue();
		for (std::vector<uint>::iterator item = shaderObjects.begin(); item != shaderObjects.end(); item++)
		{
			val->addUint("uid", (*item));
		}
		hmm->addValue("", val);
		hmm->Write();
		hmm->closeFile();

		CompileShaderProgram();
	}
}

bool ResourceShaderProgram::ContainsShader(uint uuid)
{
	for (std::vector<uint>::iterator item = shaderObjects.begin(); item != shaderObjects.end(); item++)
	{
		if ((*item) == uuid)
		{
			return true;
		}
	}
	return false;
}
