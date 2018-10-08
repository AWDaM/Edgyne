#include "GUIRandomNumberTest.h"
#include "pcg_variants.h"

#include <time.h>


GUIRandomNumberTest::GUIRandomNumberTest() : GUIElements("random_number_test")
{
	posx = 420;
	posy = 200;
	width = 600;
	height = 300;
}


GUIRandomNumberTest::~GUIRandomNumberTest()
{
}

void GUIRandomNumberTest::Draw()
{

	//Random Number without Limits
	ImGui::Begin("PCG Random Test", &active);
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Normal Random Number");
	if (ImGui::Button("Generate Random Number"))
	{
		pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
	}
	ImGui::Text("%d", rng);


	//Random Number Between 0 And A Boundary
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Normal Random Number Between 0 and X");
	ImGui::InputInt("Min Number", &tmpBoundMin);
	ImGui::InputInt("Max Number", &tmpBoundMax);
	bound = tmpBoundMax - tmpBoundMin + 1;
	if (ImGui::Button("Generate Random Number With Boundaries"))
	{
		rng2 = pcg32_boundedrand_r(&dunnoWhatThisIs, bound);
		rng2 += tmpBoundMin;
	}
	ImGui::Text("%d", rng2);

	//Random Float Between 0 and 1
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Random Float Between 0 and 1");
	if (ImGui::Button("Generate Random Float Between 0 and 1"))
	{
		rng3 = ldexp(pcg32_random_r(&rngSeed3), -32);
	}
	ImGui::Text("%f", rng3);

	ImGui::End();
}
