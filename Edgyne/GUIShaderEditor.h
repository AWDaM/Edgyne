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

	void Draw();
public:
	TextEditor* text_editor;

};

#endif
