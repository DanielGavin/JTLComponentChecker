#pragma once

#include <map>
#include <string>

static std::map<std::string, std::map<int, double>> armorTable = {
	{"Armor", { {1, 239.2}, {2, 322.3}, {3, 593.8}, {4, 756.1}, {5, 1109.5}, {6, 1431.4}, {7, 1647.5}, {8, 2102.0}, {9, 2135.0}, {10, 2483.5}}},
	{"Hp",    { {1, 239.2}, {2, 322.3}, {3, 593.8}, {4, 756.1}, {5, 1109.5}, {6, 1431.4}, {7, 1647.5}, {8, 2102.0}, {9, 2135.0}, {10, 2483.5}}},
	{"Mass",  { {1, 575.4}, {2, 902.2}, {3, 1138.6}, {4, 1699.4}, {5, 2046.1}, {6, 3522.1}, {7, 3926.4}, {8, 5694.1}, {9, 4244.5}, {10, 5364.3}}}
};

enum ComponentType {
	Engine,
	Capacitor,
	Weapon,
	Booster,
	Shield,
	Reactor,
	Armor,
	Droid
};

struct Component {
	ComponentType type;
	std::map<std::string, double> values;
};


void componentWidget(const Component& component);