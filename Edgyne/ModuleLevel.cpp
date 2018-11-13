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

#include "ModuleLoader.h"



ModuleLevel::ModuleLevel(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Level";
}


ModuleLevel::~ModuleLevel()
{
}

bool ModuleLevel::Init(rapidjson::Value& node)
{
	AABB test({ -1,-1,-1 }, { 10, 2, 10 });
	quad_tree = new EdgyQuadTree();
	quad_tree->Create(test);


	root = new GameObject(nullptr, "root");
	game_objects.push_back(root);
	root->transform = (Transform*)root->AddComponent(TRANSFORM);

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
		App->loader->SaveScene();
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		DeleteLevel();
		App->loader->LoadScene();

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

	std::list<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		if ((*item)->active)
		{
			if (game_camera->frustum.ContainsAABBCustom((*item)->aligned_bounding_box)&& game_camera->frustum.Intersects((*item)->bounding_sphere) && App->renderer3D->camera_culling)
			{
				(*item)->Draw();
			}
	
			else if (!App->renderer3D->camera_culling)
			{
				(*item)->Draw();
			}

			if (selected_game_object)
			{
				App->debug->Draw_AABB(selected_game_object->aligned_bounding_box);
			}

			if (App->debug->draw_boundingboxes)
			{
				App->debug->Draw_AABB((*item)->aligned_bounding_box);
			}

			if (App->debug->draw_normals)
			{
				Mesh* mesh = (Mesh*)(*item)->GetComponent(MESH);
				if(mesh)
					App->debug->Draw_Normals(mesh->vertex,mesh->normals,mesh->num_vertex);
			}
		if(game_camera)
			App->debug->Draw_Camera(game_camera);

		}
		item++;
	}

}
