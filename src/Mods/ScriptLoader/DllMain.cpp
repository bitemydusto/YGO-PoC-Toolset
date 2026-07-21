#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "Utils.h"

const uint32_t BASE_OFFSET = 0x005ed0a8; // Change this if it's different in your version of the game
std::string scriptFolder = ".\\scripts";
namespace fs = std::filesystem;
using json = nlohmann::json;

struct EffectScript
{
    uint32_t CardID;
    uint32_t Pointer1;
    uint32_t Pointer2;
    uint32_t Pointer3;
    uint32_t Pointer4;
    uint32_t Pointer5;
};

std::vector<EffectScript> Effects;
void LoadScripts();
void Log(const std::string& message, bool clearLog = false);


DWORD WINAPI MainThread(LPVOID lpParam)
{
    LoadScripts();

    return 0;
}
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinst);
        CreateThread(0, 0, &MainThread, 0, 0, NULL);
    }

    return TRUE;
}
void LoadScripts()
{
    // Load list of EffectScripts from memory
	int address = BASE_OFFSET;
    for (size_t i = 0; i < 443; i++)
    {
		uint32_t cardID = Utils::ReadUint32((void*)address);
		address += 4;
		uint32_t pointer1 = Utils::ReadUint32((void*)address);
		address += 4;
		uint32_t pointer2 = Utils::ReadUint32((void*)address);
		address += 4;
		uint32_t pointer3 = Utils::ReadUint32((void*)address);
		address += 4;
		uint32_t pointer4 = Utils::ReadUint32((void*)address);
		address += 4;
		uint32_t pointer5 = Utils::ReadUint32((void*)address);
		address += 4;

		Effects.push_back({ cardID, pointer1, pointer2, pointer3, pointer4, pointer5 });
    }

	// Load scripts from the script folder
    fs::create_directories(scriptFolder);

    for (auto file : fs::recursive_directory_iterator(scriptFolder))
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

					int index = j["Index"];
					uint32_t cardID;
					uint32_t effect1;
					uint32_t effect2;
					uint32_t condition;
					uint32_t cost;
					uint32_t target;

					std::string s_cardID = j["CardID"];
					std::string s_effect1 = j["Effect1"];
					std::string s_effect2 = j["Effect2"];
					std::string s_condition = j["Condition"];
					std::string s_cost = j["Cost"];
					std::string s_target = j["Target"];

					if (s_cardID.starts_with("0x") || s_cardID.starts_with("0X"))
						cardID = std::stoul(s_cardID, nullptr, 16);
					else
						cardID = std::stoul(s_cardID, nullptr, 10);

					if (s_effect1.starts_with("0x") || s_effect1.starts_with("0X"))
						effect1 = std::stoul(s_effect1, nullptr, 16);
					else
						effect1 = std::stoul(s_effect1, nullptr, 10);

					if (s_effect2.starts_with("0x") || s_effect2.starts_with("0X"))
						effect2 = std::stoul(s_effect2, nullptr, 16);
					else
						effect2 = std::stoul(s_effect2, nullptr, 10);

					if (s_condition.starts_with("0x") || s_condition.starts_with("0X"))
						condition = std::stoul(s_condition, nullptr, 16);
					else
						condition = std::stoul(s_condition, nullptr, 10);

					if (s_cost.starts_with("0x") || s_cost.starts_with("0X"))
						cost = std::stoul(s_cost, nullptr, 16);
					else
						cost = std::stoul(s_cost, nullptr, 10);

					if (s_target.starts_with("0x") || s_target.starts_with("0X"))
						target = std::stoul(s_target, nullptr, 16);
					else
						target = std::stoul(s_target, nullptr, 10);

					Effects[index] = { cardID, effect1, effect2, condition, cost, target };
				}
			}
			catch (const std::exception&)
			{
				Log("Failed to load script: " + file.path().string());
			}
		}
    }

	// Sort effects by their ID in ascending order
	std::sort(std::begin(Effects), std::end(Effects),
		[](const EffectScript& a, const EffectScript& b)
		{
			return a.CardID < b.CardID;
		});

	// Write the sorted effects to memory
	int writeAddress = BASE_OFFSET;
	for (const auto& effect : Effects)
	{
		Utils::WriteUint32((void*)writeAddress, effect.CardID);
		writeAddress += 4;
		Utils::WriteUint32((void*)writeAddress, effect.Pointer1);
		writeAddress += 4;
		Utils::WriteUint32((void*)writeAddress, effect.Pointer2);
		writeAddress += 4;
		Utils::WriteUint32((void*)writeAddress, effect.Pointer3);
		writeAddress += 4;
		Utils::WriteUint32((void*)writeAddress, effect.Pointer4);
		writeAddress += 4;
		Utils::WriteUint32((void*)writeAddress, effect.Pointer5);
		writeAddress += 4;
	}
}
void Log(const std::string& message, bool clearLog)
{
	fs::create_directories(scriptFolder);

	std::ios::openmode mode = clearLog ? std::ios::trunc : std::ios::app;
	std::ofstream logFile(fs::path(scriptFolder) / "log.txt", mode);
	if (logFile.is_open())
	{
		logFile << message << '\n';
	}

}
