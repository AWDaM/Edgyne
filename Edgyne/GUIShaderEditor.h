#ifndef __GUI_SHADER_EDITOR__
#define __GUI_SHADER_EDITOR__

#include "GUIElements.h"
class TextEditor;

class GUIShaderEditor :
	public GUIElements
{
public:
	GUIShaderEditor(bool active);
	~GUIShaderEditor();

	void SetShaderToEdit(bool fragment, char* shader, const std::string& name);
	void Draw();
public:
	TextEditor* text_editor;
	bool editing_fragment = true;
	std::string shader_name;
};

#endif
