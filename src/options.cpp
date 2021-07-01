#include "options.h"
#include <imgui/imgui.h>

void drawOptionsWidget(Options& options)
{
	ImGui::Begin("Options");
	ImGui::InputDouble("Stat filter", &options.filterStat);
	ImGui::ColorEdit3("Stat text color", options.statTextColor);
	ImGui::InputDouble("Stat text top margin", &options.statTextTopMargin);
	ImGui::Checkbox("Debug", &options.debug);
	ImGui::End();
}