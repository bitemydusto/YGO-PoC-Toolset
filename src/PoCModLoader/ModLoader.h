#pragma once
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

class ModLoader
{
	const static std::string modFolder;

	public:
		void LoadMods();
		static void Log(const std::string& message, bool clearLog = false);

	private:
		void LoadDLLs();
};
