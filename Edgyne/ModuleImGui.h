#pragma once
#include "Module.h"
#include "pcg_variants.h"

class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void RandomNumberTest();
	void IntersectionsTest();
	//void Configuration();
	void HelpMenu();
	void About();

private:
	pcg32_random_t rng;
	int rng2 = 0;
	pcg32_random_t rngSeed3;
	float rng3 = 0;
	pcg_state_setseq_64 dunnoWhatThisIs;
	uint32_t bound;
	int tmpBoundMin = 0;
	int tmpBoundMax = 100;


public:
	bool show_demo_window = false;
	bool show_random_number_test = false;
	bool show_intersections_test = false;
	bool show_configuration = false;
	bool show_about_window = false;
	bool perma_true = true;
	bool perma_true_2 = true;
	bool to_close = false;
};

