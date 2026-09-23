#pragma once

#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>

#include "GameData.h"
#include "HookAPI.h"


using Fn0 = uint32_t(__cdecl*)();
using Fn1 = uint32_t(__cdecl*)(uint32_t);
using Fn2 = uint32_t(__cdecl*)(uint32_t, uint32_t);
using Fn3 = uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t);
using Fn4 = uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t);
using Fn5 = uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
using Fn6 = uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

void Start();

uint32_t CallFunction(uintptr_t address, const std::vector<uint32_t>& args);

void MillCommand(uint32_t playerIdx, uint32_t amount);
void DrawCommand(uint32_t playerIdx, uint32_t amount);
void AddCommand(uint8_t player, uint16_t cardID);
void RollCommand(uint32_t playerIdx, uint32_t sideIdx, uint8_t diceCmd);
void SummonCommand(uint8_t player, uint16_t cardID);