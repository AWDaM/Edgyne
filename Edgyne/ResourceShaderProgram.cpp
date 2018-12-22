#include "ResourceShaderProgram.h"
#include "Application.h"
#include "ModuleShaders.h"
#include "JSONManager.h"



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

	void* buffer;
	if (App->shaders->CreateShaderProgram(shaderObjects, &tmp_program_index))
	{
		program = tmp_program_index;
		ret = true;
	}
	return ret;
}

void ResourceShaderProgram::AddNewObjectToProgram(uint uuid)
{
	if (App->shaders->FindShaderObjectFromUID(uuid))
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
