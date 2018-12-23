#include "GUIShaderEditor.h"
#include "Application.h"
#include "ModuleShaders.h"
#include "ImGuiTextEditor/TextEditor.h"



GUIShaderEditor::GUIShaderEditor(bool active = false) : GUIElements("Shader Editor", active)
{
	text_editor = new TextEditor();
	text_editor->SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());

}


GUIShaderEditor::~GUIShaderEditor()
{
}

void GUIShaderEditor::SetShaderToEdit(bool fragment)
{
	if (fragment)
	{
		text_editor->SetText(App->shaders->vertex_shader);
		editing_fragment = true;
	}
	else
	{
		text_editor->SetText(App->shaders->pixel_shader);
		editing_fragment = false;
	}
}

void GUIShaderEditor::Draw()
{
	auto cpos = text_editor->GetCursorPosition();
	ImGui::Begin("Text Editor Demo", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Compile"))
			{
	
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = text_editor->IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				text_editor->SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && text_editor->CanUndo()))
				text_editor->Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && text_editor->CanRedo()))
				text_editor->Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, text_editor->HasSelection()))
				text_editor->Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && text_editor->HasSelection()))
				text_editor->Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && text_editor->HasSelection()))
				text_editor->Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				text_editor->Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				text_editor->SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(text_editor->GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				text_editor->SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				text_editor->SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				text_editor->SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1,text_editor->GetTotalLines(),
		text_editor->IsOverwrite() ? "Ovr" : "Ins",
		text_editor->CanUndo() ? "*" : " ",
		text_editor->GetLanguageDefinition().mName.c_str());

	text_editor->Render(name.c_str());
	ImGui::End();
}
