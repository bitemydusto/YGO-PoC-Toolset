#include "ModLoader.h"

const std::string ModLoader::modFolder = ".\\mods";

void ModLoader::LoadMods()
{
	Log("ModLoader initialized!");
	LoadDLLs();
}

void ModLoader::LoadDLLs()
{
	// Load DLLs from the mod folder
	fs::create_directories(modFolder);

	for (auto file : fs::recursive_directory_iterator(modFolder))
	{
		if (file.is_regular_file() && file.path().extension() == ".dll")
		{
			Log("Loading mod: " + file.path().filename().string());
			if (LoadLibraryA(file.path().string().c_str()))
			{
				Log("Successfully loaded mod: " + file.path().filename().string());
			}
			else
			{
				Log("Failed to load mod: " + file.path().filename().string());
			}
		}
	}
}

void ModLoader::Log(const std::string& message, bool clearLog)
{
	fs::create_directories(modFolder);

	std::ios::openmode mode = clearLog ? std::ios::trunc : std::ios::app;
	std::ofstream logFile(fs::path(modFolder) / "log.txt", mode);
	if (logFile.is_open())
	{
		logFile << message << '\n';
	}
}
bool ModLoader::WaitForGameReady()
{
	Log("Waiting for the game to be ready...", true);

	const int maxWaitTime = 5000; // Maximum wait time in milliseconds
	const int startTime = GetTickCount();

	while (!IsGameReady())
	{
		if (GetTickCount() - startTime > maxWaitTime)
		{
			Log("Error: Mod loader timed out waiting for game to be ready.");
			return false;
		}
		Sleep(100);
	}
	Log("Game was ready after " + std::to_string(GetTickCount() - startTime) + " ms.");
	Sleep(500); // Additional wait to ensure the game is fully initialized

	return true;
}
bool ModLoader::IsGameReady()
{
	const uintptr_t DLG_TEXT_LIST_ADDRESS = 0x05f24c0;

	if (*(uint32_t*)DLG_TEXT_LIST_ADDRESS == 0) return false;

	return true;
}