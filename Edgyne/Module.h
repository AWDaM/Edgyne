#pragma once
#include "rapidjson/document.h"// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filewritestream.h"

#include<string>

class Application;

class Module
{
private :
	bool enabled;

public:
	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(rapidjson::Value& node)
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os) {};
	virtual void Load(rapidjson::Document& doc) {};

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void Configuration()
	{}

public:
	std::string name = "";
};