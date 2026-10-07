#pragma once

#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"
#include "Cards.h"

using EffectScript = Utils::EffectScript;
using EffectBlock = FUN::EffectBlock;
inline auto* duel = GameData::GetDuel();
inline auto* battleResult = GameData::GetBattleResult();

void Start();

void Install_Breaker();
void Install_Tsukuyomi();
void Install_Chaos();
void Install_XYZ();
void Install_Metamorphosis();
void Install_LavaGolem();
void Install_Mirage();
void Install_Tribe();
void Install_Reasoning();
void Install_MonsterGate();
void Install_SakuretsuArmor();
void Install_DarkPaladin();