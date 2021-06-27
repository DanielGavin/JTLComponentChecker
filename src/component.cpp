#include "component.h"
#include <imgui/imgui.h>






void componentWidget(const Component& component)
{
    static bool opened;

    ImGui::Begin("Component Info", &opened);

	for (auto const &[key, val] : component.values)
	{
		ImGui::Text(key.c_str()); 
		ImGui::SameLine(); 
		ImGui::Text("%f", val);
	}

    ImGui::End();
}