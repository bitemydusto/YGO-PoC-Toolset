#pragma once

#pragma once
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <string>

#include <iostream>
#include <vector>
#include <array>
#include <cstdint>

namespace CRC
{
	const std::string gameExecutablePath = ".\\Joey_PC.exe";

	constexpr auto generateCRCTable() {
		std::array<uint32_t, 256> table{};
		for (uint32_t i = 0; i < 256; ++i) {
			uint32_t crc = i;
			for (int j = 0; j < 8; ++j) {
				crc = (crc & 1) ? (0xEDB88320 ^ (crc >> 1)) : (crc >> 1);
			}
			table[i] = crc;
		}
		return table;
	}
	static constexpr auto crcTable = generateCRCTable();

	uint32_t calculateFileCRC32(const std::string& filePath) {
		std::ifstream file(filePath, std::ios::binary);
		if (!file) throw std::runtime_error("Could not open file.");

		uint32_t crc = 0xFFFFFFFF;
		constexpr size_t bufferSize = 65536;
		std::vector<char> buffer(bufferSize);

		while (file.read(buffer.data(), bufferSize) || file.gcount() > 0) {
			size_t bytesRead = file.gcount();
			for (size_t i = 0; i < bytesRead; ++i) {
				uint8_t byte = static_cast<uint8_t>(buffer[i]);
				crc = crcTable[(crc ^ byte) & 0xFF] ^ (crc >> 8);
			}
		}

		return crc ^ 0xFFFFFFFF;
	}
	void CheckGameHash()
	{
		uint32_t fileCRC = calculateFileCRC32(gameExecutablePath);
		if (fileCRC != 3516070012)
		{
			MessageBoxA(NULL, "Your game executable is edited or not the expected version. Make sure to use the NOCD/Fixed [DE] version.", "Warning", MB_OK);
		}
	}
}