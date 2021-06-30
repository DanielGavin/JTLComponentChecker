#include "component.h"
#include <imgui/imgui.h>

#pragma warning(push)
#pragma warning(disable : 4267)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include "csv.h"
#pragma warning(pop)

#include <iostream>

static std::map<std::string, std::map<int, double>> armorTable;
static std::map<std::string, std::map<int, double>> boosterTable;
static std::map<std::string, std::map<int, double>> weaponTable;
static std::map<std::string, std::map<int, double>> capacitorTable;

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
		armorTable["Armor"][shipLevel] = armor;
		armorTable["Hp"][shipLevel] = hp;
		armorTable["Mass"][shipLevel] = mass;
	}

	//BOOSTER

	std::string boosterPath = dataPath + "/" + "booster.csv";
	io::CSVReader<10> boosterIn(boosterPath);
	boosterIn.read_header(io::ignore_extra_column, "ShipLevel", "Armor", "Hp", "ReactorDrain", "Mass", "BoosterEnergy", "RechargeRate", "ConsumptionRate", "Acceleration", "Speed");

	double reactorDrain, boosterEnergy, rechargeRate, consumptionRate, acceleration, speed;

	while (boosterIn.read_row(shipLevel, armor, hp, reactorDrain, mass, boosterEnergy, rechargeRate, consumptionRate, acceleration, speed))
	{
		boosterTable["Armor"][shipLevel] = armor;
		boosterTable["Hp"][shipLevel] = hp;
		boosterTable["Mass"][shipLevel] = mass;
		boosterTable["ReactorDrain"][shipLevel] = reactorDrain;
		boosterTable["BoosterEnergy"][shipLevel] = boosterEnergy;
		boosterTable["RechargeRate"][shipLevel] = rechargeRate;
		boosterTable["ConsumptionRate"][shipLevel] = consumptionRate;
		boosterTable["Acceleration"][shipLevel] = acceleration;
		boosterTable["Speed"][shipLevel] = speed;
	}

	//WEAPON
	std::string weaponPath = dataPath + "/" + "weapon.csv";
	io::CSVReader<11> weaponIn(weaponPath);
	weaponIn.read_header(io::ignore_extra_column, "ShipLevel","Armor","Hp","ReactorDrain","Mass","MinDamage","MaxDamage","VsShields","VsArmor","EnergyShot","RechargeRate");

	double minDamage, maxDamage, vsShields, vsArmor, energyShot;

	while (weaponIn.read_row(shipLevel, armor, hp, reactorDrain, mass, minDamage, maxDamage, vsShields, vsArmor, energyShot, rechargeRate))
	{
		weaponTable["Armor"][shipLevel] = armor;
		weaponTable["Hp"][shipLevel] = hp;
		weaponTable["Mass"][shipLevel] = mass;
		weaponTable["ReactorDrain"][shipLevel] = reactorDrain;
		weaponTable["MaxDamage"][shipLevel] = maxDamage;
		weaponTable["MinDamage"][shipLevel] = minDamage;
		weaponTable["VsShields"][shipLevel] = vsShields;
		weaponTable["VsArmor"][shipLevel] = vsArmor;
		weaponTable["Energy/Shot"][shipLevel] = energyShot;
		weaponTable["RefireRate"][shipLevel] = rechargeRate;
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
		{
			double cap = divCap(val, armorTable[key][component.values["ShipLevel"]]);
			str = str + key + ": " + std::to_string(cap) + "\n";
			break;
		}
		case Weapon:
		{
			double cap = divCap(val, weaponTable[key][component.values["ShipLevel"]]);
			str = str + key + ": " + std::to_string(cap) + "\n";
			break;
		}
		}
	}

	return str;
}