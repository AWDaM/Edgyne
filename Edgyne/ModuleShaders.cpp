#include "ModuleShaders.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleShaders::ModuleShaders(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Shaders";
}


ModuleShaders::~ModuleShaders()
{
}

bool ModuleShaders::Init(rapidjson::Value & node)
{
	vertex_shader = "#version 330 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 color;\n"
		"layout(location = 2) in vec2 texCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 model_matrix;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model_matrix * vec4(position, 1.0f);\n"
		"	ourColor = color;\n"
		"	TexCoord = texCoord;\n"
		"}\n";	pixel_shader = "#version 330 core\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"	color = texture(ourTexture, TexCoord);\n"
		"}\n";	default_vertex_shader = CompileShader(vertex_shader, true);	default_pixel_shader = CompileShader(pixel_shader, false);	std::vector<uint> shader_list;	shader_list.push_back(default_vertex_shader);	shader_list.push_back(default_pixel_shader);	default_shader_program = CreateShaderProgram(shader_list);
	return true;
}

uint ModuleShaders::CompileShader(char * shader, bool is_vertex_shader)
{
	GLuint shader_index = 0;
	if (is_vertex_shader)
		shader_index = glCreateShader(GL_VERTEX_SHADER);
	else
		shader_index = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(shader_index, 1, &shader, NULL);
	glCompileShader(shader_index);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader_index, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		glGetShaderInfoLog(shader_index, 512, NULL, infoLog);
		LOG("Shader compilation error: %s", infoLog);
	}
	//glDeleteShader(shader_index);
	return shader_index;
}

uint ModuleShaders::CreateShaderProgram(std::vector<uint> shaders)
{
	GLuint ret = 0;
	ret = glCreateShaderProgramv(ret);
	for (std::vector<uint>::iterator item = shaders.begin(); item != shaders.end(); item++)
	{
		glAttachShader(ret, (*item));
	}
	glLinkProgram(ret);
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(ret, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ret, 512, NULL, infoLog);
		LOG("Shader link error: %s", infoLog);
	}
}
