#include "help.h"
#include <imgui/imgui.h>

static bool open = true;

void drawHelpWidget()
{
	ImGui::Begin("Help", &open);
	ImGui::Text("To use this application, you need to specify which region to read from by clicking the capture key on the top left and bottom right that forms a rectangle that incompasses all the ship stats.");
	ImGui::Text("Click the execute button to start analyzing the ship component. The result is filtered through the option settingsm and only the stats that meet the filter requirement is shown.");
	ImGui::NewLine();
	ImGui::Text("All the data that is used to compare against the looted component data is located in data folder as csv files.");
	ImGui::NewLine();
	ImGui::Text("Note: the text recognition might take some time to complete - normally 2-4 seconds. This hopefully can be improved without loss of accuracy.");
	ImGui::End();
}