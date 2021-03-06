#ifndef __MODULE_H__
#define __MODULE_H__


#include "JSONManager.h"


#include<string>
#include <list>


#include "Globals.h"

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

#endif // !__MODULE_H__