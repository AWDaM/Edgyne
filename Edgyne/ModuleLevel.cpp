#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleLevel.h"
#include "ModuleLoader.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleResourceManager.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "ModuleDebug.h"
#include "QuadTree.h"
#include "Mesh.h"

#include "SDL\include\SDL_opengl.h"





ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Level";
}


ModuleLevel::~ModuleLevel()
{
}

bool ModuleLevel::Init(rapidjson::Value& node)
{
	AABB test({ -100,-10,-100 }, { 100, 10, 100 });
	quad_tree = new EdgyQuadTree();
	quad_tree->Create(test);


	root = new GameObject(nullptr, "root");
	game_objects.push_back(root);
	root->transform = (Transform*)root->AddComponent(TRANSFORM);

	current_scene_name = "New Scene";
	return true;
}

bool ModuleLevel::Start()
{
	//App->loader->ReceivedFile("Assets\\BakerHouse\\BakerHouse.FBX");
	GameObject* camera = NewGameObject("Camera");
	game_camera = (Camera*)camera->AddComponent(CAMERA);
	camera->tag = MAIN_CAMERA;
		
		return true;
}

bool ModuleLevel::CleanUp()
{
	selected_game_object = nullptr;
	game_camera = nullptr;

	RELEASE(root);
	RELEASE(quad_tree);

	return true;
}
update_status ModuleLevel::PreUpdate(float dt)
{
	std::list<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end() && game_objects.size() > 1)
	{

		if ((*item)->to_remove)
		{
			if ((*item) != root)
				(*item)->RemoveSelfFromParent();
			else
				(*item)->to_remove = false;

			(*item)->RecursiveDeleteGameObject();
			item = game_objects.begin();
		}
		else
			item++;

	}

	if (generate_quadtree)
	{
		std::list<GameObject*>::iterator item = game_objects.begin();

		quad_tree->Clear();
		while (item != game_objects.end())
		{
			if ((*item)->Static && (*item)->aligned_bounding_box.IsFinite())
			{
				quad_tree->Insert((*item));
			}
			item++;
		}
		generate_quadtree = false;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleLevel::Update(float dt)
{
	std::list<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		if ((*item)->active)
		{
			(*item)->Update();
		}
		item++;
	}
	if (App->camera->scene_clicked && !App->debug->usingGuizmo && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
	{
		selected_game_object = MousePicking(App->input->GetMouseX(), App->input->GetMouseY());
	}

	return UPDATE_CONTINUE;
}

GameObject * ModuleLevel::NewGameObject(std::string name, bool with_transform)
{
	GameObject* ret = root->AddGameObject(name, with_transform);

	return ret;
}

std::vector<GameObject*> ModuleLevel::GetNonStaticObjects()
{
	std::vector<GameObject*> ret;
	std::list<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		if (!(*item)->Static) {}
			ret.push_back((*item));
		item++;
	}
	return ret;
}

void ModuleLevel::DeleteLevel()
{
	std::list<GameObject*>::iterator item = root->childs.begin();

	while (item != root->childs.end())
	{
		RELEASE((*item));
	}
	root->childrenUID.clear();
}

void ModuleLevel::Draw()
{
	if(root)
		root->RecursiveTransformChanged(root->global_transform_matrix);
	//------------------General Draws----------------------//
	if (selected_game_object)
		App->debug->Draw_AABB(selected_game_object->aligned_bounding_box);

	//-------------With Camera Culling---------------//
	std::map<float, Mesh*> map;

	if (App->renderer3D->camera_culling)
	{
		std::vector<GameObject*> frustum_game_objects;

		quad_tree->CollectIntersections(frustum_game_objects, game_camera->frustum);

		GenerateDistanceToCamMap(map,frustum_game_objects, game_camera->frustum);

	}
	//---------------------Without Camera Culling and Active game_obejcts------------//
	std::list<GameObject*>::iterator _item = game_objects.begin();

	while (_item != game_objects.end())
	{
		if (App->debug->draw_boundingboxes)
		{
			App->debug->Draw_AABB((*_item)->aligned_bounding_box);
		}

		if ((*_item)->GetComponent(CAMERA))
		{
			(*_item)->Draw();
		}

		if (!(*_item)->Static)
		{
			Mesh* mesh = (Mesh*)(*_item)->GetComponent(MESH);
			if (mesh)
			{
				map.emplace( game_camera->frustum.Distance((*_item)->transform->position), mesh);
			}
		}
		
		else if ((*_item)->Static && !App->renderer3D->camera_culling)
		{
			Mesh* mesh = (Mesh*)(*_item)->GetComponent(MESH);
			if (mesh)
			{
				map.emplace(game_camera->frustum.Distance((*_item)->transform->position), mesh);
			}
		}

		_item++;
	}

	//Here we draw the meshes. first for for opaque,second for transparency
	for (std::map<float, Mesh*>::iterator it = map.begin(); it != map.end(); ++it)
	{
		if((*it).second->IsOpaque())
			(*it).second->ComponentDraw();
	}

	for (std::map<float, Mesh*>::iterator it = map.begin(); it != map.end(); ++it)
	{
		if (!(*it).second->IsOpaque())
			(*it).second->ComponentDraw();
	}

	root->RecursiveResetAddedToQuadTree();
}

void ModuleLevel::GenerateDistanceToCamMap(std::map<float, Mesh*>& buffer, std::vector<GameObject*> objects_to_draw, math::Frustum camera)
{

	std::vector<GameObject*>::iterator item = objects_to_draw.begin();

	while (item != objects_to_draw.end())
	{
		if ((*item)->active && (*item)->Static)
		{
			if (game_camera->frustum.ContainsAABBCustom((*item)->aligned_bounding_box) && game_camera->frustum.Intersects((*item)->bounding_sphere))
			{
				Mesh* mesh = (Mesh*)(*item)->GetComponent(MESH);
				if (mesh)
				{
					buffer.emplace(camera.Distance((*item)->transform->position), mesh);
					item++;
				}
			}
		}
	}
}



GameObject* ModuleLevel::MousePicking(int posX, int posY)
{
	GameObject* go = nullptr;

	float shortestDistance = FLOAT_INF;


	float mouseX = (float)posX - App->imGui->GetScenePos().x;
	float mouseY = (float)posY - App->imGui->GetScenePos().y;

	mouseX = (mouseX / (App->imGui->GetSceneSize().x / 2)) - 1;
	mouseY = (mouseY / (App->imGui->GetSceneSize().y / 2)) - 1;
	ray = App->camera->editor_camera->frustum.UnProjectLineSegment(mouseX, -mouseY);


	// Static objects
	std::vector<GameObject*> hits;
	App->level->quad_tree->CollectIntersections(hits, ray);
	LOG("%i", hits.size());
	// Dynamic objects
	std::vector<GameObject*> dynamicGameObjects = GetNonStaticObjects();

	for (uint i = 0; i < dynamicGameObjects.size(); ++i)
	{
		if (dynamicGameObjects[i]->aligned_bounding_box.IsFinite() && ray.Intersects(dynamicGameObjects[i]->aligned_bounding_box))
			hits.push_back(dynamicGameObjects[i]);
	}
	float distance;

	for (int i = 0; i < hits.size(); ++i)
	{

		math::Plane face[6];
		hits[i]->aligned_bounding_box.GetFacePlanes(face);
		for (int j = 0; j < 6; j++)
		{
			if (ray.Intersects(face[j], &distance))
			{
				if (shortestDistance > distance)
				{
					shortestDistance = distance;
					go = hits[i];
				}
			}
		}
	}
	/*
	for (int i = 0; i < hits.size(); ++i)
	{
		float3 first, second, third;
		math::LineSegment localSegment(ray);
		localSegment.Transform(hits[i]->global_transform_matrix.Inverted());

		Mesh* component_mesh = (Mesh*)hits[i]->GetComponent(MESH);
		ResourceMesh* mesh = (ResourceMesh*)App->resource_manager->GetResourceFromUID(component_mesh->resource_mesh);

		if(mesh && mesh->has_triangle_faces)
		for (int j = 0; j < mesh->num_index;)
		{
			first = math::float3(mesh->vertex[mesh->index[j] * 3], mesh->vertex[mesh->index[j] * 3 + 1], mesh->vertex[mesh->index[j] * 3 + 2]);  j++;
			second = math::float3(mesh->vertex[mesh->index[j] * 3], mesh->vertex[mesh->index[j] * 3 + 1], mesh->vertex[mesh->index[j] * 3 + 2]);  j++;
			third = math::float3(mesh->vertex[mesh->index[j] * 3], mesh->vertex[mesh->index[j] * 3 + 1], mesh->vertex[mesh->index[j] * 3 + 2]);  j++;

			float distance;
			if (localSegment.Intersects({ first, second, third }, &distance))
			{
				if (shortestDistance > distance)
				{
					shortestDistance = distance;
					go = hits[i];
				}
			}
		}
	}*/
	return go;
}