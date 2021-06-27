#pragma once

#include <map>

static std::map<int, float> armorArmor = { {1, 239.2f}, {2, 322.3f}, {3, 593.8f}, {4, 756.1f}, {5, 1109.5f}, {6, 1431.4f}, {7, 1647.5f}, {8, 2102.0f}, {9, 2135.0f}, {10, 2483.5f}};

static std::map<int, float> armorHp = { {1, 239.2f}, {2, 322.3f}, {3, 593.8f}, {4, 756.1f}, {5, 1109.5f}, {6, 1431.4f}, {7, 1647.5f}, {8, 2102.0f}, {9, 2135.0f}, {10, 2483.5f}};

static std::map<int, float> armorMass = { {1, 575.4f}, {2, 902.2f}, {3, 1138.6f}, {4, 1699.4f}, {5, 2046.1f}, {6, 3522.1f}, {7, 3926.4f}, {8, 5694.1f}, {9, 4244.5f}, {10, 5364.3f}};



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
	float armor;
	float hp;
	float reactorGeneration;
	float acceleration;
	float speed;
	float mass;
	float firingRate;
	float rechargeRate;
	float refireRate;
	float damageMin;
	float damageMax;
	float reverseLevel;
	float level;
	float reactorDrain;
	float capacitorEnergy;
	float vsShield;
	float vsArmor;
	float energyPerShot;
	float yaw;
	float pitch;
	float roll;
	float engineTopSpeed;
	float boosterEnergy;
	float boosterRecharge;
	float boosterConsumption;
	float boosterSpeed;
	float frontShieldHp;
	float backShieldHp;
	float droidSpeed;
};