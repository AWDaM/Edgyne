#pragma once
#include "GUIElements.h"
#include "pcg_variants.h"

class GUIRandomNumberTest :
	public GUIElements
{
public:
	GUIRandomNumberTest();
	~GUIRandomNumberTest();

	void Draw();
	bool Save(rapidjson::Value & Node, rapidjson::Document::AllocatorType& allocator);

private:
public:
private:
	pcg32_random_t rng;
	int rng2 = 0;
	pcg32_random_t rngSeed3;
	float rng3 = 0;
	pcg_state_setseq_64 dunnoWhatThisIs;
	uint32_t bound;
	int tmpBoundMin = 0;
	int tmpBoundMax = 100;
};

