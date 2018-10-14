#pragma once
#include "Module.h"
#include "GUIElements.h"


#include <list>
#include <string>
#include <vector>

class GUIConsole;
class GUIAbout;
class GUIConfiguration;
class GUIRandomNumberTest;
class GUIInspector;
class GUIScene;

class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init(rapidjson::Value& node);
	bool CleanUp();

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void Draw();

	void AddLog(const char* Log);

	//void IntersectionsTest();

	void HelpMenu();


	void MainMenu();
	 
	void ToggleEditor();

private:
	//std::list<std::string> LogInformation;
	bool EditorOff = false;



public:
	GUIConsole* console = nullptr;
	GUIAbout* about = nullptr;
	GUIConfiguration* configuration = nullptr;
	GUIRandomNumberTest* random_number_test = nullptr;
	GUIInspector* inspector = nullptr;
	GUIScene* scene = nullptr;

	std::vector<GUIElements*> GUIElement;

	bool show_demo_window = false;
	bool show_console = false;
	bool show_random_number_test = false;
	bool show_intersections_test = false;
	bool show_configuration = false;
	bool show_about_window = false;
	bool show_inspector = false;
	
	bool perma_true = true;
	bool perma_true_2 = true;
	bool to_close = false;
};

