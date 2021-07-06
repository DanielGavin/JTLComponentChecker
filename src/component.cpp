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

static int crapCount = 1;

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
		capTable[Booster]["BoosterSpeed"][shipLevel] = speed;
	}

	//WEAPON
	std::string weaponPath = dataPath + "/" + "weapon.csv";
	io::CSVReader<11> weaponIn(weaponPath);
	weaponIn.read_header(io::ignore_extra_column, "ShipLevel", "Armor", "Hp", "ReactorDrain", "Mass", "MinDamage", "MaxDamage", "VsShields", "VsArmor", "EnergyShot", "RefireRate");

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

	//REACTOR
	std::string reactorPath = dataPath + "/" + "reactor.csv";
	io::CSVReader<5> reactorIn(reactorPath);
	reactorIn.read_header(io::ignore_extra_column, "ShipLevel", "Armor", "Hp", "Mass", "GenerationRate");

	double generationRate;

	while (reactorIn.read_row(shipLevel, armor, hp, mass, generationRate))
	{
		capTable[Reactor]["Armor"][shipLevel] = armor;
		capTable[Reactor]["Hp"][shipLevel] = hp;
		capTable[Reactor]["Mass"][shipLevel] = mass;
		capTable[Reactor]["ReactorGeneration"][shipLevel] = generationRate;
	}

	//CAPACITOR
	std::string capacitorPath = dataPath + "/" + "capacitor.csv";
	io::CSVReader<7> capacitorIn(capacitorPath);
	capacitorIn.read_header(io::ignore_extra_column, "ShipLevel", "Armor", "Hp", "ReactorDrain", "Mass", "Energy", "RechargeRate");

	double energy;

	while (capacitorIn.read_row(shipLevel, armor, hp, reactorDrain, mass, energy, rechargeRate))
	{
		capTable[Capacitor]["Armor"][shipLevel] = armor;
		capTable[Capacitor]["Hp"][shipLevel] = hp;
		capTable[Capacitor]["Mass"][shipLevel] = mass;
		capTable[Capacitor]["ReactorDrain"][shipLevel] = reactorDrain;
		capTable[Capacitor]["Energy"][shipLevel] = energy;
		capTable[Capacitor]["RechargeRate"][shipLevel] = rechargeRate;
	}

	//ENGINE
	std::string enginePath = dataPath + "/" + "engine.csv";
	io::CSVReader<9> engineIn(enginePath);
	engineIn.read_header(io::ignore_extra_column, "ShipLevel","Armor","Hp","ReactorDrain","Mass","Pitch","Yaw","Roll","TopSpeed");

	double yaw, roll, topSpeed, pitch;

	while (engineIn.read_row(shipLevel, armor, hp, reactorDrain, mass, pitch, yaw, roll, topSpeed))
	{
		capTable[Engine]["Armor"][shipLevel] = armor;
		capTable[Engine]["Hp"][shipLevel] = hp;
		capTable[Engine]["Mass"][shipLevel] = mass;
		capTable[Engine]["ReactorDrain"][shipLevel] = reactorDrain;
		capTable[Engine]["Yaw"][shipLevel] = yaw;
		capTable[Engine]["Pitch"][shipLevel] = pitch;
		capTable[Engine]["Roll"][shipLevel] = roll;
		capTable[Engine]["EngineTopSpeed"][shipLevel] = topSpeed;
	}

	//SHIELD
	std::string shieldPath = dataPath + "/" + "shield.csv";
	io::CSVReader<8> shieldIn(shieldPath);
	shieldIn.read_header(io::ignore_extra_column, "ShipLevel","Armor","Hp","ReactorDrain","Mass","FrontShield","BackShield","RechargeRate");

	double frontShield, backShield;

	while (shieldIn.read_row(shipLevel, armor, hp, reactorDrain, mass, frontShield, backShield, rechargeRate))
	{
		capTable[Shield]["Armor"][shipLevel] = armor;
		capTable[Shield]["Hp"][shipLevel] = hp;
		capTable[Shield]["Mass"][shipLevel] = mass;
		capTable[Shield]["ReactorDrain"][shipLevel] = reactorDrain;
		capTable[Shield]["FrontShieldHp"][shipLevel] = frontShield;
		capTable[Shield]["BackShieldHp"][shipLevel] = backShield;
		capTable[Shield]["RechargeRate"][shipLevel] = rechargeRate;
	}

	//DROID
	std::string droidPath = dataPath + "/" + "droid.csv";
	io::CSVReader<6> droidIn(droidPath);
	droidIn.read_header(io::ignore_extra_column, "ShipLevel","Armor","Hp","ReactorDrain","Mass","DroidCommandSpeed");

	double droidCommandSpeed;

	while (droidIn.read_row(shipLevel, armor, hp, reactorDrain, mass, droidCommandSpeed))
	{
		capTable[Droid]["Armor"][shipLevel] = armor;
		capTable[Droid]["Hp"][shipLevel] = hp;
		capTable[Droid]["Mass"][shipLevel] = mass;
		capTable[Droid]["ReactorDrain"][shipLevel] = reactorDrain;
		capTable[Droid]["DroidSpeed"][shipLevel] = droidCommandSpeed;
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
			double cap = divCap(val, capTable[component.type][key][(int)component.values["ShipLevel"]]);

			if (options.debug)
			{
				str = str + key + ": " + std::to_string(cap) + " - debug: " + std::to_string(val) + "\n";
			}
			else if (options.filterStat <= cap)
			{
				str = str + key + ": " + std::to_string(cap) + "\n";
			}
		}
	}

	if (str.empty() && component.values.size() > 0)
	{
		str = "Crap " + std::to_string(crapCount);
		crapCount += 1;
	}
	else 
	{
		crapCount = 1;
	}

	return str;
}