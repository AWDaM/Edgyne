#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleResourceManager.h"
#include "GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <cstddef>
#include <stddef.h> 


ResourceMesh::ResourceMesh(std::string& file) : Resource(ResourceType::RESOURCE_MESH,file)
{
	LoadResource(file);
}

ResourceMesh::ResourceMesh() : Resource(ResourceType::RESOURCE_MESH)
{

}



ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::LoadResource(std::string& path)
{
	file = path;
	std::string tmp = "Library\\Meshes\\";

	tmp += file;
	
	App->importer->CopyDataFromFile(tmp, this);

	int counter = 0;
	int tex_counter = 0;
	true_vertex = new Vertex[num_vertex];
	color = { 1.0f,0.5f,0.5f };
	for (int i = 0; i < num_vertex * 3; i += 3)
	{
		//LOG("%i",counter);
		true_vertex[counter].Assign({ vertex[i],vertex[i + 1],vertex[i + 2] }, { texCoords[tex_counter],texCoords[tex_counter + 1] }, color);
		counter++;
		tex_counter += 2;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	LOG("%i", sizeof(true_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(Vertex::position) + sizeof(Vertex::tex_coords) +sizeof(Vertex::color))*num_vertex, &true_vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *num_index, &index[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

