#include "options.h"
#include <imgui/imgui.h>

void drawOptionsWidget(Options& options)
{
	ImGui::Begin("Options");
	ImGui::InputText("Execute Key", options.executeKey, 2);
	ImGui::Checkbox("Execute shift modifier", &options.executeKeyShiftModifier);
	ImGui::InputText("Record Key", options.recordKey, 2);
	ImGui::Checkbox("Record shift modifier", &options.recordKeyShiftModifier);
	ImGui::InputDouble("Stat filter", &options.filterStat);
	ImGui::ColorEdit3("Stat text color", options.statTextColor);
	ImGui::InputDouble("Stat text top margin", &options.statTextTopMargin);
	ImGui::Checkbox("Debug", &options.debug);
	ImGui::End();
}