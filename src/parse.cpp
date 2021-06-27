#include "parse.h"

float scanReal(const std::string &text, int &index)
{
	unsigned char current = text[index];

	while (isspace(current))
	{
		current = text[++index];
	}

	if (isdigit(current))
	{
		char *end;
		float v = strtof(&text[index], &end);
		uintptr_t diff = (uintptr_t)end - (uintptr_t)&text[index];
		index += diff;
		return v;
	}
	else
	{
		return 0;
	}
}

float scanRealDivisor(const std::string &text, int &index)
{
	scanReal(text, index);

	unsigned char current = text[index];

	while (isspace(current))
	{
		current = text[++index];
	}

	if (current == '/')
	{
		index += 1;
		return scanReal(text, index);
	}

	return 0;
}

void scanRealMinMax(const std::string &text, int &index, float &min, float &max)
{
	min = scanReal(text, index);

	unsigned char current = text[index];

	while (isspace(current))
	{
		current = text[++index];
	}

	if (current == '-')
	{
		index += 1;
		max = scanReal(text, index);
	}
}

void inferComponentType(Component& component)
{
	if (component.reactorGeneration)
	{
		component.type = ComponentType::Reactor;
	}
	else if (component.engineTopSpeed)
	{
		component.type = ComponentType::Engine;
	}
	else if (component.backShieldHp)
	{
		component.type = ComponentType::Shield;
	}
	else if (component.armor)
	{
		component.type = ComponentType::Armor;
	}
	else if (component.capacitorEnergy)
	{
		component.type = ComponentType::Capacitor;
	}
	else if (component.boosterRecharge)
	{
		component.type = ComponentType::Booster;
	}
	else if (component.energyPerShot)
	{
		component.type = ComponentType::Weapon;
	}
	else if (component.droidSpeed)
	{
		component.type = ComponentType::Droid;
	}
}

std::string popWordStack(std::vector<std::string> &wordStack)
{
	if (wordStack.size() < 2)
	{
		return "";
	}

	wordStack.pop_back();

	auto str = wordStack.back();
	
	return str;
}

Component parseComponent(const std::string &imageText)
{
	Component component = {0};
	unsigned char current = imageText[0];
	int index = 0;
	bool lastSpace = true;
	bool lastWord = false;
	int startWordIndex = 0;

	std::vector<std::string> wordStack;

	//We do tokenization and parsing at the same time
	while (current != '\0')
	{
		if (isspace(current) && lastWord)
		{
			wordStack.push_back(imageText.substr(startWordIndex, index - startWordIndex));

			//Have to check for level word because it is different than all other key value pairs
			if (wordStack.back() == "Level") 
			{
				while (isspace(current))
				{
					current = imageText[++index];
				}

				int start = index;

				while (isdigit(current)) {
					current = imageText[++index];
				}

				wordStack.push_back(imageText.substr(start, index - start));
			}

			lastWord = false;
		}

		while (isspace(current) || isdigit(current))
		{
			current = imageText[++index];
			lastSpace = true;
		}

		if (isalpha(current))
		{
			if (lastSpace)
			{
				startWordIndex = index;
			}
			lastWord = true;
			lastSpace = false;
		}

		else if (current == ':')
		{
			if (lastWord)
			{
				wordStack.push_back(imageText.substr(startWordIndex, index - startWordIndex));
				lastWord = false;
			}

			current = imageText[++index];

			if (wordStack.size() != 0)
			{
				std::string back = wordStack.back();

				if (back == "Certification")
				{
					std::string secondBack = popWordStack(wordStack);

					if (secondBack == "Equipment")
					{
						std::string thirdBack = popWordStack(wordStack);

						if (thirdBack == "Ship")
						{
							std::string fourthBack = popWordStack(wordStack);
							component.level = std::stof(fourthBack);
						}
					}

				}

				else if (back == "Pitch")
				{
					component.pitch = scanReal(imageText, index);
				}
				else if (back == "Yaw")
				{
					component.yaw = scanReal(imageText, index);
				}
				else if (back == "Roll")
				{
					component.roll = scanReal(imageText, index);
				}
				else if (back == "Speed")
				{
					std::string secondBack = popWordStack(wordStack);

					if (secondBack == "Top")
					{
						component.engineTopSpeed = scanReal(imageText, index);
					}
					else if (secondBack == "Booster")
					{
						component.boosterSpeed = scanReal(imageText, index);
					}
				}
				else if (back == "Energy")
				{
					std::string secondBack = popWordStack(wordStack);

					if (secondBack == "Booster")
					{
						component.boosterEnergy = scanReal(imageText, index);
					}
					else if (secondBack == "Capacitor")
					{
						component.capacitorEnergy = scanReal(imageText, index);
					}
				}
				else if (back == "Armor")
				{
					component.armor = scanRealDivisor(imageText, index);
				}
				else if (back == "Hitpoints")
				{
					std::string secondBack = popWordStack(wordStack);
					
					if (secondBack == "Shield")
					{
						std::string thirdBack = popWordStack(wordStack);

						if (thirdBack == "Front")
						{
							component.frontShieldHp = scanRealDivisor(imageText, index);
						}
						else if (thirdBack == "Back")
						{
							component.backShieldHp = scanRealDivisor(imageText, index);
						}
					}
					else {
						component.hp = scanReal(imageText, index);
					}
				}
				else if (back == "Mass")
				{
					component.mass = scanReal(imageText, index);
				}
				else if (back == "Rate")
				{
					std::string secondBack = popWordStack(wordStack);
					if (secondBack == "Refire")
					{
						component.refireRate = scanReal(imageText, index);
					}
					else if (secondBack == "Recharge")
					{
						std::string thirdBack = popWordStack(wordStack);
						component.rechargeRate = scanReal(imageText, index);
					}
					else if (secondBack == "Firing")
					{
						component.firingRate = scanReal(imageText, index);
					}
					else if (secondBack == "Generation")
					{
						component.reactorGeneration = scanReal(imageText, index);
					}
				}
				else if (back == "Speed")
				{
					component.speed = scanReal(imageText, index);
				}
				else if (back == "Acceleration")
				{
					component.acceleration = scanReal(imageText, index);
				}
				else if (back == "Damage")
				{
					scanRealMinMax(imageText, index, component.damageMin, component.damageMax);
				}
				else if (back == "Level")
				{
					std::string secondBack = popWordStack(wordStack);

					if (secondBack == "Engineering")
					{
						component.reverseLevel = scanReal(imageText, index);
					}
				}
				else if (back == "Drain")
				{
					component.reactorDrain = scanReal(imageText, index);
				}

				wordStack.clear();
			}

			lastWord = false;
			lastSpace = true;
		}
		current = imageText[++index];
	}

	inferComponentType(component);

	return component;
}