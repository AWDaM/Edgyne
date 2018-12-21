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
	float4 _color = { color, 1.0f };
	for (int i = 0; i < num_vertex * 3; i += 3)
	{
		true_vertex[counter].Assign({ vertex[i],vertex[i + 1],vertex[i + 2] }, _color);

		if(has_texture_coordinates)
		{
			true_vertex[counter].tex_coords = { texCoords[tex_counter], texCoords[tex_counter + 1] };
		}
		else
			true_vertex[counter].tex_coords = { 0,0 };

		if (has_normals)
		{
			true_vertex[counter].normal = { normals[i], normals[i + 1], normals[i + 2] };
		}
		else
			true_vertex[counter].normal = { 0,0,0 };

		counter++;
		tex_counter += 2;
	}
	glBindTexture(GL_TEXTURE_2D, id_texture);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	LOG("%i", sizeof(true_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(Vertex::position) + sizeof(Vertex::tex_coords) + sizeof(Vertex::color) + sizeof(Vertex::normal))*num_vertex, &true_vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *num_index, &index[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12*sizeof(float),(GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12*sizeof(float), (GLvoid*)(7*sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (GLvoid*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

