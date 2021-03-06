#ifndef __MODULE_IMGUI_H__
#define __MODULE_IMGUI_H__


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
class GUIHierarchy;
class GUIAssets;
class GUIShaderEditor;

class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init(rapidjson::Value& node);
	bool CleanUp();

	void Save(rapidjson::Document& doc, rapidjson::FileWriteStream& os);
	void Load(rapidjson::Document& doc);

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void Draw();

	void AddLog(const char* Log);

	void HelpMenu();

	void MainMenu();
	 
	void ToggleEditor();

	ImVec2 GetScenePos();
	ImVec2 GetSceneSize();

private:

public:
	bool EditorOff = false;

	GUIConsole* console = nullptr;
	GUIAbout* about = nullptr;
	GUIConfiguration* configuration = nullptr;
	GUIRandomNumberTest* random_number_test = nullptr;
	GUIInspector* inspector = nullptr;
	GUIScene* scene = nullptr;
	GUIHierarchy* hierarchy = nullptr;
	GUIAssets* assets = nullptr;
	GUIShaderEditor* shader_editor = nullptr;

	std::vector<GUIElements*> GUIElement;
	std::string ImGuiIniLocation;

	bool show_demo_window = false;

	float sceneX;
	float sceneY;
	float sceneW;
	float sceneH;
	bool perma_true = true;
	bool perma_true_2 = true;
	bool to_close = false;
};

#endif // !__MODULE_IMGUI_H__

