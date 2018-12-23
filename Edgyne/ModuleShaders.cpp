#include "ModuleShaders.h"
#include "Application.h"
#include "ModuleResourceManager.h"
#include "ResourceShaderProgram.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <experimental\filesystem>
ModuleShaders::ModuleShaders(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Shaders";
}


ModuleShaders::~ModuleShaders()
{
}

bool ModuleShaders::Init(rapidjson::Value & node)
{
	bool ret = true;
	//renderer things they should go back to where they belong
	App->renderer3D->context = SDL_GL_CreateContext(App->window->window);
	if (App->renderer3D->context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		LOG("Glew library not initialized properly %s\n", glewGetErrorString(err));
		ret = false;
	}
	//------------------------------------------------------------------
	FILE* vfile = fopen(defaultVertexFile.c_str(), "rb");
	if (ftell(vfile) < 2)
	{
		fseek(vfile, 0, SEEK_END);
		vertex_shader = "#version 330 core\n"
			"layout(location = 0) in vec3 position;\n"
			"layout(location = 1) in vec4 color;\n"
			"layout(location = 2) in vec2 texCoord;\n"
			"layout(location = 3) in vec3 normal;\n"
			"out vec4 ourColor;\n"
			"out vec2 TexCoord;\n"
			"out vec3 end_pos;\n"
			"out vec3 ret_normal;\n"
			"out float amp;\n"
			"out float max_amp;\n"

			"uniform mat4 model_matrix;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"uniform float time;\n"
			"void main()\n"
			"{\n"
			"float freq = 10;\n"
			"max_amp = 1;\n"
			"end_pos = position;\n"
			"amp =  max_amp*sin(freq*time+(end_pos.z));\n"
			"end_pos.y += amp;\n"
			//"end_pos.y += sin(end_pos.x);\n"
			"	gl_Position = projection * view * model_matrix * vec4(end_pos, 1.0f);\n"
			"	ourColor = color;\n"
			"	TexCoord = texCoord;\n"
			"	ret_normal = normal;\n"
			"}\n";
		fwrite(vertex_shader, 1, strlen(vertex_shader), vfile);

		std::string metaPath = defaultVertexFile;
		metaPath = metaPath.append(".meta");
		JSON_File* meta = App->JSON_manager->openWriteFile(metaPath.c_str());

		JSON_Value* muid = meta->createValue();
		defaultVertexUID = pcg32_random_r(&App->rng);
		muid->addUint("uid", defaultVertexUID);
		meta->addValue("meta", muid);

		meta->Write();
		meta->closeFile();
	}
	else
	{
		std::string metaPath = defaultVertexFile;
		metaPath = metaPath.append(".meta");
		JSON_File* meta = App->JSON_manager->openReadFile(metaPath.c_str());
		JSON_Value* muid = meta->getValue("meta");
		defaultVertexUID = muid->getBool("uid");
		meta->closeFile();
	}
	fclose(vfile);
		//vertex_shader = "#version 330 core\n"
		//"layout(location = 0) in vec3 position;\n"
		//"uniform mat4 model_matrix;\n"
		//"uniform mat4 view;\n"
		//"uniform mat4 projection;\n"
		//"void main()\n"
		//"{\n"
		//"	gl_Position = projection * view * model_matrix * vec4(position, 1.0f);\n"
		//"}\n";
	FILE* pfile = fopen(defaultPixelFile.c_str(), "rb");
	if (ftell(pfile) < 2)
	{
		fseek(pfile, 0, SEEK_END);
		pixel_shader = "#version 330 core\n"
			"in vec4 ourColor;\n"
			"in vec2 TexCoord;\n"
			"in vec3 end_pos;\n"
			"in float max_amp;\n"
			"in float amp;\n"
			"out vec4 color;\n"
			"out vec4 texture_color;\n"
			"in vec3 ret_normal;\n"
			"uniform sampler2D ourTexture;\n"
			"void main()\n"
			"{\n"
			"	texture_color = texture(ourTexture, TexCoord);\n"
			//	" color = vec4(vec3((texture_color.r+texture_color.g+texture_color.b)/3.0),1.0);\n"
			" color = vec4(amp/max_amp,amp/max_amp,1,0.5);\n"
			//"	color = vec4(ret_normal,1.0);\n"
			"}\n";
		fwrite(pixel_shader, 1, strlen(pixel_shader), pfile);

		std::string metaPath = defaultPixelFile;
		metaPath = metaPath.append(".meta");
		JSON_File* meta = App->JSON_manager->openWriteFile(metaPath.c_str());

		JSON_Value* muid = meta->createValue();
		defaultPixelUID = pcg32_random_r(&App->rng);
		muid->addUint("uid", defaultPixelUID);
		meta->addValue("meta", muid);

		meta->Write();
		meta->closeFile();
	}
	else
	{
		std::string metaPath = defaultPixelFile;
		metaPath = metaPath.append(".meta");
		JSON_File* meta = App->JSON_manager->openReadFile(metaPath.c_str());
		JSON_Value* muid = meta->getValue("meta");
		defaultPixelUID = muid->getBool("uid");
		meta->closeFile();
	}
	fclose(pfile);
	//pixel_shader = "#version 330 core\n"
	//	"out vec4 FragColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"   FragColor = vec4(1.0f, 0.7f, 0.7f, 1.0f);\n"
	//	"}\n\0";

	CreateDefaultProgram();
	return ret;
}

bool ModuleShaders::CreateDefaultProgram()
{
	uint tmp_vertex_index = 0;
	uint tmp_fragment_index = 0;
	uint tmp_program_index = 0;
	if (CompileShader(vertex_shader, true, &tmp_vertex_index))
		default_vertex_shader = tmp_vertex_index;
	if (CompileShader(pixel_shader, false, &tmp_fragment_index))
		default_pixel_shader = tmp_fragment_index;

	std::vector<uint> shader_list;
	shader_list.push_back(default_vertex_shader);
	shader_list.push_back(default_pixel_shader);

	void* buffer;
	if (CreateShaderProgram(shader_list, &tmp_program_index))
	{
		default_shader_program = tmp_program_index;
	}
	return true;
}

bool ModuleShaders::CompileShader(char * shader, bool is_vertex_shader, uint* shader_index)
{
	bool ret = true;
	if (is_vertex_shader)
		*shader_index = glCreateShader(GL_VERTEX_SHADER);
	else
		*shader_index = glCreateShader(GL_FRAGMENT_SHADER);


	glShaderSource(*shader_index, 1, &shader, NULL);
	glCompileShader(*shader_index);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(*shader_index, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(*shader_index, 512, NULL, infoLog);
		LOG("Shader compilation error: %s", infoLog);
		ret = false;
	}
	else
		LOG("Shader compilation success");
	//glDeleteShader(shader_index);
	return ret;
}

bool ModuleShaders::CreateShaderProgram(std::vector<uint>& shaders, uint* program_index)
{
	bool ret = true;

	*program_index = glCreateProgram();
	for (std::vector<uint>::iterator item = shaders.begin(); item != shaders.end(); item++)
	{
		glAttachShader(*program_index, (*item));
	}
	glLinkProgram(*program_index);
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(*program_index, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(*program_index, 512, NULL, infoLog);
		LOG("Shader link error: %s", infoLog);
		ret = false;
	}
	else
		LOG("Shader link success");

	return ret;
}

char* ModuleShaders::FindShaderObjectFromUID(uint uid, bool& isVertex)
{
	char* ret = "";
	const std::experimental::filesystem::directory_iterator end{};

	for (std::experimental::filesystem::directory_iterator iter{ "Assets\\" }; iter != end; ++iter)
	{
		if (std::experimental::filesystem::is_regular_file(*iter))
		{
			//std::string extension = (*iter).path().extension().string();
			if ((*iter).path().extension().string() == "meta")
			{
				JSON_File* meta = App->JSON_manager->openReadFile((*iter).path().string().c_str());
				JSON_Value* muid = meta->getValue("meta");
				if (uid == muid->getUint("uid"))
				{
					std::string shaderFile = (*iter).path().string();
					shaderFile = shaderFile.substr(0, shaderFile.find_last_of("."));
					strcpy(ret, GetShaderDataFromFile(shaderFile.c_str(), isVertex));			
				}
				meta->closeFile();
			}
		}
	}
	return ret;
}

char* ModuleShaders::GetShaderDataFromFile(const char* fileName, bool& isVertex)
{
	char* ret;

	FILE* file = fopen(fileName, "rb");
	int size = ftell(file);

	fseek(file, 0, SEEK_END);
	if (size > 0)
	{
		ret = new char[size];
		fread(ret, 1, size, file);
	}
	std::string fn = fileName;
	fn = fn.erase(0, fn.find_last_of("."));
	if (fn == ".edgyvertex")
		isVertex = true;
	return ret;
}

void ModuleShaders::CreateNewShaderObject(const char* shaderName, const char* bufferData)
{
	std::string path = "Assets\\";
	path += shaderName;

	FILE* file = fopen(path.c_str(), "wb");

	char* data = "#version 330 core\n";
	strcat(data, bufferData);

	fwrite(data, sizeof(char), strlen(data), file);
	fclose(file);

	std::string metaPath = path;
	metaPath = metaPath.append(".meta");
	JSON_File* meta = App->JSON_manager->openWriteFile(metaPath.c_str());

	JSON_Value* muid = meta->createValue();
	muid->addUint("uid", pcg32_random_r(&App->rng));
	meta->addValue("meta", muid);

	meta->Write();
	meta->closeFile();
}

void ModuleShaders::CreateNewProgram(const char * fileName)
{
	std::string fn = fileName;
	ResourceShaderProgram* rsp = (ResourceShaderProgram*)App->resource_manager->CreateNewResource(Resource::RES_SHADER, fn);
	rsp->shaderObjects.push_back(defaultVertexUID);
	rsp->shaderObjects.push_back(defaultPixelUID);
	std::vector<uint> indexList;
	for (std::vector<uint>::iterator it = rsp->shaderObjects.begin(); it != rsp->shaderObjects.end(); it++)
	{
		bool isVertex = false;
		char* data = FindShaderObjectFromUID(*it, isVertex);
		uint index = 0;
		if(CompileShader(data, isVertex, &index))
			indexList.push_back(index);
	}
	CreateShaderProgram(indexList, &rsp->program);
}



