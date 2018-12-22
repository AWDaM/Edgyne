#pragma once
#include "Resource.h"
#include <list>
class ResourceShaderProgram :
	public Resource
{
public:
	ResourceShaderProgram(std::string& file);
	~ResourceShaderProgram();

	bool CompileShaderProgram();
	void AddNewObjectToProgram(uint uuid);
	

public: 
	std::vector<uint> shaderObjects;
	uint program;


};

