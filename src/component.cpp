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


std::string buildComponentString(Component &component)
{
	std::string str;

	for (auto const &[key, val] : component.values)
	{
		if (key == "ShipLevel" || key == "ReverseLevel")
		{
			continue;
		}

		switch (component.type)
		{
		case Armor:
			double cap = divCap(val, armorTable[key][component.values["ShipLevel"]]);
			str = str + key + ": " + std::to_string(cap) + "\n";
			break;
		}
	}

	return str;
}