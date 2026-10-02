#include "HookManager.h"
#include "FUN.h"

using namespace Utils;
uint16_t* collection = nullptr;
uint32_t collectionStart;
uint32_t collectionEnd;
uint16_t collectionSize;

const uint32_t STOCK_COLLECTION_START = 0x00a53ccc;
const uint32_t STOCK_COLLECTION_END = 0x00a54582;

void* gLibCanAddToDeckTrampoline = nullptr;

uint16_t GetCollectionSize();
auto FUN_00464fc0 = (void(__thiscall*)(uint32_t, uint32_t, uint32_t, uint32_t))0x00464fc0;

void HookManager::InstallLibraryHooks()
{
	collectionSize = GetCollectionSize();

	collection = HookManager::cardCollection;
	collectionStart = (uint32_t)collection;
	collectionEnd = collectionStart + (collectionSize * 2);
	



	Hook hLibCardIntIDs = InstallHook((void*)0x00402460, 6, PatchGetCardIntID);
	Hook hLibCardIDs = InstallHook((void*)0x004022e0, 6, PatchGetCardID);

	Hook hLibLoadAlbum = InstallHook((void*)0x0047db50, 5, PatchLoadAlbum);
	Hook hLibRebuildDeck = InstallHook((void*)0x005beae0, 5, PatchRebuildDeck);
	Hook hLibLoadDeckBuilder1 = InstallHook((void*)0x004640aa, 5, PatchLoadDeckBuilder1);
	Hook hLibLoadDeckBuilder2 = InstallHook((void*)0x004640ce, 5, PatchLoadDeckBuilder2);

	//Hook hLibValidateMainDeck = InstallHook((void*)0x005be969, 7, PatchValidateMainDeck);
	//Hook hLibValidateSideDeck = InstallHook((void*)0x005be9dd, 7, PatchValidateSideDeck);
	//Hook hLibValidateExtraDeck = InstallHook((void*)0x005bea35, 8, PatchValidateExtraDeck);
	//Hook hLibClearCollectionFlags = InstallHook((void*)0x005bed60, 5, PatchClearCollectionFlags);

	Hook hLibCanAddToDeck = InstallHook((void*)0x005be271, 9, PatchCanAddToDeck);
	gLibCanAddToDeckTrampoline = hLibCanAddToDeck.Trampoline;

	//Hook hLibFUN_004673e0 = InstallHook((void*)0x004673e0, 5, PatchFUN_004673e0);
}
uint16_t GetCollectionSize()
{
	int size = std::filesystem::file_size("data/bin#/card_id.bin");
	return (uint16_t)size / 2;
}
uint16_t __stdcall HookManager::M_GetCardIntID(uint16_t cardID)
{
	cardID &= 0xFFF;
	if (cardID > 0xfff) return 0;
	uint16_t* INTID_LIST = *(uint16_t**)0x005f2418;

	return INTID_LIST[cardID];
}
__declspec(naked) void HookManager::PatchGetCardIntID()
{
	__asm
	{
		PUSH DWORD PTR DS : [ESP + 4]
		CALL HookManager::M_GetCardIntID
		PUSH 0x004024cb
		RET
	}
}
uint16_t __stdcall HookManager::M_GetCardID(uint16_t cardIntID)
{
	cardIntID &= 0xFFF;
	if (cardIntID > 0xfff) return 0;
	uint16_t* CARDID_LIST = *(uint16_t**)0x005F2450;

	return CARDID_LIST[cardIntID];
}
__declspec(naked) void HookManager::PatchGetCardID()
{
	__asm
	{
		PUSH DWORD PTR DS : [ESP + 4]
		CALL HookManager::M_GetCardID
		PUSH 0x00402321
		RET
	}
}
void __stdcall HookManager::M_LoadAlbum(uint32_t album)
{
	uint16_t thisPack = 0;
	if (*(char*)(album + 0x43c) == 0) thisPack = FUN::GetThisPack();
	else thisPack = FUN::GetVersionMask();

	int& refCount = *(int*)(album + 0x434);
	int& refPages = *(int*)(album + 0x438);
	int& refOwned = *(int*)(album + 0x430);	

	refCount = 0;

	// Load available cards for this game version
	for (uint32_t i = 1; i < 1116; i++)
	{
		uint16_t pack = FUN::GetCardPack(i);
		if ((pack & thisPack) != 0)
		{
			refCount++;
		}
	}
	// Set the number of pages in the album
	refPages = (refCount + 0x31) / 0x32;

	auto FUN_0047f840 = (void(__thiscall*)(void*, int))0x0047f840;
	FUN_0047f840((void*)(album + 0x444), refCount);
	
	refOwned = 0;
	int* list = *(int**)(album + 0x448);

	int n = 0;
	for (uint32_t i = 1; i < 1116; i++)
	{
		uint16_t pack = FUN::GetCardPack(i);
		if ((pack & thisPack) != 0)
		{
			list[n++] = i;
			if (i > 1114) *(uint16_t*)(0x00A53CCC + i * 2) = collection[i];

			if (i > 1114) refOwned++;
			else
			{
				uint8_t amount = (uint8_t)(collection[i] & 0xFF);
				if (amount != 0)
				{
					refOwned++;
				}
			}
		}
	}
	auto FUN_005c3f59 = (void(__cdecl*)(int*, int, int, void*))0x005c3f59;
	FUN_005c3f59(list, refCount, 4, (void*)0x0043aeb0);
}
__declspec(naked) void HookManager::PatchLoadAlbum()
{
	__asm
	{
		PUSH ECX
		CALL HookManager::M_LoadAlbum
		PUSH 0x0047dc47
		RET
	}
}
bool __stdcall HookManager::M_RebuildDeck()
{
	for (size_t i = 0; i < 4096; i++)
	{
		if (i < 1115) collection[i] = *(uint16_t*)(STOCK_COLLECTION_START + i * 2);
		else if (i < collectionSize)
		{
			if (collection[i] == 0) collection[i] = 0x4003;
		}
	}

	for (uint32_t i = 0; i < collectionSize; i++)
	{
		collection[i] &= 0xC0FF;
	}
	for (uint32_t i = 0; i < 1115; i++)
	{
		*(uint16_t*)(STOCK_COLLECTION_START + i * 2) &= 0xC0FF;
	}
	
	uint16_t& refMainDeckSize = *(uint16_t*)0x00a54f3c;
	uint16_t& refSideDeckSize = *(uint16_t*)0x00a54f3e;
	uint16_t& refExtraDeckSize = *(uint16_t*)0x00a54f40;

	uint16_t* mainDeck = (uint16_t*)0x00a54e60;
	uint16_t* sideDeck = (uint16_t*)0x00a54f00;
	uint16_t* extraDeck = (uint16_t*)0x00a54f1E;

	bool deckModified = false;

	if (refMainDeckSize != 0)
	{
		for (int i = 0; i < refMainDeckSize;)
		{
			uint16_t card = mainDeck[i];
			uint16_t pack = FUN::GetCardPack(card);
			uint16_t mask = FUN::GetVersionMask();
			bool canAdd = FUN::CanAddToDeck(card);
			uint8_t amount = (uint8_t)(collection[card] & 0xFF);
			uint8_t flags = (uint8_t)((collection[card] >> 8) & 0xFF) & 3;

			//if (card < 1115 && ((pack & mask) == 0 || !canAdd || amount <= flags))
			//{
			//	refMainDeckSize--;
			//	mainDeck[i] = mainDeck[refMainDeckSize];
			//	deckModified = true;
			//}
			//else
			//{
			//	uint16_t updated = ((collection[card] & 0xff00) + 0x100 ^ collection[card]) & 0x300 ^ collection[card];
			//	collection[card] = updated;
			//	if (card < 1115) *(uint16_t*)(STOCK_COLLECTION_START + card * 2) = updated;
			//	i++;
			//}
			uint16_t updated = ((collection[card] & 0xff00) + 0x100 ^ collection[card]) & 0x300 ^ collection[card];
			collection[card] = updated;
			if (card < 1115) *(uint16_t*)(STOCK_COLLECTION_START + card * 2) = updated;
			i++;
		}
	}

	if (refExtraDeckSize != 0)
	{
		for (int i = 0; i < refExtraDeckSize;)
		{
			uint16_t card = extraDeck[i];
			uint16_t pack = FUN::GetCardPack(card);
			uint16_t mask = FUN::GetVersionMask();
			bool canAdd = FUN::CanAddToDeck(card);

			uint8_t amount = (uint8_t)(collection[card] & 0xFF);
			uint8_t flags = (uint8_t)((collection[card] & 0x3000) >> 12);
			//if (card < 1115 && ((pack & mask) == 0 || !canAdd || amount <= flags))
			//{
			//	refExtraDeckSize--;
			//	extraDeck[i] = extraDeck[refExtraDeckSize];
			//	deckModified = true;
			//}
			//else
			//{
			//	uint16_t updated = ((collection[card] & 0xf000) + 0x1000 ^ collection[card]) & 0x3000 ^ collection[card];
			//	collection[card] = updated;
			//	if (card < 1115) *(uint16_t*)(STOCK_COLLECTION_START + card * 2) = updated;
			//	i++;
			//}
			uint16_t updated = ((collection[card] & 0xf000) + 0x1000 ^ collection[card]) & 0x3000 ^ collection[card];
			collection[card] = updated;
			if (card < 1115) *(uint16_t*)(STOCK_COLLECTION_START + card * 2) = updated;
			i++;

		}
	}

	if (refSideDeckSize != 0)
	{
		for (int i = 0; i < refSideDeckSize;)
		{
			uint16_t card = sideDeck[i];
			uint16_t pack = FUN::GetCardPack(card);
			uint16_t mask = FUN::GetVersionMask();
			bool canAdd = FUN::CanAddToDeck(card);

			uint8_t amount = (uint8_t)(collection[card] & 0xFF);
			uint8_t flags = (uint8_t)((collection[card] & 0xc00) >> 10);
			//if (card < 1115 && ((pack & mask) == 0 || !canAdd || amount <= flags))
			//{
			//	refSideDeckSize--;
			//	sideDeck[i] = sideDeck[refSideDeckSize];
			//	deckModified = true;
			//}
			//else
			//{
			//	uint16_t updated = ((collection[card] & 0xfc00) + 0x400 ^ collection[card]) & 0xc00 ^ collection[card];
			//	collection[card] = updated;
			//	if (card < 1115) *(uint16_t*)(STOCK_COLLECTION_START + card * 2) = updated;
			//	i++;
			//}
			uint16_t updated = ((collection[card] & 0xfc00) + 0x400 ^ collection[card]) & 0xc00 ^ collection[card];
			collection[card] = updated;
			if (card < 1115) *(uint16_t*)(STOCK_COLLECTION_START + card * 2) = updated;
			i++;

		}
	}
	return deckModified;
}
__declspec(naked) void HookManager::PatchRebuildDeck()
{
	__asm
	{
		CALL HookManager::M_RebuildDeck
		PUSH 0x005bed58
		RET
	}
}
__declspec(naked) void HookManager::PatchLoadDeckBuilder1()
{
	__asm
	{
		MOV EAX, collectionStart
	hook_loop :
		CMP BYTE PTR DS : [EAX] , 0
		JZ hook_skip
		INC ECX
	hook_skip :
		ADD EAX, 0x2
		CMP EAX, collectionEnd
		JL hook_loop
		PUSH 0x004640bf
		RET
	}
}
__declspec(naked) void HookManager::PatchLoadDeckBuilder2()
{
	__asm
	{
		MOV ESI, collectionStart
	hook_loop:
		CMP BYTE PTR DS : [ESI], 0
		JZ hook_skip
		PUSH EDI
		CALL FUN::GetCardPack
		ADD ESP, 0x4
		MOV BX, AX
		CALL FUN::GetVersionMask
		AND EBX, EAX
		TEST BX, BX
		JZ hook_skip
		XOR EAX, EAX
		MOV AX, WORD PTR DS : [ESI]
		MOV ECX, EAX
		MOV EDX, EAX
		SHR ECX, 0xc
		AND ECX, 0x3
		AND EDX, 0xff
		SUB EDX, ECX
		MOV ECX, EAX
		SHR ECX, 0xa
		AND ECX, 0x3
		SHR EAX, 0x8
		SUB EDX, ECX
		AND EAX, 0x3
		SUB EDX, EAX
		MOV ECX, EBP
		PUSH EDX
		MOV EDX, DWORD PTR DS : [ESP + 0x14]
		PUSH EDI
		PUSH EDX
		CALL FUN_00464fc0

	hook_skip:
		ADD ESI, 0x2
		INC EDI
		CMP ESI, collectionEnd
		JL hook_loop
		PUSH 0x00464133
		RET
	}
}
__declspec(naked) void HookManager::PatchValidateMainDeck()
{
	__asm
	{
		LEA ECX, [EAX * 0x2 + collectionStart]
		MOV AX, WORD PTR DS : [EAX * 0x2 + collectionStart]
		PUSH 0x005be978
		RET
	}
}
__declspec(naked) void HookManager::PatchValidateSideDeck()
{
	__asm
	{
		LEA ECX, [EAX * 0x2 + collectionStart]
		MOV AX, WORD PTR DS : [EAX * 0x2 + collectionStart]
		PUSH 0x005be9ec
		RET
	}
}
__declspec(naked) void HookManager::PatchValidateExtraDeck()
{
	__asm
	{
		MOV SI, WORD PTR DS : [EAX * 0x2 + collectionStart]
		LEA EBX, [EAX * 0x2 + collectionStart]
		PUSH 0x005bea44
		RET
	}
}
__declspec(naked) void HookManager::PatchClearCollectionFlags()
{
	__asm
	{
		MOV EAX, collectionStart
	hook_loop:
		AND WORD PTR DS : [EAX], 0xc0ff
		ADD EAX, 0x2
		CMP EAX, collectionEnd
		JL hook_loop
		PUSH 0x005bed74
		RET
	}
}
__declspec(naked) void HookManager::PatchCanAddToDeck()
{
	__asm
	{
	hook:
		CMP ECX, 1115
		JL hook_end
		MOV AL, 1
		POP EDI
		POP ESI
		POP EBX
		PUSH 0x005be82d
		RET

	hook_end:
		JMP[gLibCanAddToDeckTrampoline]
	}
}

__declspec(naked) void HookManager::PatchRebuildDeck1()
{
	__asm
	{
		MOV EAX, collectionStart
		PUSH 0x005beaf0
		RET
	}
}
__declspec(naked) void HookManager::PatchFUN_004673e0()
{
	__asm
	{
		PUSH 0x0046748e
		RET
	}
}
