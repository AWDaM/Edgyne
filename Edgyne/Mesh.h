#ifndef __MESH_H__
#define __MESH_H__



#include "Component.h"
#include "Globals.h"

class Material;

class Mesh : public Component
{
public:
	Mesh();
	Mesh(GameObject* game_object);
	~Mesh();

	rapidjson::Value SaveToScene(rapidjson::Document::AllocatorType& allocator);
	void LoadComponent(rapidjson::Value::ConstMemberIterator comp);

	void OnEditor();
	bool ComponentUpdate();
	bool ComponentDraw();
	void SetBoundingVolume();
	void TransformChanged();

	void CopyDataFromFile(std::string& path);
private:
public:
	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint num_vertex = 0;
	float* vertex = nullptr;

	float* normals = nullptr;

	uint id_texture = 0;
	float* texCoords = nullptr;
	bool has_texture_coordinates = false;
	bool has_normals = false;
	bool has_triangle_faces = true;

	std::string fileName;

	Material* material = nullptr;

private:
	
};

#endif // !__MESH_H__