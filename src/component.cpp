#include "component.h"
#include <imgui/imgui.h>

double divCap(const double &val, const double &table)
{
	if (table > val)
	{
		return val / table * 100;
	}
	else
	{
		return table / val * 100;
	}
}

void drawComponentWidget(Component &component)
{
	static bool opened;

	ImGui::Begin("Component Info", &opened);

	for (auto const &[key, val] : component.values)
	{
		ImGui::Text("%s:", key.c_str());
		ImGui::SameLine();
		ImGui::Text("%f", val);

		if (key == "ShipLevel" || key == "ReverseLevel")
		{
			continue;
		}

		switch (component.type)
		{
		case Armor:
			ImGui::SameLine();
			ImGui::Text("Percentage:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "%f", divCap(val, armorTable[key][component.values["ShipLevel"]]));
			break;
		}
	}

	ImGui::End();
}