#pragma once

#include <cstdint>
#include "HookAPI.h"

using namespace std;

// 0-4: Monster Zones
// 5-9: Spell/Trap Zones
enum Location : uint8_t
{
	FIELDZONE = 0xA,
	HAND = 0xB,
	EXTRA = 0xC,
	DECK = 0xD,
	GRAVE = 0xE,
	BANISHED = 0xF
};
enum SubType : uint8_t
{
	NORMAL = 0,
	EFFECT = 1,
	FUSION = 2,
	RITUAL = 3,
};
enum CardType : uint8_t
{
	Dragon = 0x01,
	Zombie = 0x02,
	Fiend = 0x03,
	Pyro = 0x04,
	SeaSerpent = 0x05,
	Rock = 0x06,
	Machine = 0x07,
	Fish = 0x08,
	Dinosaur = 0x09,
	Insect = 0x0A,
	Beast = 0x0B,
	BeastWarrior = 0x0C,
	Plant = 0x0D,
	Aqua = 0x0E,
	Warrior = 0x0F,
	WingedBeast = 0x10,
	Fairy = 0x11,
	Spellcaster = 0x12,
	Thunder = 0x13,
	Reptile = 0x14,
	Trap = 0x15,
	Spell = 0x16
};
enum DialogMode : uint8_t
{
	OK = 0,
	YESNO = 1,
	OPTION2 = 2,
	OPTION3 = 3,
	TYPE = 4,
	ATTRIBUTE = 5,
	POSITION = 6,
	COIN = 7,
};
enum ResponseWindow : uint8_t
{
	NORMAL_SUMMON = 0x5,
	FLIP_SUMMON = 0x6,
	SPECIAL_SUMMON = 0x7,
	ATTACK_DECLARATION = 0x12
};
enum CardTag : uint16_t
{
	DESTROY = 0x0,
	SEARCH = 0x1,
	DRAW = 0x2,
};
namespace FUN
{
	// Helper structs
	struct FieldMaskGenerator
	{
		bool zones[2][11] = {};

		uint32_t GenerateMask()
		{
			uint32_t mask = 0;
			for (int i = 0; i < 11; i++)
			{
				if (zones[0][i]) mask |= (1 << i);
				if (zones[1][i]) mask |= (1 << (i + 16));
			}
			return mask;
		}
	};
	struct Param
	{
		uint8_t* block = nullptr;
		uint16_t* block16 = nullptr;


		uint8_t finishedResolving;
		uint16_t cardIntID;
		uint8_t  playerIdx;
		uint8_t oppIdx;
		uint8_t zoneIdx;
		uint8_t  location;
		uint32_t instance;
		uint16_t responseWindow;
		uint32_t targetCount;
		uint16_t* fieldTargets;
		uint32_t* outerTargets;
		uint16_t triggerSide;
		uint16_t triggerZone;
		Param(unsigned int* param)
		{
			block = (uint8_t*)param;
			block16 = (uint16_t*)param;

			finishedResolving = block[4] & 4;
			cardIntID = *(uint16_t*)(block + 0) & 0xFFF;
			playerIdx = block[2] & 0x1;
			oppIdx = playerIdx ^ 1;
			zoneIdx = (block[2] >> 1) & 0xF;
			location = (block[2] >> 1) & 0x1F;
			instance = (*(uint16_t*)(block + 4) & 0x1FE0) >> 5;
			responseWindow = (block16[1] & 0xfc0) >> 6;
			targetCount = *(uint16_t*)(block + 4) >> 13;
			fieldTargets = (uint16_t*)(block + 6);
			outerTargets = (uint32_t*)(block + 6);
			triggerSide = (block16[8]) & 1;
			triggerZone = (block16[8] >> 8) & 0xf;
		}
		uint8_t GetFieldTargetSide(uint8_t index)
		{
			return fieldTargets[index] & 1;
		}
		uint8_t GetFieldTargetZone(uint8_t index)
		{
			return (fieldTargets[index] >> 4) & 0xF;
		}
	};

	struct EffectBlock
	{
		uint16_t id = 0;
		uint16_t properties = 0;
		uint16_t flags = 0;
		uint8_t targets[10] = {};
		uint32_t eventInfo = 0;

		void BuildId(uint16_t cardIntID)
		{
			this->id = cardIntID;
		}
		void BuildProperties(uint8_t side, uint8_t location, uint8_t responseWindow, uint8_t kind, uint8_t alreadyUsed)
		{
			side &= 1;
			location &= 0x1f;
			responseWindow &= 0x3f;
			kind &= 3;
			alreadyUsed &= 1;
			this->properties = (alreadyUsed << 14) | (kind << 12) | (responseWindow << 6) | (location << 1) | (side);
		}
		void BuildFlags(uint8_t finishedResolving, uint8_t instance, uint8_t targetCount)
		{
			finishedResolving &= 1;
			instance &= 0xff;
			targetCount &= 7;
			this->flags = (targetCount << 13) | (instance << 5) | (finishedResolving << 2);
		}
		void BuildEventInfo(uint8_t eventSide, uint8_t eventLocation)
		{
			eventSide &= 1;
			eventLocation &= 0x1f;
			this->eventInfo = (eventLocation << 8) | (eventSide);
		}
		void AddInstanceTarget(uint16_t inst)
		{
			if (this->GetTargetCount() > 5) return;
			((uint16_t*)targets)[this->GetTargetCount()] = inst;
		}
		void AddFullTarget(uint32_t dword)
		{
			if (this->GetTargetCount() > 2) return;
			((uint32_t*)targets)[this->GetTargetCount()] = dword;
		}
		uint8_t GetIntID()
		{
			return id & 0xFFF;
		}
		uint8_t GetSide()
		{
			return properties & 1;
		}
		uint8_t GetLocation()
		{
			return (properties >> 1) & 0x1F;
		}
		uint8_t GetResponseWindow()
		{
			return (properties >> 6) & 0x3F;
		}
		uint8_t GetKind()
		{
			return (properties >> 12) & 3;
		}
		bool GetAlreadyUsed()
		{
			return (properties >> 14) & 1;
		}
		bool GetFinishedResolving()
		{
			return (flags >> 2) & 1;
		}
		uint8_t  GetInstance()
		{
			return (flags >> 5) & 0xff;
		}
		uint8_t GetTargetCount()
		{
			return (flags >> 13) & 7;
		}
		uint16_t GetInstanceTarget(uint8_t index)
		{
			if (index >= this->GetTargetCount()) return 0;
			return ((uint16_t*)targets)[index];
		}
		uint32_t  GetFullTarget(uint8_t index)
		{
			if (index >= this->GetTargetCount()) return 0;
			return ((uint32_t*)targets)[index];
		}
		uint8_t GetEventSide()
		{
			return eventInfo & 1;
		}
		uint8_t GetEventLocation()
		{
			return (eventInfo >> 8) & 0x1F;
		}
	};
	struct SummonParam {
		uint8_t  side;
		uint8_t  destZone;
		uint8_t  srcPlace;
		uint8_t  srcIndex;
		bool     faceUp;
		bool     atkPos;
		uint8_t  tribZone[3];
		uint8_t  tribSide[3];
		bool     tribUsed[3];

		SummonParam()
		{
			uint32_t p = *(uint32_t*)0x00A55080;

			side = p & 1;
			destZone = (p >> 1) & 0x1F;
			uint32_t src = (p >> 6) & 0xFF;
			srcIndex = src & 0xF;
			srcPlace = src >> 4;
			faceUp = (p & 0x4000) != 0;
			atkPos = (p & 0x8000) != 0;
			tribZone[0] = (p >> 16) & 7;
			tribZone[1] = (p >> 19) & 7;
			tribZone[2] = (p >> 22) & 7;
			tribUsed[0] = (p & 0x02000000) != 0;
			tribUsed[1] = (p & 0x04000000) != 0;
			tribUsed[2] = (p & 0x08000000) != 0;
			tribSide[0] = (p >> 28) & 1;
			tribSide[1] = (p >> 29) & 1;
			tribSide[2] = (p >> 30) & 1;
		}
	};
	struct Stats
	{
		uint32_t ATK;
		uint32_t DEF;
	};


	template <typename T>
	inline T GetFunction(std::uintptr_t address)
	{
		return reinterpret_cast<T>(address);
	}
	inline auto GetCardName = reinterpret_cast<char* (__cdecl*)(uint16_t intID)>(0x00402290);

	inline auto GetCardDescription = reinterpret_cast<char* (__cdecl*)(uint16_t intID)>(0x004022b0);

	inline auto GetMonsterType = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004025D0);

	inline auto GetCardSubType = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x00402740);

	inline auto GetMonsterAttribute = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x00402650);

	inline auto GetMonsterLevel = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004026c0);

	inline auto GetMonsterATK = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004027b0);

	inline auto GetMonsterDEF = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x00402800);

	inline auto GetCardID = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004022e0);

	inline auto GetCardIntID = reinterpret_cast<uint32_t(__cdecl*)(uint16_t cardID)>(0x00402460);

	inline auto GetSpellTrapType = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x00402700);

	inline auto GetCardPack = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004024e0);

	inline auto GetSpellSpeed = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x0057e030);

	inline auto ShowDialog = reinterpret_cast<void(__cdecl*)(const char*)>(0x005bf860);

	inline auto ShowDialog2 = reinterpret_cast<void(__cdecl*)(unsigned int)>(0x005bf860);

	inline auto ShowDialogOptions = reinterpret_cast<void(__cdecl*)(unsigned int, unsigned int)>(0x005bfa00);

	inline auto PayLifePoints = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount)>(0x005783b0);

	inline auto IsCardOnTheField = reinterpret_cast<uint32_t(__cdecl*)(uint16_t cardID)>(0x005699c0);

	// Generates target parameters and saves them to the target pointer
	inline auto SetTargetParams = reinterpret_cast<uint32_t(__cdecl*)(unsigned int block, int param2, unsigned int* target)>(0x005833e0);

	// Sends the card from the field to the destination based on the zone bit field (16 bit for each field, first 11 bits -> monster/st/fieldspell)
	// destCode: 0xb = hand, 0xd = deck, 0xe = grave, 0xf = banish
	// fxCode: bit field for effect, 0 = no effect, 2 = play destroy sound and visual effect
	inline auto SendCardFromField = reinterpret_cast<unsigned int(__cdecl*)(uint8_t * block, unsigned int zoneBitField, unsigned int destCode, unsigned int fxCode)>(0x005768b0);

	inline auto DiscardFromHand = reinterpret_cast<unsigned int(__cdecl*)(unsigned int player, unsigned int handIdx, unsigned int flag)>(0x005758c0);

	inline auto DealEffectDamage = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount)>(0x00578430);

	inline auto GetCardPtrFromLocation = reinterpret_cast<uint32_t * (__cdecl*)(unsigned int playerIdx, unsigned int destCode, unsigned int idx)>(0x00570040);

	inline auto BanishFromGrave = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int* cardPtr, unsigned int flag)>(0x00575f30);

	inline auto InitiateSelectionList = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int type, unsigned int cardID, unsigned int loc)>(0x0040c990);

	inline auto PopulateSelectionList = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int cardID, unsigned int param3)>(0x00599d70);

	inline auto GetSelectionListCount = reinterpret_cast<uint32_t(__cdecl*)()>(0x0040cb30);

	inline auto SelectionConfirmed = reinterpret_cast<uint32_t(__cdecl*)()>(0x0040cac0);

	inline auto GetSelectedItem = reinterpret_cast<uint32_t(__cdecl*)()>(0x0040cb00);

	inline auto SetSelectedItemIndex = reinterpret_cast<void(__cdecl*)(unsigned int index)>(0x0040cd00);

	// Stores a 16 bit value to the target block
	// Used once for field targets, twice for full card dwords stored in lists
	inline auto StoreTarget = reinterpret_cast<void(__cdecl*)(int block, uint16_t value)>(0x00592a40);

	inline auto QueueCommand = reinterpret_cast<void(__cdecl*)(unsigned short opCode, unsigned int arg0, unsigned int arg1, unsigned int flag)>(0x005b91e0);

	inline auto AddTargetedCardToHand = reinterpret_cast<void(__cdecl*)(uint8_t * block, unsigned int playerIdx, unsigned int* param3)>(0x00575ca0);

	inline auto NumOfEmptyValidSummonZones = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx)>(0x0056a000);

	inline auto NumOfTributableMonsters = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx, unsigned int param2)>(0x0056a200);

	inline auto CanPlayerSummon = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx)>(0x00570a90);

	inline auto IsCardOnField = reinterpret_cast<uint32_t(__cdecl*)(unsigned int cardID)>(0x005699c0);

	inline auto IsCardOnSideOfField = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int cardID)>(0x005699a0);

	inline auto BanishCardFromGrave = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int* cardDword)>(0x00575f30);

	inline auto NormalSummon = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int handIdx, unsigned int destZone, unsigned int packedTributes, int set)>(0x005ad710);

	inline auto GetSummonZone = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx)>(0x0056a030);

	inline auto IsFieldSelectionReady = reinterpret_cast<uint32_t(__cdecl*)(unsigned int mask)>(0x005aa410);

	inline auto MarkZoneAsTributed = reinterpret_cast<void(__cdecl*)(unsigned int side, unsigned int col)>(0x00486bb0);

	inline auto IsFieldSelectionConfirmed = reinterpret_cast<uint32_t(__cdecl*)()>(0x005aa450);

	inline auto TributeSelected = reinterpret_cast<uint32_t(__cdecl*)(unsigned int side, unsigned int col)>(0x00577a80);

	inline auto IsMonsterTributable = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int sideIdx, unsigned int zoneIdx)>(0x0056a0d0);

	inline auto CopyCard = reinterpret_cast<void(__cdecl*)(void* dest, void* src)>(0x005675c0);

	inline auto PayCostToSummon = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx)>(0x0059cfe0);

	inline auto SummonMonster = reinterpret_cast<void(__cdecl*)()>(0x005ad890);

	inline auto QueueEffect = reinterpret_cast<void(__cdecl*)(unsigned int pack, unsigned int inst, unsigned int extra)>(0x005ba500);

	inline auto ChainEffect = reinterpret_cast<void(__cdecl*)(unsigned int pack, unsigned int inst, unsigned int* source, unsigned int extra)>(0x005ba720);

	inline auto DrawCards = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount)>(0x00578ab0);

	inline auto SelectCardsToDiscard = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount, int param3, int param4)>(0x005bce30);

	inline auto IsCardProhibited = reinterpret_cast<uint32_t(__cdecl*)(unsigned int cardIntID, unsigned int param2)>(0x0056b440);

	inline auto GetCurrentATK = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx)>(0x0056f5e0);

	inline auto GetCurrentDEF = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx)>(0x0056f600);

	inline auto AddEffectEntityToZone = reinterpret_cast<void(__cdecl*)(unsigned int packedCard, unsigned int effectIntID, uint16_t effect)>(0x0056ab60);

	inline auto IsZoneValid = reinterpret_cast<uint32_t(__cdecl*)(unsigned int side, unsigned int zoneIdx)>(0x00569e10);

	inline auto ChangeMonsterPosition = reinterpret_cast<void(__cdecl*)(uint8_t * block, unsigned int playerIdx, unsigned int zoneIdx, unsigned int set, unsigned int param5)>(0x00575130);

	inline auto ToggleMonsterPosition = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx, unsigned int toggleSet, unsigned int param4, unsigned int param5)>(0x00575160);

	inline auto ToggleFaceUp = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx, int param3, unsigned int effectIntID)>(0x00574ec0);

	inline auto FlashCardPortrait = reinterpret_cast<void(__cdecl*)(unsigned int side, unsigned int cardIntID, unsigned int zone)>(0x005782e0);

	inline auto HasEffectEntiry = reinterpret_cast<int(__cdecl*)(unsigned int sideIdx, unsigned int zoneIdx, unsigned int effectID)>(0x0056da20);

	inline auto GetInstIndexInGrave = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx, int inst)>(0x00568ad0);

	inline auto GetInstIndexInHand = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx, int inst)>(0x00568b30);

	inline auto MillCards = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount, int fxFlag)>(0x00578b00);

	inline auto CanBeSummonedByEffect = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int cardIntID)>(0x00570ac0);

	// Returns the index of the card in the spell/trap zone if it is face up, otherwise returns -1
	inline auto IsCardFaceUpInSpellTrapZones = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx, unsigned int cardID)>(0x00569720);

	inline auto CanBeRevived = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* cardDWORD)>(0x00568bd0);

	inline auto CanBeRevivedFromGrave = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int graveIdx)>(0x00599d40);

	inline auto DiscardRandomCard = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int param2, unsigned int amount)>(0x005bcec0);

	inline auto GainLP = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount)>(0x00578740);

	inline auto GetRandomNumber = reinterpret_cast<int(__cdecl*)(int max)>(0x005bde20);

	inline auto SelectCardInOppHand = reinterpret_cast<void(__cdecl*)(unsigned int activatingPlayerIdx)>(0x005bcf30);

	inline auto DisCardSelectedHandIndex = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int handIdx, unsigned int param3)>(0x005758c0);

	inline auto MarkCardForFusion = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, Location location, unsigned int selectedColumn)>(0x00486bf0);

	// top = 1 -> put on top of deck
	// top = 0 -> put on bottom of deck
	inline auto PutCardFromHandToDeck = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int handIdx, unsigned int top)>(0x005757f0);

	// SelectionType:
	// 0 = OK
	// 1 = Yes/No
	// 2 = 2-way effect selection
	// 3 = 3-way effect selection
	// 4 = card type
	// 5 = attribute
	// 6 = atk/def position
	// 7 = coin side
	inline auto SetupSelector = reinterpret_cast<void(__cdecl*)(uint8_t selectionType, uint16_t cardID)>(0x005bfa00);

	inline auto InitiateSelector = reinterpret_cast<void(__cdecl*)()>(0x005bfa20);

	inline auto IssueCommand = reinterpret_cast<void(__cdecl*)(unsigned int cmd, unsigned int src, unsigned int dest, unsigned int param3)>(0x005b91e0);

	inline auto CanCardBeTargeted = reinterpret_cast<bool(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx)>(0x0056c510);

	// position = 0 : DEF, position = 1 : ATK
	// summonType = 0 : Normal Summon, summonType = 1 : Special Summon
	// returns 1 when the SM finished, 0 otherwise
	inline auto SummonStateMachine = reinterpret_cast<uint32_t(__cdecl*)(int position, int summonType, int consumeNormal)>(0x0059dee0);

	inline auto GetEmptySpellTrapZoneIdx = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx)>(0x0056a400);

	inline auto MoveCardOnTheField = reinterpret_cast<bool(__cdecl*)(uint8_t * block, int srcSide, char srcZone, int destSide, int destZone)>(0x00577bf0);

	// equipment, target = packed location (zone << 8 | side)
	// mode = 1 : regular equip, 5 : absorbed monster
	inline auto EquipCard = reinterpret_cast<void(__cdecl*)(int targetIdx, uint16_t equipment, uint16_t target, int mode)>(0x00574900);

	inline auto ClearTributeMarks = reinterpret_cast<void(__cdecl*)()>(0x00486c30);

	inline auto RemoveEffectEntity = reinterpret_cast<void(__cdecl*)(unsigned int side, unsigned int zone, int effectIdx)>(0x005695e0);

	inline auto IndexOfZoneEffect = reinterpret_cast<int(__cdecl*)(unsigned int sideIdx, unsigned int zoneIdx, unsigned int effectID)>(0x0056d990);

	inline auto GetVersionMask = reinterpret_cast<int(__cdecl*)()>(0x005beac0);

	inline auto CanAddToDeck = reinterpret_cast<bool(__cdecl*)(uint16_t cardIntID)>(0x005be260);

	inline auto GetLimitedStatus = reinterpret_cast<int(__cdecl*)(uint16_t cardIntID)>(0x005be200);

	inline auto GetThisPack = reinterpret_cast<int(__cdecl*)()>(0x005beaa0);

	inline auto HasInherentSummon = reinterpret_cast<uint32_t(__cdecl*)(uint16_t cardIntID)>(0x00567a00);

	// Highlights and stores a target from the field
	inline auto TargetCard = reinterpret_cast<void(__cdecl*)(unsigned int* param, unsigned int side, unsigned int zone)>(0x00592a80);

	inline auto CanCardRespond = reinterpret_cast<uint32_t(__cdecl*)(EffectBlock* self, EffectBlock * source)>(0x0057e5c0);


	using FUN_591A00_t = uint32_t(__cdecl*)(uint32_t player, uint32_t matId, uint32_t excl1, uint32_t excl2);
	using FUN_591C90_t = uint32_t(__cdecl*)(uint32_t player, uint32_t packed);
	using FUN_568580_t = int(__cdecl*)(uint32_t cardIntId);
	using FUN_56A030_t = int(__cdecl*)(uint32_t player);
	using FUN_569E10_t = int(__cdecl*)(uint32_t player, uint32_t zone);
	using FUN_56c510_t = bool(__cdecl*)(unsigned int playerIdx, int zoneIdx);
	using FUN_5777d0_t = uint32_t(__cdecl*)(uint8_t* block, unsigned int playerIdx, unsigned int zoneIdx);
	using FUN_579880_t = void(__cdecl*)(unsigned int playerIdx, unsigned int param2, unsigned int param3);
	using FUN_5b91e0_t = void(__cdecl*)(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4);

	static FUN_591A00_t  FUN_00591A00 = (FUN_591A00_t)0x00591A00;
	static FUN_591C90_t  FUN_00591C90 = (FUN_591C90_t)0x00591C90;
	static FUN_568580_t  FUN_00568580 = (FUN_568580_t)0x00568580;
	static FUN_56A030_t  FUN_0056A030 = (FUN_56A030_t)0x0056A030;
	static FUN_569E10_t  FUN_00569E10 = (FUN_569E10_t)0x00569E10;
	static FUN_56c510_t  FUN_0056C510 = (FUN_56c510_t)0x0056C510;
	static FUN_5777d0_t  FUN_005777D0 = (FUN_5777d0_t)0x005777D0;
	static FUN_579880_t  FUN_00579880 = (FUN_579880_t)0x00579880;
	static FUN_5b91e0_t  FUN_005b91e0 = (FUN_5b91e0_t)0x005b91e0;

	// From hand
	inline void SpecialSummon(unsigned int playerIdx, unsigned int handIdx, unsigned int desZone, unsigned int PackTributes, unsigned int pos)
	{
		auto fun = reinterpret_cast<void(__cdecl*)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)>(0x005adbc0);

		fun(playerIdx, handIdx, desZone, PackTributes, pos);
	}
	// With position selection
	inline void SpecialSummon(unsigned int side, unsigned int* cardPtr, unsigned int posSelectorType, unsigned int flags, unsigned int srcLoc, unsigned int owner)
	{
		auto fun = reinterpret_cast<void(__cdecl*)(unsigned int, unsigned int*, unsigned int, unsigned int, unsigned int, unsigned int)>(0x005adae0);

		fun(side, cardPtr, posSelectorType, flags, srcLoc, owner);
	}
	// With specified position
	inline void SpecialSummon(unsigned int side, unsigned int* cardPtr, unsigned int faceUp, unsigned int pos, unsigned int flags, unsigned int srcLoc, unsigned int owner)
	{
		auto fun = reinterpret_cast<void(__cdecl*)(unsigned int, unsigned int*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)>(0x005ad9f0);

		fun(side, cardPtr, faceUp, pos, flags, srcLoc, owner);
	}

	// Wrapers
	inline bool W_CanSummon(uint8_t playerIdx)
	{
		if (FUN::CanPlayerSummon(playerIdx) == 0) return false;
		if (FUN::NumOfEmptyValidSummonZones(playerIdx) == 0) return false;

		return true;
	}
	inline void W_RevealTopCard(uint8_t side, uint16_t cardID, uint32_t skip)
	{
		FUN::QueueCommand((side ? 0x8000u : 0u) | 0x20, cardID, 0x0D, skip);
		FUN::QueueCommand((side ? 0x8000u : 0u) | 0xf1, 0x1E, 0, 0);
	}
	inline void W_ShowDialog(const char* dlgText, DialogMode mode, uint16_t cardID = 0)
	{
		ShowDialog(dlgText);
		SetupSelector(mode, cardID);
		InitiateSelector();
	}
	inline void W_AddEffectEntityToZone(uint32_t playerIdx, uint32_t zoneIdx, uint32_t effectIntID, uint16_t effect)
	{
		FUN::AddEffectEntityToZone((zoneIdx << 8) | playerIdx, effectIntID, effect);
	}
	inline uint32_t W_RollDice(uint16_t* param, uint8_t diceCmd = 0xe2)
	{
		int roll = FUN::GetRandomNumber(6) + 1;
		FUN::QueueCommand((param[1] << 15) | diceCmd, roll, (param[1] >> 1) & 0x1F, 0);

		return roll;
	}
	inline void W_RollRiggedDice(uint32_t playerIdx, uint32_t sideIdx, uint8_t roll, uint8_t diceCmd = 0xe2)
	{
		FUN::QueueCommand((sideIdx << 15) | diceCmd, roll, (playerIdx >> 1) & 0x1F, 0);
	}
	inline void W_PutCardFromLocationToDeck(uint32_t playerIdx, Location location, uint32_t idx, bool top)
	{
		if (location < 0xa) return;

		uint32_t* cardDword = (uint32_t*)FUN::GetCardPtrFromLocation(playerIdx, location, idx);
		uint8_t x = (uint8_t)(*cardDword >> 0xc);

		FUN::QueueCommand(
			0x8e,
			((((uint16_t)(char)(*cardDword >> 0x18) * 2 + (x & 1)) << 8) | (uint8_t)(char)playerIdx) & 0xff01 | 0x16,
			((((uint16_t)(top == 0)) << 8) | (uint8_t)x) & 0xff01 | 0x1a,
			0
		);
	}
	inline void W_PutCardFromFieldToDeck(uint32_t sideIdx, uint32_t zoneIdx, bool top)
	{
		uint32_t* cardDword = (uint32_t*)(0x00A55D64 + sideIdx * 0xd44 + 0x10 + zoneIdx * 0x90);

		uint8_t x = (uint8_t)(*cardDword >> 0xc);
		FUN::QueueCommand(
			0x8e,
			((((uint16_t)(char)(*cardDword >> 0x18) * 2 + (x & 1)) << 8) | (uint8_t)(char)sideIdx) & 0xff01 | 0x16,
			((((uint16_t)(top == 0)) << 8) | (uint8_t)x) & 0xff01 | 0x1a,
			0
		);
	}
	inline uint32_t W_RollDice(uint32_t playerIdx, uint32_t sideIdx, uint8_t diceCmd = 0xe2)
	{
		int roll = FUN::GetRandomNumber(6) + 1;
		FUN::QueueCommand((sideIdx << 15) | diceCmd, roll, (playerIdx >> 1) & 0x1F, 0);

		return roll;
	}
	inline void W_MoveCard(uint32_t cardDword, uint8_t _src, uint8_t _dest)
	{
		uint8_t owner = (cardDword >> 12) & 1;
		uint8_t inst = (uint8_t)(((cardDword >> 24) & 0x7F) * 2 + ((cardDword >> 12) & 1));

		uint32_t src = ((uint32_t)inst << 8) | (_src << 1) | (owner & 1);

		uint32_t dest = (_dest << 1) | (owner & 1);


		// CMD 0x8E = move
		QueueCommand(0x8E, src, dest, 0);

	}
	inline void W_NegateActivation(unsigned int* source, bool destroy)
	{
		Param src(source);

		// Full location halfword (side + place + flags)
		uint16_t locWord = *(uint16_t*)(src.block + 2);

		uint16_t opcode = (uint16_t)((locWord << 15) | 0xB1);

		uint16_t place = src.location;

		QueueCommand(opcode, place, 1, 0);

		src.block[4] |= 0x0E; // cancel resolve on their block

		if (destroy)
		{
			FieldMaskGenerator maskGen;
			maskGen.zones[src.playerIdx][src.zoneIdx] = true;

			uint8_t block[32] = {};
			SendCardFromField(block, maskGen.GenerateMask(), 0xe, 2);
		}
	}
	inline void W_StoreFieldTarget(unsigned int* param, uint16_t target)
	{
		FUN::StoreTarget((int)param, target);
	}
	inline void W_StoreListTarget(unsigned int* param, uint32_t dword)
	{
		FUN::StoreTarget((int)param, (uint16_t)dword);
		FUN::StoreTarget((int)param, (uint16_t)(dword >> 16));
	}
	inline uint8_t W_GetZoneSide(unsigned int* zoneAddress)
	{
		uint32_t offset = (uint32_t)zoneAddress - 0x00A55D74;

		return offset / 0xD44;
	}
	inline uint8_t W_GetZoneIndex(unsigned int* zoneAddress)
	{
		uint32_t offset = (uint32_t)zoneAddress - 0x00A55D74;

		return (offset % 0xD44) / 0x90;
	}
	inline void W_HighlightAndStoreTarget(unsigned int* param, unsigned int* cardAddress, Location location)
	{
		if (location > Location::FIELDZONE)
		{
			uint32_t dword = *cardAddress;
			uint8_t  owner = (dword >> 12) & 1;
			uint32_t inst = owner + ((dword >> 24) & 0x7F) * 2;
			uint32_t sideBit = owner ? 0x8000u : 0;

			uint32_t cardId = FUN::GetCardID(dword & 0xFFF);

			// Highlight / reveal
			FUN::QueueCommand(sideBit | 0xDF, cardId, inst, 0);
			FUN::QueueCommand(sideBit | 0x08, owner, location, 0);

			FUN::W_StoreListTarget(param, dword);
		}
		else
		{
			FUN::TargetCard(param, W_GetZoneSide(cardAddress), W_GetZoneIndex(cardAddress));
		}
	}
	inline void W_SS_HandToOpp(uint32_t handPlayer, int handIndex, uint32_t destZone, uint32_t extra, int posArg)
	{
		// This is a reimplementation of FUN_SpecialSummonFromHand, but modified so it summons to the opponent's field instead of the player's field.

		if ((int)destZone < 0) return;

		const uint32_t src = handPlayer & 1;
		const uint32_t dst = src ^ 1;

		const uint8_t posFlag = (uint8_t)(((uint32_t)(posArg == 0) << 9) >> 8);
		uint32_t mid = ((uint32_t)((posFlag << 8) | (uint8_t)handIndex) | 0x100);
		mid = (mid << 5 | (destZone & 0x1F)) << 1;

		uint32_t summonParam = Utils::ReadUint32((void*)0x00A55080);

		uint32_t kept = (summonParam & 0xFFFF4000) ^ dst;

		if (extra == 0) summonParam = mid | (kept & 0xF1FFFFFF);
		else
		{
			const uint8_t b8 = (uint8_t)(extra >> 8);
			const uint8_t b16 = (uint8_t)(extra >> 16);
			uint32_t hi =
				((((((((b16 & 0x10) << 1 | (b8 & 0x10)) << 1 | (extra & 0x10)) << 2
					| (b16 & 7)) << 2
					| (b16 & 0x80)) << 1
					| (b8 & 0x87)) << 1
					| (extra & 0x80)) << 2
					| (extra & 7));
			summonParam = (hi << 16) | mid | (kept & 0x8000FFFF);
		}

		Utils::WriteUint32((void*)0x00A55080, summonParam);

		const uintptr_t handAddr = 0x00A56434 + (uint32_t)(handIndex + (int)src * 0x351) * 4;

		const uint32_t handDword = Utils::ReadUint32((void*)handAddr);
		const uint32_t old84 = Utils::ReadUint32((void*)0x00a55084);
		Utils::WriteUint32((void*)0x00a55084, (handDword & 0xFFF) | (old84 & 0xFFFF0000));

		CopyCard((void*)0x00A55088, (void*)handAddr);

		const uint16_t old8e = Utils::ReadUint16((void*)0x00A5508E);
		Utils::WriteUint16((void*)0x00A5508E, (uint16_t)((old8e & 0xFFFB) | 0x18));
		Utils::WriteUint16((void*)0x00A5508C, 5);

		PayCostToSummon(src);

		SummonMonster();
	}

	inline bool W_BothLocked(uint32_t player, uint16_t a, uint16_t b)
	{
		uint32_t idA = FUN_00591C90(player, a) & 0xFFFF;
		uint32_t idB = FUN_00591C90(player, b) & 0xFFFF;
		return FUN_00568580(idA) != 0 && FUN_00568580(idB) != 0;
	}

	inline bool W_FieldCanFreeZone(uint32_t player, uint16_t packed)
	{
		if ((packed & 0x4000) == 0) return false;
		return FUN_00569E10(player, packed & 0xFFF) != 0;
	}

	// Effects
	inline auto DestroyEffect = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* param, unsigned int* param2, int param3)>(0x00585C10);

	inline auto TargetFieldCard = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* param, int param2, int param3)>(0x00596570);

	// Misc.
	inline void __fastcall DrawSpellCounter(int actor, int renderer, int spriteIndex)
	{
		if (!actor) return;

		uint16_t loc = *(uint16_t*)(actor + 0x24);
		uint8_t  side = loc & 1;
		uint8_t  place = ((loc & 0x3E) >> 1) + ((loc >> 6) & 0xFF);
		(void)place;

		if (place > 10) return;

		uint8_t* zone = (uint8_t*)(0x00A55D64 + side * 0xd44 + 0x10 + place * 0x90);
		uint16_t intId = *(uint16_t*)(zone) & 0xfff;
		uint16_t status = *(uint8_t*)(zone + 0x6);

		if (intId == 0 || (status & 2) == 0) return;

		uint8_t n = HasEffectEntiry(side, place, 0x96);

		if (n < 1) return;

		int* atlas = *(int**)0x005F2FD8;
		if (!atlas) return;

		int x = *(int*)(actor + 0x30) - 15 * (1 - (2 * side));
		int y = *(int*)(actor + 0x34) + 65 * (1 - (2 * side));
		int local = *(uint8_t*)0x00A54E5C & 1;
		int flip = ((~local & 1) ^ (~side & 1)) * -2 + 1;
		y += flip * ((*(int*)0x005F3008) / 2 - 5);

		int id = spriteIndex;

		int* getter = (int*)atlas[0x14];
		auto select = *(void(__thiscall**)(int*, int*))(*getter);
		select(getter, &id);

		auto bind = *(void(__thiscall**)(int*, int, int))(*atlas + 8);
		bind(atlas, id, -1);

		int* flags = (int*)atlas[0x0A];
		if (flags) flags[id] = 0;

		int table = atlas[0x18];
		int spriteObj = table ? *(int*)(table + id * 8 + 4) : 0;
		if (!spriteObj) return;

		if (renderer) {
			int* rv = *(int**)renderer;
			auto blit = *(void(__thiscall**)(int, int, int, int, int, int))(rv + 0x50 / 4);
			blit(renderer, spriteObj, x, y, 0, 0x100);
		}

		if (actor == 0 || renderer == 0) return;

		if (n == 0) return;

		int fontBox = actor + 0xAC;

		int* fontAtlas = *(int**)0x005F33EC;
		auto pickStyle = (uint32_t(__thiscall*)(int*, int))0x0040EB30;
		*(uint32_t*)(fontBox + 4) = pickStyle(fontAtlas, 8);
		*(int*)(fontBox + 8) = 10;

		auto drawNum = (int(__thiscall*)(
			int /*this = fontBox*/,
			int /*renderer*/,
			int /*x, digits grow left*/,
			int /*y*/,
			uint32_t /*value*/,
			int /*color*/,
			int /*flag*/,
			int /*digitW, 0 = auto*/))0x00480470;

		drawNum(fontBox, renderer, x + 16, y - 5, n, 0xA0, 0, 0);
	}
}
