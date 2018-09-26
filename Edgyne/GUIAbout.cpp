#include "GUIAbout.h"
#include "Application.h"


GUIAbout::GUIAbout() : GUIElements("about")
{
}


GUIAbout::~GUIAbout()
{
}

void GUIAbout::Draw()
{
	ImGui::Begin("Edgyne", &active);
	ImGui::Dummy({ 0, 10 });
	ImGui::Text("Simple 3D engine developed for the Game Engines subject ");
	ImGui::Dummy({ 0, 10 });
	ImGui::Text("Authors: ");
	if (ImGui::Button("Lorien Portella Lopez"))					App->OpenBrowser("https://github.com/Witiza");
	if (ImGui::Button("Jaume Suris Valhondo "))					App->OpenBrowser("https://github.com/Farmak09");
	ImGui::Dummy({ 0, 10 });
	ImGui::Text("Libraries Used:");
	if (ImGui::Button("Simple Directmedia Layer 2.0.8"))		App->OpenBrowser("https://www.libsdl.org/");
	if (ImGui::Button("OpenGL 3.1"))									App->OpenBrowser("https://www.opengl.org/");
	if (ImGui::Button("MathGeoLib 1.5"))								App->OpenBrowser("https://github.com/juj/MathGeoLib");
	if (ImGui::Button("ImGui 1.65"))									App->OpenBrowser("https://github.com/ocornut/imgui/wiki");
	if (ImGui::Button("PCG Random 0.94"))							App->OpenBrowser("http://www.pcg-random.org/");
	if (ImGui::Button("RapidJSON 1.1.0"))							App->OpenBrowser("http://rapidjson.org/");
	ImGui::Dummy({ 0, 20 });
	ImGui::Text("License:");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("MIT License");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("Copyright(c) 2018 Jaume Suris Valhondo, Lorien Portella Lopez");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
	ImGui::Text("of this software and associated documentation files(the Software), to deal");
	ImGui::Text("in the Software without restriction, including without limitation the rights");
	ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
	ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
	ImGui::Text("furnished to do so, subject to the following conditions :");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("The above copyright notice and this permission notice shall be included in all");
	ImGui::Text("copies or substantial portions of the Software.");
	ImGui::Dummy({ 0, 5 });
	ImGui::Text("THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
	ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
	ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
	ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
	ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
	ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
	ImGui::Text("SOFTWARE.");


	ImGui::End();
}
