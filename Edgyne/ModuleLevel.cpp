#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleLevel.h"
#include "ModuleLoader.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
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
	std::list<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		(*item)->CleanUp();
		item++;
	}


	RELEASE(selected_game_object);
	RELEASE(root);
	RELEASE(game_camera);

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
	if (debugRay)
	{
		glBegin(GL_LINES);
		glLineWidth(3.0f);

		glVertex3f(ray.a.x,ray.a.y,ray.a.z);
		glVertex3f(ray.b.x,ray.b.y,ray.b.z);
		glEnd();
		glLineWidth(1.0f);

		glBegin(GL_TRIANGLES);
		for (std::vector<math::float3>::iterator it = triangles.begin(); it != triangles.end(); it++)
		{
			glVertex3f((*it).x, (*it).y, (*it).z);
		}
		glEnd();
	}

	std::list<GameObject*>::iterator item = game_objects.begin();

	while (item != game_objects.end())
	{
		if ((*item)->active)
		{
			(*item)->Update();
		}
		item++;
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		debugRay = true;
		float distance;
		math::float3 hitPoint;
		selected_game_object = ScreenPointToRay(App->input->GetMouseX(), App->input->GetMouseY(), distance, hitPoint);
		game_camera = App->camera->editor_camera;

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
		if (!(*item)->Static)
			ret.push_back((*item));
		item++;
	}
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

GameObject* ModuleLevel::ScreenPointToRay(int posX, int posY, float& shortestDistance, math::float3& shortestHitPoint)
{
	GameObject* go = root;
	shortestHitPoint = math::float3::inf;
	shortestDistance = FLOAT_INF;

	//float sceneX = App->imGui->GetScenePos().x;
	//float sceneY = App->imGui->GetScenePos().y;

	//float normalized_x = 1.0f - (float(posX - sceneX) * 2.0f) / (App->window->window_w);
	//float normalized_y = 1.0f - (float(posY - sceneY) * 2.0f) / (App->window->window_h);

	float mouseX = (float)App->input->GetMouseX() - App->imGui->sceneX;
	LOG("%f", mouseX);
	float mouseY = (float)App->input->GetMouseY() - App->imGui->sceneY;
	LOG("%f", mouseY);

	mouseX = (mouseX / (App->imGui->sceneW / 2)) - 1;
	mouseY = (mouseY / (App->imGui->sceneH / 2)) - 1;
	ray = App->camera->editor_camera->frustum.UnProjectLineSegment(mouseX, -mouseY);


	// Static objects
	std::vector<GameObject*> hits;
	App->level->quad_tree->CollectIntersections(hits, ray);

	// Dynamic objects
	std::vector<GameObject*> dynamicGameObjects = GetNonStaticObjects();

	for (uint i = 0; i < dynamicGameObjects.size(); ++i)
	{
		if (dynamicGameObjects[i]->aligned_bounding_box.IsFinite() && ray.Intersects(dynamicGameObjects[i]->aligned_bounding_box))
			hits.push_back(dynamicGameObjects[i]);
	}

	for (int i = 0; i < hits.size(); ++i)
	{
		math::Triangle tri;
		math::LineSegment localSpaceSegment(ray);
		localSpaceSegment.Transform(hits[i]->global_transform_matrix.Inverted());

		const Mesh* mesh = (const Mesh*)hits[i]->GetComponent(MESH);

		if(mesh)
		for (int j = 0; j < mesh->num_index;)
		{
			triangles.push_back({ mesh->vertex[mesh->index[j] * 3], mesh->vertex[mesh->index[j] * 3 + 1], mesh->vertex[mesh->index[j] * 3 + 2] });
			triangles.push_back({ mesh->vertex[mesh->index[j+1] * 3], mesh->vertex[mesh->index[j + 1] * 3 + 1], mesh->vertex[mesh->index[j + 1] * 3 + 2] });
			triangles.push_back({ mesh->vertex[mesh->index[j + 2] * 3], mesh->vertex[mesh->index[j + 2] * 3 + 1], mesh->vertex[mesh->index[j + 2] * 3 + 2] });

			tri.a = math::float3(mesh->vertex[mesh->index[j] * 3], mesh->vertex[mesh->index[j] * 3 + 1], mesh->vertex[mesh->index[j] * 3 + 2]);  j++;
			tri.b = math::float3(mesh->vertex[mesh->index[j] * 3], mesh->vertex[mesh->index[j] * 3 + 1], mesh->vertex[mesh->index[j] * 3 + 2]);  j++;
			tri.c = math::float3(mesh->vertex[mesh->index[j] * 3], mesh->vertex[mesh->index[j] * 3 + 1], mesh->vertex[mesh->index[j] * 3 + 2]);  j++;

			float distance;
			math::float3 hitPoint;
			if (localSpaceSegment.Intersects(tri, &distance, &hitPoint))
			{
				if (shortestDistance > distance)
				{
					triangles.push_back({ tri.a.x, tri.a.y, tri.a.z });
					triangles.push_back({ tri.b.x, tri.b.y, tri.b.z });
					triangles.push_back({ tri.c.x, tri.c.y, tri.c.z });
					shortestDistance = distance;
					shortestHitPoint = hitPoint;
					go = hits[i];
				}
			}
		}
	}
	return go;
}