#include "parse.h"

double scanReal(const std::string &text, int &index)
{
	unsigned char current = text[index];

	while (isspace(current))
	{
		current = text[++index];
	}

	if (isdigit(current))
	{
		char *end;
		double v = strtod(&text[index], &end);
		uintptr_t diff = (uintptr_t)end - (uintptr_t)&text[index];
		index += diff;
		return v;
	}
	else
	{
		return 0;
	}
}

double scanRealDivisor(const std::string &text, int &index)
{
	scanReal(text, index);

	unsigned char current = text[index];

	while (isspace(current))
	{
		current = text[++index];
	}

	//Sometimes tesseract thinks there are commas with slashes, if this only happens on slahes, then this should fix it, otherwise improve the preprocessing.
	if (current == ',' || current == '.')
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

void scanRealMinMax(const std::string &text, int &index, double &min, double &max)
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

void ValidateComponent(Component& component)
{
	//TODO(add range checks to see that all the input actually makes sense in relation to caps)

	if (component.values.count("Reactor Generation"))
	{
		component.type = ComponentType::Reactor;
	}
	else if (component.values.count("EngineTopSpeed"))
	{
		component.type = ComponentType::Engine;
	}
	else if (component.values.count("BackShieldHp"))
	{
		component.type = ComponentType::Shield;
	}
	else if (component.values.count("CapacitorEnergy"))
	{
		component.type = ComponentType::Capacitor;
	}
	else if (component.values.count("BoosterSpeed"))
	{
		component.type = ComponentType::Booster;
	}
	else if (component.values.count("RefireRate"))
	{
		component.type = ComponentType::Weapon;
	}
	else if (component.values.count("DroidSpeed"))
	{
		component.type = ComponentType::Droid;
	}
	else if (component.values.count("Armor"))
	{
		component.type = ComponentType::Armor;
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
							if (fourthBack != "")
							{
								component.values["ShipLevel"] = std::stof(fourthBack);
							}
						}
					}

				}

				else if (back == "Pitch")
				{
					component.values["Pitch"] = scanReal(imageText, index);
				}
				else if (back == "Yaw")
				{
					component.values["Yaw"] = scanReal(imageText, index);
				}
				else if (back == "Roll")
				{
					component.values["Roll"] = scanReal(imageText, index);
				}
				else if (back == "Speed")
				{
					std::string secondBack = popWordStack(wordStack);

					if (secondBack == "Top")
					{
						component.values["EngineTopSpeed"] = scanReal(imageText, index);
					}
					else if (secondBack == "Booster")
					{
						component.values["BoosterSpeed"] = scanReal(imageText, index);
					}
					else if (secondBack == "Command")
					{
						component.values["DroidSpeed"] = scanReal(imageText, index);
					}
				}
				else if (back == "Energy")
				{
					std::string secondBack = popWordStack(wordStack);

					if (secondBack == "Booster")
					{
						component.values["BoosterEnergy"] = scanReal(imageText, index);
					}
					else if (secondBack == "Capacitor")
					{
						component.values["CapacitorEnergy"] = scanReal(imageText, index);
					}
				}
				else if (back == "Armor")
				{
					std::string secondBack = popWordStack(wordStack);
					
					//The V font in swg does not at all look like v's, especially when pre processed
					if (component.values.count("Armor") > 0)
					{
						component.values["VsArmor"] = scanReal(imageText, index);
					}
					else {
						component.values["Armor"] = scanRealDivisor(imageText, index);
					}
				}
				else if (back == "Shields")
				{
					component.values["VsShields"] = scanReal(imageText, index);				
				}
				else if (back == "Energy/Shot")
				{
					component.values["Energy/Shot"] = scanReal(imageText, index);
				}
				else if (back == "Hitpoints")
				{
					std::string secondBack = popWordStack(wordStack);
					
					if (secondBack == "Shield")
					{
						std::string thirdBack = popWordStack(wordStack);

						if (thirdBack == "Front")
						{
							component.values["FrontShieldHp"] = scanRealDivisor(imageText, index);
						}
						else if (thirdBack == "Back")
						{
							component.values["BackShieldHp"] = scanRealDivisor(imageText, index);
						}
					}
					else {
						component.values["Hp"] = scanReal(imageText, index);
					}
				}
				else if (back == "Mass")
				{
					component.values["Mass"] = scanReal(imageText, index);
				}
				else if (back == "Rate")
				{
					std::string secondBack = popWordStack(wordStack);
					if (secondBack == "Refire")
					{
						component.values["RefireRate"] = scanReal(imageText, index);
					}
					else if (secondBack == "Recharge")
					{
						component.values["RechargeRate"] = scanReal(imageText, index);
					}
					else if (secondBack == "Generation")
					{
						component.values["ReactorGeneration"] = scanReal(imageText, index);
					}
					else if (secondBack == "Consumption")
					{
						component.values["ConsumptionRate"] = scanReal(imageText, index);
					}
				}
				else if (back == "Speed")
				{
					component.values["Speed"] = scanReal(imageText, index);
				}
				else if (back == "Acceleration")
				{
					component.values["Acceleration"] = scanReal(imageText, index);
				}
				else if (back == "Damage")
				{
					double minDmg, maxDmg;
					scanRealMinMax(imageText, index, minDmg, maxDmg);

					component.values["MinDamage"] = minDmg;
					component.values["MaxDamage"] = maxDmg;
				}
				else if (back == "Level")
				{
					std::string secondBack = popWordStack(wordStack);

					if (secondBack == "Engineering")
					{
						component.values["ReverseLevel"] = scanReal(imageText, index);
					}
				}
				else if (back == "Drain")
				{
					component.values["ReactorDrain"] = scanReal(imageText, index);
				}

				wordStack.clear();
			}

			lastWord = false;
			lastSpace = true;
		}
		current = imageText[++index];
	}

	ValidateComponent(component);

	return component;
}