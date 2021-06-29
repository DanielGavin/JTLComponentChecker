#include "options.h"
#include <imgui/imgui.h>

void drawOptionsWidget()
{
	double test;
	ImGui::Begin("Options");
	ImGui::InputDouble("Emit sound on percentile", &test);
	ImGui::End();
}