#include "PatchLoader.h"

namespace PatchLoader
{
	const uint32_t EFFECT_SCRIPTS_OFFSET = 0x005ed0a8; // Change this if it's different in your version of the game
	const uint32_t FUSION_RECIPES_OFFSET = 0x005ecf10; // Change this if it's different in your version of the game
	const std::string patchFolder = ".\\patches";

	std::vector<EffectScript> EffectScripts;
	std::vector<Fusion2> FusionRecipes2;
	std::vector<Fusion3> FusionRecipes3;

	std::vector<SpellSpeed> SpellSpeeds;
	std::vector<uint16_t> ActivatableCards;
	std::vector<uint16_t> FlipMonsters;
	std::vector<uint16_t> InherentSpecialSummons;

	void LoadPatches()
	{
		Log("Loading patches...", true);
		LoadInMemoryLists();

		fs::create_directories(patchFolder);
		for (auto file : fs::recursive_directory_iterator(patchFolder))
		{
			if (file.is_regular_file() && file.path().extension() == ".json")
			{
				try
				{
					std::ifstream input(file.path());
					if (input)
					{
						json j;
						input >> j;

						// EffectScripts
						if (j.contains("EffectScripts"))
						{
							for (const auto& script : j["EffectScripts"])
							{
								int index = script["Index"];
								if (index < 0 || index >= EffectScripts.size())
								{
									Log("Invalid index in EffectScripts: " + std::to_string(index) + " in file: " + file.path().string());
									continue;
								}

								uint32_t cardID;
								uint32_t effect;
								uint32_t appliesTo;
								uint32_t condition;
								uint32_t cost;
								uint32_t target;

								std::string s_cardID = script["CardID"];
								std::string s_effect = script["Effect"];
								std::string s_appliesTo = script["AppliesTo"];
								std::string s_condition = script["Condition"];
								std::string s_cost = script["Cost"];
								std::string s_target = script["Target"];

								if (s_cardID.starts_with("0x") || s_cardID.starts_with("0X")) cardID = std::stoul(s_cardID, nullptr, 16);
								else cardID = std::stoul(s_cardID, nullptr, 10);

								if (s_effect.starts_with("0x") || s_effect.starts_with("0X")) effect = std::stoul(s_effect, nullptr, 16);
								else effect = std::stoul(s_effect, nullptr, 10);

								if (s_appliesTo.starts_with("0x") || s_appliesTo.starts_with("0X")) appliesTo = std::stoul(s_appliesTo, nullptr, 16);
								else appliesTo = std::stoul(s_appliesTo, nullptr, 10);

								if (s_condition.starts_with("0x") || s_condition.starts_with("0X")) condition = std::stoul(s_condition, nullptr, 16);
								else condition = std::stoul(s_condition, nullptr, 10);

								if (s_cost.starts_with("0x") || s_cost.starts_with("0X")) cost = std::stoul(s_cost, nullptr, 16);
								else cost = std::stoul(s_cost, nullptr, 10);

								if (s_target.starts_with("0x") || s_target.starts_with("0X")) target = std::stoul(s_target, nullptr, 16);
								else target = std::stoul(s_target, nullptr, 10);

								EffectScripts[index] = { cardID, effect, appliesTo, condition, cost, target };
							}
						}
						// FusionRecipes2
						if (j.contains("Fusion2Recipes"))
						{
							for (const auto& recipe : j["Fusion2Recipes"])
							{
								int index = recipe["Index"];
								if (index < 0 || index >= FusionRecipes2.size())
								{
									Log("Invalid index in Fusion2Recipes: " + std::to_string(index) + " in file: " + file.path().string());
									continue;
								}
								uint16_t result;
								std::string s_result = recipe["Result"];

								if (s_result.starts_with("0x") || s_result.starts_with("0X")) result = std::stoul(s_result, nullptr, 16);
								else result = std::stoul(s_result, nullptr, 10);

								FusionRecipes2[index].Result = result;
								for (int i = 0; i < 2; i++)
								{
									uint16_t material;
									std::string s_material = recipe["Materials"][i];

									if (s_material.starts_with("0x") || s_material.starts_with("0X")) material = std::stoul(s_material, nullptr, 16);
									else material = std::stoul(s_material, nullptr, 10);

									FusionRecipes2[index].Materials[i] = material;
								}
							}
						}
						// FusionRecipes3
						if (j.contains("Fusion3Recipes"))
						{
							for (const auto& recipe : j["Fusion3Recipes"])
							{
								int index = recipe["Index"];
								if (index < 0 || index >= FusionRecipes3.size())
								{
									Log("Invalid index in Fusion3Recipes: " + std::to_string(index) + " in file: " + file.path().string());
									continue;
								}
								uint16_t result;
								std::string s_result = recipe["Result"];

								if (s_result.starts_with("0x") || s_result.starts_with("0X")) result = std::stoul(s_result, nullptr, 16);
								else result = std::stoul(s_result, nullptr, 10);

								FusionRecipes3[index].Result = result;
								for (int i = 0; i < 3; i++)
								{
									uint16_t material;
									std::string s_material = recipe["Materials"][i];

									if (s_material.starts_with("0x") || s_material.starts_with("0X")) material = std::stoul(s_material, nullptr, 16);
									else material = std::stoul(s_material, nullptr, 10);

									FusionRecipes3[index].Materials[i] = material;
								}
							}
						}
						// ActivatableCards
						if (j.contains("ActivatableCards"))
						{
							for (const auto& card : j["ActivatableCards"])
							{
								uint16_t cardID;
								std::string s_cardID = card;

								if (s_cardID.starts_with("0x") || s_cardID.starts_with("0X")) cardID = std::stoul(s_cardID, nullptr, 16);
								else cardID = std::stoul(s_cardID, nullptr, 10);

								ActivatableCards.push_back(cardID);
							}
						}
						// SpellSpeeds
						if (j.contains("SpellSpeeds"))
						{
							for (const auto& item : j["SpellSpeeds"])
							{
								SpellSpeed speed;
								uint32_t cardID;
								std::string  s_cardID = item["CardID"];

								if (s_cardID.starts_with("0x") || s_cardID.starts_with("0X")) cardID = std::stoul(s_cardID, nullptr, 16);
								else cardID = std::stoul(s_cardID, nullptr, 10);

								speed.CardID = cardID;
								speed.Speed = item["Speed"];
								SpellSpeeds.push_back(speed);
							}
						}
						// FlipMonsters
						if (j.contains("FlipMonsters"))
						{
							for (const auto& card : j["FlipMonsters"])
							{
								uint16_t cardID;
								std::string s_cardID = card;

								if (s_cardID.starts_with("0x") || s_cardID.starts_with("0X")) cardID = std::stoul(s_cardID, nullptr, 16);
								else cardID = std::stoul(s_cardID, nullptr, 10);

								FlipMonsters.push_back(cardID);
							}
						}
						// InherentSpecialSummons
						if (j.contains("InherentSpecialSummons"))
						{
							for (const auto& card : j["InherentSpecialSummons"])
							{
								uint16_t cardID;
								std::string s_cardID = card;
								if (s_cardID.starts_with("0x") || s_cardID.starts_with("0X")) cardID = std::stoul(s_cardID, nullptr, 16);
								else cardID = std::stoul(s_cardID, nullptr, 10);
								InherentSpecialSummons.push_back(cardID);
							}
						}
					}
					Log("Successfully loaded patch: " + file.path().string());
				}
				catch (const std::exception& e)
				{
					Log("Failed to load patch: " + file.path().string() + "\nError: " + std::string(e.what()));
				}
			}

		}
		WriteInMemoryLists();
	}
	void LoadInMemoryLists()
	{
		// Load EffectScripts from memory
		uint32_t effectScriptAddress = EFFECT_SCRIPTS_OFFSET;
		for (size_t i = 0; i < 443; i++)
		{
			EffectScript script;
			script.CardID = Utils::ReadUint32((void*)effectScriptAddress);
			effectScriptAddress += 4;
			script.Effect = Utils::ReadUint32((void*)effectScriptAddress);
			effectScriptAddress += 4;
			script.AppliesTo = Utils::ReadUint32((void*)effectScriptAddress);
			effectScriptAddress += 4;
			script.Condition = Utils::ReadUint32((void*)effectScriptAddress);
			effectScriptAddress += 4;
			script.Cost = Utils::ReadUint32((void*)effectScriptAddress);
			effectScriptAddress += 4;
			script.Target = Utils::ReadUint32((void*)effectScriptAddress);
			effectScriptAddress += 4;

			EffectScripts.push_back(script);
		}
		// Load FusionRecipes2 from memory
		uint32_t fusionRecipe2Address = FUSION_RECIPES_OFFSET;
		for (size_t i = 0; i < 52; i++)
		{
			Fusion2 recipe;
			recipe.Result = Utils::ReadUint16((void*)fusionRecipe2Address);
			fusionRecipe2Address += 2;
			for (int j = 0; j < 2; j++)
			{
				recipe.Materials[j] = Utils::ReadUint16((void*)fusionRecipe2Address);
				fusionRecipe2Address += 2;
			}

			FusionRecipes2.push_back(recipe);
		}
		// Load FusionRecipes3 from memory
		uint32_t fusionRecipe3Address = fusionRecipe2Address; // Assuming FusionRecipes3 follows FusionRecipes2 in memory
		for (size_t i = 0; i < 3; i++)
		{
			Fusion3 recipe;
			recipe.Result = Utils::ReadUint16((void*)fusionRecipe3Address);
			fusionRecipe3Address += 2;
			for (int j = 0; j < 3; j++)
			{
				recipe.Materials[j] = Utils::ReadUint16((void*)fusionRecipe3Address);
				fusionRecipe3Address += 2;
			}

			FusionRecipes3.push_back(recipe);
		}
	}
	void Log(const std::string& message, bool clearLog)
	{
		fs::create_directories(patchFolder);

		std::ios::openmode mode = clearLog ? std::ios::trunc : std::ios::app;
		std::ofstream logFile(fs::path(patchFolder) / "log.txt", mode);
		if (logFile.is_open())
		{
			logFile << message << '\n';
		}
	}
	void WriteInMemoryLists()
	{
		// Sort effects by their ID in ascending order
		std::sort(std::begin(EffectScripts), std::end(EffectScripts),
			[](const EffectScript& a, const EffectScript& b)
			{
				return a.CardID < b.CardID;
			});

		// Write EffectScripts to memory
		uint32_t effectScriptAddress = EFFECT_SCRIPTS_OFFSET;
		for (const auto& script : EffectScripts)
		{
			Utils::WriteUint32((void*)effectScriptAddress, script.CardID);
			effectScriptAddress += 4;
			Utils::WriteUint32((void*)effectScriptAddress, script.Effect);
			effectScriptAddress += 4;
			Utils::WriteUint32((void*)effectScriptAddress, script.AppliesTo);
			effectScriptAddress += 4;
			Utils::WriteUint32((void*)effectScriptAddress, script.Condition);
			effectScriptAddress += 4;
			Utils::WriteUint32((void*)effectScriptAddress, script.Cost);
			effectScriptAddress += 4;
			Utils::WriteUint32((void*)effectScriptAddress, script.Target);
			effectScriptAddress += 4;
		}
		// Write FusionRecipes2 to memory
		uint32_t fusionRecipe2Address = FUSION_RECIPES_OFFSET;
		for (const auto& recipe : FusionRecipes2)
		{
			Utils::WriteUint16((void*)fusionRecipe2Address, recipe.Result);
			fusionRecipe2Address += 2;
			for (int i = 0; i < 2; i++)
			{
				Utils::WriteUint16((void*)fusionRecipe2Address, recipe.Materials[i]);
				fusionRecipe2Address += 2;
			}
		}
		// Write FusionRecipes3 to memory
		uint32_t fusionRecipe3Address = fusionRecipe2Address; // Assuming FusionRecipes3 follows FusionRecipes2 in memory
		for (const auto& recipe : FusionRecipes3)
		{
			Utils::WriteUint16((void*)fusionRecipe3Address, recipe.Result);
			fusionRecipe3Address += 2;
			for (int i = 0; i < 3; i++)
			{
				Utils::WriteUint16((void*)fusionRecipe3Address, recipe.Materials[i]);
				fusionRecipe3Address += 2;
			}
		}
	}

}