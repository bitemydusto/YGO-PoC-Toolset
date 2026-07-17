#pragma once
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

class ModLoader
{
	public:
		void LoadMods();

	private:
		std::string modFolder = ".\\mods";
		void LoadDLLs();
		void Log(const std::string& message, bool clearLog = false);
};
