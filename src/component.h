#pragma once

#include <map>
#include <vector>
#include <string>

enum ComponentType
{
	Engine,
	Capacitor,
	Weapon,
	Booster,
	Shield,
	Reactor,
	Armor,
	Droid
};

struct Component
{
	ComponentType type;
	std::map<std::string, double> values;
};


void readComponentCapData(std::string dataPath);


std::string buildComponentString(Component &component);
