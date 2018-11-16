#ifndef __MODULE_TIME_MANAGER_H__
#define __MODULE_TIME_MANAGER_H__

#include "Module.h"

class PerfTimer;

class ModuleTimeManager :
	public Module
{
public:
	ModuleTimeManager(Application* app, bool start_enabled = true);
	~ModuleTimeManager();

	bool Init(rapidjson::Value& node);
	bool CleanUp();

	update_status Update(float dt);

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);

public:
	PerfTimer* real_clock;
	PerfTimer* game_clock;
};


#endif // !__MODULE_TIME_MANAGER_H__
