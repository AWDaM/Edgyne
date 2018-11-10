#include "Application.h"
#include "ModuleDebug.h"
#include "Camera.h"
#include "SDL\include\SDL_opengl.h"
#include <cmath>
#include <vector>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "GL/lib/glew32.lib")

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(start_enabled)
{
	name = "Debug";
}


ModuleDebug::~ModuleDebug()
{
}

bool ModuleDebug::Init(rapidjson::Value& node)
{
	return true;
}

bool ModuleDebug::Start()
{
	Vertex_Array_Cube();
	Indices_Array_Cube();

	const int CHECKERS_HEIGHT = 100;
	const int CHECKERS_WIDTH = 100;
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void ModuleDebug::Save(rapidjson::Document & doc, rapidjson::FileWriteStream & os)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	
	rapidjson::Value obj(rapidjson::kObjectType);
	obj.AddMember("draw_wireframe", draw_wireframe, allocator);
	obj.AddMember("direct_mode_cube", direct_mode_cube, allocator);
	obj.AddMember("vertex_cube", vertex_cube, allocator);
	obj.AddMember("indices_cube", indices_cube, allocator);
	obj.AddMember("sphere", sphere, allocator);
	obj.AddMember("draw_plane", draw_plane, allocator);
	obj.AddMember("draw_axis", draw_axis, allocator);
	obj.AddMember("draw_normals", draw_normals, allocator);
	obj.AddMember("draw_meshBoundingBox", draw_meshBoundingBox, allocator);

	doc.AddMember((rapidjson::Value::StringRefType)name.data(), obj, allocator);

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
}

void ModuleDebug::Load(rapidjson::Document& doc)
{
	rapidjson::Value& node = doc[name.data()];

	draw_wireframe = node["draw_wireframe"].GetBool();
	if (draw_wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	direct_mode_cube = node["direct_mode_cube"].GetBool();
	vertex_cube = node["vertex_cube"].GetBool();
	indices_cube = node["indices_cube"].GetBool();
	sphere = node["sphere"].GetBool();
	draw_plane = node["draw_plane"].GetBool();
	draw_axis = node["draw_axis"].GetBool();
	draw_normals = node["draw_normals"].GetBool();
	draw_meshBoundingBox = node["draw_meshBoundingBox"].GetBool();

}

update_status ModuleDebug::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleDebug::Draw()
{

	glLineWidth(3.0f);


	if (direct_mode_cube)
	{
		Draw_Cube_Direct_Mode();
	}
	if (draw_plane)
		Draw_Plane();
	if (draw_axis)
		Draw_Axis();
	if(sphere)
		Draw_Sphere();

	if(vertex_cube)
	Draw_Cube_Vertex();


	if(indices_cube)
	Draw_Cube_Indices();
		

	glColor3f(1.0f, 1.0, 1.0);
	
}

void ModuleDebug::Configuration()
{
	if (ImGui::CollapsingHeader("Debug"))
	{
		if (ImGui::Checkbox("WireframeRender", &draw_wireframe))
		{
			if (draw_wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		ImGui::Checkbox("Direct Mode Cube", &direct_mode_cube);
		ImGui::Checkbox("Vertex Cube", &vertex_cube);
		ImGui::Checkbox("Indices Cube", &indices_cube);
		ImGui::Checkbox("Sphere", &sphere);
		ImGui::Checkbox("Plane", &draw_plane);
		ImGui::Checkbox("Axis", &draw_axis);
		ImGui::Checkbox("Mesh Normals", &draw_normals);
		ImGui::Checkbox("Mesh Bounding Box", &draw_meshBoundingBox);
	}
}

void ModuleDebug::Draw_Cube_Direct_Mode()
{
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glBegin(GL_TRIANGLES);

	glTexCoord2f(1, 1);
	glVertex3f(1, 0, 1); //v3
	glTexCoord2f(0, 1);
	glVertex3f(-1, 0, 1); //v2
	glTexCoord2f(0, 0);
	glVertex3f(-1, 0, -1); // v7

	glTexCoord2f(0, 0);
	glVertex3f(-1, 0, -1); 
	glTexCoord2f(1, 0);
	glVertex3f(1, 0, -1); //v4
	glTexCoord2f(1, 1);
	glVertex3f(1, 0, 1);

	glTexCoord2f(0, 0);
	glVertex3f(-1, 0, 1); //v2
	glTexCoord2f(1, 0);
	glVertex3f(1, 0, 1);//v3
	glTexCoord2f(1, 1);
	glVertex3f(1, 2, 1); //v0

	glTexCoord2f(0, 0);
	glVertex3f(-1, 0, 1); //v2
	glTexCoord2f(1, 1);
	glVertex3f(1, 2, 1); //v0
	glTexCoord2f(0, 1);
	glVertex3f(-1, 2, 1); //v1

	glTexCoord2f(0, 1);
	glVertex3f(1, 2, 1); //vo
	glTexCoord2f(0, 0);
	glVertex3f(1, 0, 1); //v3
	glTexCoord2f(1, 1);
	glVertex3f(1, 2, -1); //v5

	glTexCoord2f(1, 1);
	glVertex3f(1, 2, -1);
	glTexCoord2f(0, 0);
	glVertex3f(1, 0, 1);
	glTexCoord2f(1, 0);
	glVertex3f(1, 0, -1);

	glTexCoord2f(0, 0);
	glVertex3f(-1, 2, 1);
	glTexCoord2f(1, 1);
	glVertex3f(1, 2, -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1, 2, -1);

	glTexCoord2f(1, 0);
	glVertex3f(1, 2, 1);
	glTexCoord2f(1, 1);
	glVertex3f(1, 2, -1);
	glTexCoord2f(0, 0);
	glVertex3f(-1, 2, 1);

	glTexCoord2f(1, 1);
	glVertex3f(-1, 2, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-1, 0, -1);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 0, 1);

	glTexCoord2f(0,1);
	glVertex3f(-1, 2, -1);
	glTexCoord2f(0,0);
	glVertex3f(-1, 0, -1);
	glTexCoord2f(1, 1);
	glVertex3f(-1, 2, 1);

	glTexCoord2f(1, 1);
	glVertex3f(-1, 2, -1);
	glTexCoord2f(0, 1);
	glVertex3f(1, 2, -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1, 0, -1);

	glTexCoord2f(0,0);
	glVertex3f(1, 0, -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1, 0, -1);
	glTexCoord2f(0, 1);
	glVertex3f(1, 2, -1);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleDebug::Draw_Plane()
{
	glBegin(GL_LINES);
	//-----Is this a plane?

	glColor3f(0.5f, 0.5f, 0.5f);
	float d = 20.0f;
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
}

void ModuleDebug::Draw_Axis()
{
	glLineWidth(3.0f);
	
	glBegin(GL_LINES);

	glColor3f(1.0f, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3f(0.f, 1.0f, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor3f(0.f, 0.f, 1.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	glEnd();
	glLineWidth(1.0f);
}

void ModuleDebug::Draw_Cube_Vertex()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}


void ModuleDebug::Draw_Sphere()
{


	//TEMP
	if (toDrawSphere)
	{
		vec initialPos = pos;

		float x, y, z, xz;                              // vertex position

		float sectorStep = 2 * pi / sectors;
		float stackStep = pi / rings;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= rings; ++i)
		{
			stackAngle = pi / 2 - i * stackStep;					// starting from pi/2 to -pi/2
			xz = radius * cosf(stackAngle);							// r * cos(u)
			y = initialPos.y + radius * sinf(stackAngle);			// r * sin(u)

																	// add (sectorCount+1) vertices per stack
																	// the first and last vertices have same position and normal, but different tex coods
			for (int j = 0; j <= sectors; ++j)
			{
				sectorAngle = j * sectorStep;

				// vertex position (x, y, z)
				z = initialPos.z + xz * cosf(sectorAngle);			// r * cos(u) * cos(v)
				x = initialPos.x + xz * sinf(sectorAngle);			// r * cos(u) * sin(v)
				shape.push_back(x);
				shape.push_back(y);
				shape.push_back(z);
			}
		}

		int k1, k2;
		for (int i = 0; i < rings; ++i)
		{
			k1 = i * (sectors + 1);     // beginning of current stack
			k2 = k1 + sectors + 1;      // beginning of next stack

			for (int j = 0; j < sectors; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding 1st and last stacks
				if (i != 0)
				{
					indicesS.push_back(k1);
					indicesS.push_back(k2);
					indicesS.push_back(k1 + 1);
				}

				if (i != (rings - 1))
				{
					indicesS.push_back(k1 + 1);
					indicesS.push_back(k2);
					indicesS.push_back(k2 + 1);
				}
			}
		}

		glGenBuffers(1, (GLuint*)&(my_id));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indicesS.size(), &indicesS[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		toDrawSphere = false;
	}
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, &shape[0]);
	glDrawElements(GL_TRIANGLES, indicesS.size(), GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}


void ModuleDebug::Draw_Cube_Indices()
{
	glColor3f(1.0f, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id_array);

	glVertexPointer(3, GL_FLOAT, 0, &cube_indices_array[0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

}

void ModuleDebug::Draw_Camera(Camera * camera)
{
	glLineWidth(4.0f);
	glColor3f(1, 0, 1);
	glBegin(GL_LINES);

	glVertex3f(camera->frustum.CornerPoint(0).x, camera->frustum.CornerPoint(0).y, camera->frustum.CornerPoint(0).z);
	glVertex3f(camera->frustum.CornerPoint(1).x, camera->frustum.CornerPoint(1).y, camera->frustum.CornerPoint(1).z);

	glVertex3f(camera->frustum.CornerPoint(0).x, camera->frustum.CornerPoint(0).y, camera->frustum.CornerPoint(0).z);
	glVertex3f(camera->frustum.CornerPoint(2).x, camera->frustum.CornerPoint(2).y, camera->frustum.CornerPoint(2).z);

	glVertex3f(camera->frustum.CornerPoint(0).x, camera->frustum.CornerPoint(0).y, camera->frustum.CornerPoint(0).z);
	glVertex3f(camera->frustum.CornerPoint(4).x, camera->frustum.CornerPoint(4).y, camera->frustum.CornerPoint(4).z);

	glVertex3f(camera->frustum.CornerPoint(7).x, camera->frustum.CornerPoint(7).y, camera->frustum.CornerPoint(7).z);
	glVertex3f(camera->frustum.CornerPoint(6).x, camera->frustum.CornerPoint(6).y, camera->frustum.CornerPoint(6).z);

	glVertex3f(camera->frustum.CornerPoint(6).x, camera->frustum.CornerPoint(6).y, camera->frustum.CornerPoint(6).z);
	glVertex3f(camera->frustum.CornerPoint(2).x, camera->frustum.CornerPoint(2).y, camera->frustum.CornerPoint(2).z);

	glVertex3f(camera->frustum.CornerPoint(7).x, camera->frustum.CornerPoint(7).y, camera->frustum.CornerPoint(7).z);
	glVertex3f(camera->frustum.CornerPoint(5).x, camera->frustum.CornerPoint(5).y, camera->frustum.CornerPoint(5).z);

	glVertex3f(camera->frustum.CornerPoint(7).x, camera->frustum.CornerPoint(7).y, camera->frustum.CornerPoint(7).z);
	glVertex3f(camera->frustum.CornerPoint(3).x, camera->frustum.CornerPoint(3).y, camera->frustum.CornerPoint(3).z);

	glVertex3f(camera->frustum.CornerPoint(3).x, camera->frustum.CornerPoint(3).y, camera->frustum.CornerPoint(3).z);
	glVertex3f(camera->frustum.CornerPoint(1).x, camera->frustum.CornerPoint(1).y, camera->frustum.CornerPoint(1).z);

	glVertex3f(camera->frustum.CornerPoint(1).x, camera->frustum.CornerPoint(1).y, camera->frustum.CornerPoint(1).z);
	glVertex3f(camera->frustum.CornerPoint(5).x, camera->frustum.CornerPoint(5).y, camera->frustum.CornerPoint(5).z);

	glVertex3f(camera->frustum.CornerPoint(3).x, camera->frustum.CornerPoint(3).y, camera->frustum.CornerPoint(3).z);
	glVertex3f(camera->frustum.CornerPoint(2).x, camera->frustum.CornerPoint(2).y, camera->frustum.CornerPoint(2).z);

	glVertex3f(camera->frustum.CornerPoint(4).x, camera->frustum.CornerPoint(4).y, camera->frustum.CornerPoint(4).z);
	glVertex3f(camera->frustum.CornerPoint(5).x, camera->frustum.CornerPoint(5).y, camera->frustum.CornerPoint(5).z);

	glVertex3f(camera->frustum.CornerPoint(6).x, camera->frustum.CornerPoint(6).y, camera->frustum.CornerPoint(6).z);
	glVertex3f(camera->frustum.CornerPoint(4).x, camera->frustum.CornerPoint(4).y, camera->frustum.CornerPoint(4).z);

	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
}

void ModuleDebug::Draw_AABB(const AABB & box)
{
	glLineWidth(2.0f);
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);

	glVertex3f(box.CornerPoint(0).x, box.CornerPoint(0).y, box.CornerPoint(0).z);
	glVertex3f(box.CornerPoint(1).x, box.CornerPoint(1).y, box.CornerPoint(1).z);
					 																	 
	glVertex3f(box.CornerPoint(0).x, box.CornerPoint(0).y, box.CornerPoint(0).z);
	glVertex3f(box.CornerPoint(2).x, box.CornerPoint(2).y, box.CornerPoint(2).z);
																						 
	glVertex3f(box.CornerPoint(0).x, box.CornerPoint(0).y, box.CornerPoint(0).z);
	glVertex3f(box.CornerPoint(4).x, box.CornerPoint(4).y, box.CornerPoint(4).z);
																						 
	glVertex3f(box.CornerPoint(7).x, box.CornerPoint(7).y, box.CornerPoint(7).z);
	glVertex3f(box.CornerPoint(6).x, box.CornerPoint(6).y, box.CornerPoint(6).z);
																						 
	glVertex3f(box.CornerPoint(6).x, box.CornerPoint(6).y, box.CornerPoint(6).z);
	glVertex3f(box.CornerPoint(2).x, box.CornerPoint(2).y, box.CornerPoint(2).z);
																						 
	glVertex3f(box.CornerPoint(7).x, box.CornerPoint(7).y, box.CornerPoint(7).z);
	glVertex3f(box.CornerPoint(5).x, box.CornerPoint(5).y, box.CornerPoint(5).z);
					 								   									 
	glVertex3f(box.CornerPoint(7).x, box.CornerPoint(7).y, box.CornerPoint(7).z);
	glVertex3f(box.CornerPoint(3).x, box.CornerPoint(3).y, box.CornerPoint(3).z);
																						 
	glVertex3f(box.CornerPoint(3).x, box.CornerPoint(3).y, box.CornerPoint(3).z);
	glVertex3f(box.CornerPoint(1).x, box.CornerPoint(1).y, box.CornerPoint(1).z);
																						 
	glVertex3f(box.CornerPoint(1).x, box.CornerPoint(1).y, box.CornerPoint(1).z);
	glVertex3f(box.CornerPoint(5).x, box.CornerPoint(5).y, box.CornerPoint(5).z);
					  																	 
	glVertex3f(box.CornerPoint(3).x, box.CornerPoint(3).y, box.CornerPoint(3).z);
	glVertex3f(box.CornerPoint(2).x, box.CornerPoint(2).y, box.CornerPoint(2).z);
					  																	 
	glVertex3f(box.CornerPoint(4).x, box.CornerPoint(4).y, box.CornerPoint(4).z);
	glVertex3f(box.CornerPoint(5).x, box.CornerPoint(5).y, box.CornerPoint(5).z);
					  																	 
	glVertex3f(box.CornerPoint(6).x, box.CornerPoint(6).y, box.CornerPoint(6).z);
	glVertex3f(box.CornerPoint(4).x, box.CornerPoint(4).y, box.CornerPoint(4).z);

	glEnd();
	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
}


void ModuleDebug::Vertex_Array_Cube()
{
	//1
	cube_vertex_array = {
	4, 0, 1,
	2, 0, 1,
	2, 0, -1,

	2, 0, -1,
	4, 0, -1,
	4, 0, 1,

	2, 0, 1,
	4, 0, 1,
	4, 2, 1,

	2, 0, 1,
	4, 2, 1,
	2, 2, 1,

	4, 2, 1,
	4, 0, 1,
	4, 2, -1,

	4, 2, -1,
	4, 0, 1,
	4, 0, -1,

	2, 2, 1,
	4, 2, -1,
	2, 2, -1,

	4, 2, 1,
	4, 2, -1,
	2, 2, 1,

	2, 2, 1,
	2, 0, -1,
	2, 0, 1,

	2, 2, -1,
	2, 0, -1,
	2, 2, 1,

	2, 2, -1,
	4, 2, -1,
	2, 0, -1,

	4, 0, -1,
	2, 0, -1,
	4, 2, -1 };

	glGenBuffers(1, (GLuint*)&(my_id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, my_id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, &cube_vertex_array[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleDebug::Indices_Array_Cube()
{
	cube_indices_array = {
		-2, 0, 1,//B 0

		-4, 0, 1,//A 1

		-4, 0, -1, //E 2

		-2, 0, -1,//F 3

		-2, 2, 1, //D 4

		-4, 2, 1,//C 5

		-2, 2, -1,//H 6

		-4, 2, -1 }; //G 7

	cube_indices_indexes = {
		1,0,5,   5,0,4,   2,0,1,   2,3,0,   3,6,4,   3,4,0,   2,5,7,   5,2,1,   7,3,2,   7,6,3,   4,6,7,   7,5,4
	};
	glGenBuffers(1, (GLuint*)&(my_id_array));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id_array);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*36, &cube_indices_indexes[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
}
