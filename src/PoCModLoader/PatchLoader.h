#pragma once

#include <Windows.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>


#include "Utils.h"

namespace fs = std::filesystem;
namespace PatchLoader
{
	using json = nlohmann::json;
	using EffectScript = Utils::EffectScript;
	using Fusion2 = Utils::Fusion2;
	using Fusion3 = Utils::Fusion3;
	using SpellSpeed = Utils::SpellSpeed;


	extern std::vector<EffectScript> EffectScripts;
	extern std::vector<Fusion2> FusionRecipes2;
	extern std::vector<Fusion3> FusionRecipes3;

	extern std::vector<SpellSpeed> SpellSpeeds;
	extern std::vector<uint16_t> ActivatableCards;
	extern std::vector<uint16_t> FlipMonsters;
	extern std::vector<uint16_t> InherentSpecialSummons;


	void LoadPatches();
	void LoadInMemoryLists();
	void WriteInMemoryLists();
	void Log(const std::string& message, bool clearLog = false);


}