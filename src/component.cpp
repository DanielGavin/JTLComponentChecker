#include "component.h"
#include "options.h"

#include <imgui/imgui.h>

#pragma warning(push)
#pragma warning(disable : 4267)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include "csv.h"
#pragma warning(pop)

#include <iostream>

extern Options options;


static std::map<int, std::map<std::string, std::map<int, double>>> capTable;

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

void readComponentCapData(std::string dataPath)
{

	//TODO look at the new cpp filepath handling

	//ARMOR

	std::string armorPath = dataPath + "/" + "armor.csv";
	io::CSVReader<4> armorIn(armorPath);
	armorIn.read_header(io::ignore_extra_column, "ShipLevel", "Armor", "Hp", "Mass");

	int shipLevel;
	double armor;
	double hp;
	double mass;

	while (armorIn.read_row(shipLevel, armor, hp, mass))
	{
		capTable[Armor]["Armor"][shipLevel] = armor;
		capTable[Armor]["Hp"][shipLevel] = hp;
		capTable[Armor]["Mass"][shipLevel] = mass;
	}

	//BOOSTER

	std::string boosterPath = dataPath + "/" + "booster.csv";
	io::CSVReader<10> boosterIn(boosterPath);
	boosterIn.read_header(io::ignore_extra_column, "ShipLevel", "Armor", "Hp", "ReactorDrain", "Mass", "BoosterEnergy", "RechargeRate", "ConsumptionRate", "Acceleration", "Speed");

	double reactorDrain, boosterEnergy, rechargeRate, consumptionRate, acceleration, speed;

	while (boosterIn.read_row(shipLevel, armor, hp, reactorDrain, mass, boosterEnergy, rechargeRate, consumptionRate, acceleration, speed))
	{
		capTable[Booster]["Armor"][shipLevel] = armor;
		capTable[Booster]["Hp"][shipLevel] = hp;
		capTable[Booster]["Mass"][shipLevel] = mass;
		capTable[Booster]["ReactorDrain"][shipLevel] = reactorDrain;
		capTable[Booster]["BoosterEnergy"][shipLevel] = boosterEnergy;
		capTable[Booster]["RechargeRate"][shipLevel] = rechargeRate;
		capTable[Booster]["ConsumptionRate"][shipLevel] = consumptionRate;
		capTable[Booster]["Acceleration"][shipLevel] = acceleration;
		capTable[Booster]["Speed"][shipLevel] = speed;
	}

	//WEAPON
	std::string weaponPath = dataPath + "/" + "weapon.csv";
	io::CSVReader<11> weaponIn(weaponPath);
	weaponIn.read_header(io::ignore_extra_column, "ShipLevel","Armor","Hp","ReactorDrain","Mass","MinDamage","MaxDamage","VsShields","VsArmor","EnergyShot","RefireRate");

	double minDamage, maxDamage, vsShields, vsArmor, energyShot, refireRate;

	while (weaponIn.read_row(shipLevel, armor, hp, reactorDrain, mass, minDamage, maxDamage, vsShields, vsArmor, energyShot, refireRate))
	{
		capTable[Weapon]["Armor"][shipLevel] = armor;
		capTable[Weapon]["Hp"][shipLevel] = hp;
		capTable[Weapon]["Mass"][shipLevel] = mass;
		capTable[Weapon]["ReactorDrain"][shipLevel] = reactorDrain;
		capTable[Weapon]["MaxDamage"][shipLevel] = maxDamage;
		capTable[Weapon]["MinDamage"][shipLevel] = minDamage;
		capTable[Weapon]["VsShields"][shipLevel] = vsShields;
		capTable[Weapon]["VsArmor"][shipLevel] = vsArmor;
		capTable[Weapon]["Energy/Shot"][shipLevel] = energyShot;
		capTable[Weapon]["RefireRate"][shipLevel] = refireRate;
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

		if (capTable.find(component.type) != capTable.end())
		{
			double cap = divCap(val, capTable[component.type][key][component.values["ShipLevel"]]);

			if (options.debug)
			{
				str = str + key + ": " + std::to_string(val) + "\n";
			}
			else if (options.filterStat <= cap)
			{
				str = str + key + ": " + std::to_string(cap) + "\n";
			}
		}
	}

	return str;
}