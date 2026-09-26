#include <Windows.h>

#include "Utils.h"
#include "Cards.h"
#include "GameData.h"
#include "HookAPI.h"

const uint16_t LADD = Cards::ARMA_KNIGHT;

GameData::Duel* duel = GameData::GetDuel();

uint32_t* selectedMonster = nullptr;

void Start();

uint32_t __cdecl Effect_LADD(unsigned int* self, unsigned int* source, int mode);
uint32_t __cdecl Condition_LADD(unsigned int* self, unsigned int* source, int mode);
uint32_t __cdecl Target_LADD(unsigned int* self, unsigned int* source, int mode);

void __stdcall StatChange_LADD(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);
void __stdcall EffectActivated_LADD(unsigned int* srcParam, uint8_t respondingSide);


DWORD WINAPI MainThread(LPVOID lpParam)
{
    Sleep(500);
    Start();

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

void Start()
{
	//Register_ActivatableEffect(LADD);
	Register_UnRevivable(LADD);
	Register_StatChange(LADD, StatChange_LADD);
	Register_SpellSpeed(LADD, 2);
	Register_OnSentToGraveTrigger(LADD, nullptr);
	Register_OnEffectActivated(EffectActivated_LADD);

    Utils::EffectScript script;
    script.CardID = LADD;
    script.Effect = reinterpret_cast<uintptr_t>(&Effect_LADD);
    script.AppliesTo = 0;
    script.Condition = reinterpret_cast<uintptr_t>(&Condition_LADD);
    script.Cost = 0;
    script.Target = reinterpret_cast<uintptr_t>(&Target_LADD);

    Register_EffectScript(script);

}
uint32_t __cdecl Effect_LADD(unsigned int* self, unsigned int* source, int mode)
{
    FUN::Param selfParam(self);

    if (selfParam.finishedResolving) return 0;

    if (selfParam.location == 0xe)
    {
		uint8_t state = GameData::GetEffectState();

        switch (state)
        {
            case 0x80:
            {
				FUN::FieldMaskGenerator maskGen;
                for (size_t i = 0; i < 11; i++)
                {
                    maskGen.zones[selfParam.playerIdx][i] = true;
                }
                
				FUN::SendCardFromField(selfParam.block, maskGen.GenerateMask(), 0xe, 2);
                return 0x7f;
            }
			case 0x7f:
			{
				if (selfParam.targetCount < 1) return 0;
                uint16_t intId = selfParam.outerTargets[0] & 0xFFF;

                if (intId == 0) return 0;

                FUN::SpecialSummon(selfParam.playerIdx, selectedMonster, 1, 0x20, 0x0e, 0);

				return 0;
			}
        }
    }
    else
    {
        FUN::W_NegateActivation(source, false);
        FUN::W_AddEffectEntityToZone(selfParam.playerIdx, selfParam.zoneIdx, FUN::GetCardIntID(LADD), 0xb | (0 << 8));

        return 0;
    }

}
uint32_t __cdecl Condition_LADD(unsigned int* self, unsigned int* source, int mode)
{
    FUN::Param selfParam(self);

    if (selfParam.location == 0xe) return 1;
    //if (mode != 0) return 0;
	if (source == 0) return 0;

    //Checks for location, it's not needed here
    //if (0x14 < (sourceParam.block16[1] & 0x3e)) return 0;

	FUN::Param sourceParam(source);
	uint16_t tCardID = FUN::GetCardID(sourceParam.cardIntID);

	if (FUN::GetSpellSpeed(sourceParam.cardIntID) > 2) return 0;
	if (tCardID == LADD || tCardID == Cards::SPIRITUALISM) return 0;

	int atk = FUN::GetCurrentATK(selfParam.playerIdx, selfParam.zoneIdx);
	int def = FUN::GetCurrentDEF(selfParam.playerIdx, selfParam.zoneIdx);

	if (atk < 500 || def < 500) return 0;


    return 1;
}
uint32_t __cdecl Target_LADD(unsigned int* self, unsigned int* source, int mode)
{
	FUN::Param selfParam(self);
	if (selfParam.location != 0xe) return 1;

    uint8_t sub = GameData::GetEffectSubState();

    switch (sub)
    {
        case 0:
        {
			FUN::PopulateSelectionList(selfParam.playerIdx, Cards::PREMATURE_BURIAL, 0);
            if (FUN::GetSelectionListCount() < 1)
            {
				GameData::SetEffectSubState(0);
				return 1;

			}
            GameData::SetEffectSubState(1);
            return 0;
        }
        case 1:
        {
            FUN::ShowDialog("Select a @2Monster@0 from your Graveyard to @3Special Summon@0.");

			GameData::SetEffectSubState(2);
            return 0;
        }
        case 2:
        {
			FUN::InitiateSelectionList(selfParam.playerIdx, 6, Cards::PREMATURE_BURIAL, 0);

			GameData::SetEffectSubState(3);
			return 0;
        }
        case 3:
        {
            uint32_t count = FUN::GetSelectionListCount();
            if (count == 0)
            {
                GameData::SetEffectSubState(0);
                return 1;
            }

            uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
			selectedMonster = entry;
            if (!entry || (*entry & 0xFFF) == 0) return 0; // not ready

            uint32_t dword = *entry;
            uint8_t  owner = (dword >> 12) & 1;
            uint32_t inst = owner + ((dword >> 24) & 0x7F) * 2;
            uint32_t sideBit = owner ? 0x8000u : 0;

            uint32_t cardId = FUN::GetCardID(dword & 0xFFF); // match your GetCardID arity

            // Highlight / reveal
            FUN::QueueCommand(sideBit | 0xDF, cardId, inst, 0);
            FUN::QueueCommand(sideBit | 0x08, owner, 0x0E, 0);  // 0x0E = GY

            // Store targets
            FUN::StoreTarget((int)self, (uint16_t)dword);
            FUN::StoreTarget((int)self, (uint16_t)(dword >> 16));

            GameData::SetEffectSubState(0);
            return 1;
        }
        default:
        {
            GameData::SetEffectSubState(0);
            return 1;
        }
    }
}
void __stdcall StatChange_LADD(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	int count = FUN::HasEffectEntiry(playerIdx, zoneIdx, LADD);
	if (count == 0) return;

    // Modify stats
    // 0x20 = ATK, 0x24 = DEF
    uint32_t& refATK = *(uint32_t*)(statAddress + 0x20);
    uint32_t& refDEF = *(uint32_t*)(statAddress + 0x24);

    refATK = refATK += 500 * count * -1;
    refDEF = refDEF += 500 * count * -1;

}
void __stdcall EffectActivated_LADD(unsigned int* srcParam, uint8_t respondingSide)
{
    for (uint8_t j = 0; j < 5; j++)
    {
        uint16_t cardIntID = duel->players[respondingSide].monsterZones[j].card.GetIntID();
		if (cardIntID == 0) continue;

        uint16_t cardID = FUN::GetCardID(cardIntID);
        if (cardID == LADD && duel->players[respondingSide].monsterZones[j].IsFaceUp())
        {
            int atk = FUN::GetCurrentATK(respondingSide, j);
            int def = FUN::GetCurrentDEF(respondingSide, j);

            if (atk >= 500 && def >= 500)
            {
                FUN::FlashCardPortrait(respondingSide, cardIntID, j);

                uint32_t pack = ((uint32_t)(j & 0x1F) | ((uint32_t)respondingSide << 0xf) | 0x0A20u) << 16 | cardIntID;
                FUN::RespondToEffect(pack, duel->players[respondingSide].monsterZones[j].card.GetInstance(), srcParam, 1);

            }

        }
    }
}