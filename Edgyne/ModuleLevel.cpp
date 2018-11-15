#include "Application.h"
#include "ModuleLevel.h"
#include "ModuleLoader.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Camera.h"
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

	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		App->loader->SaveScene("edgyscene");
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		App->loader->LoadScene("edgyscene");
	}

	return UPDATE_CONTINUE;
}

GameObject * ModuleLevel::NewGameObject(std::string name, bool with_transform)
{
	GameObject* ret = root->AddGameObject(name, with_transform);

	return ret;
}

void ModuleLevel::DeleteLevel()
{
	std::list<GameObject*>::reverse_iterator item = game_objects.rbegin();

	while (item != game_objects.rend())
	{
		(*item)->CleanUp();
		game_objects.pop_back();
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
	if (App->renderer3D->camera_culling)
	{
		std::vector<GameObject*> frustum_game_objects;

		quad_tree->CollectIntersections(frustum_game_objects, game_camera->frustum);

		std::vector<GameObject*>::iterator item = frustum_game_objects.begin();

		while (item != frustum_game_objects.end())
		{
			if ((*item)->active && (*item)->Static)
			{
				if (game_camera->frustum.ContainsAABBCustom((*item)->aligned_bounding_box) && game_camera->frustum.Intersects((*item)->bounding_sphere))
				{
					(*item)->Draw();
				}
				/*	if (App->debug->draw_normals)
					{
						Mesh* mesh = (Mesh*)(*item)->GetComponent(MESH);
						if(mesh)
							App->debug->Draw_Normals(mesh->vertex,mesh->normals,mesh->num_vertex);
					}*/
			}
			item++;
		}
	}
	//---------------------Without Camera Culling and Active game_obejcts------------//
	std::list<GameObject*>::iterator _item = game_objects.begin();

	while (_item != game_objects.end())
	{
		if (App->debug->draw_boundingboxes)
		{
			App->debug->Draw_AABB((*_item)->aligned_bounding_box);
		}

		if (!(*_item)->Static)
			(*_item)->Draw();

		else if((*_item)->Static && !App->renderer3D->camera_culling)
			(*_item)->Draw();

		_item++;
	}

	root->RecursiveResetAddedToQuadTree();
}
