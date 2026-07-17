#include "ModLoader.h"

void ModLoader::LoadMods()
{
	Log("ModLoader initialized!", true);
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