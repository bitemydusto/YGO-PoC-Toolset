#pragma once

#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"
#include "Cards.h"

using EffectScript = Utils::EffectScript;
inline auto* duel = GameData::GetDuel();
inline auto* battleResult = GameData::GetBattleResult();

void Start();

void Install_PlagueSpreaderZombie();
void Install_LADD();
void Install_JunkSynchron();
void Install_StardustDragon();