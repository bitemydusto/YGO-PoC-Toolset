#include "Debugger.h"

GameData::Duel* duel = GameData::GetDuel();

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
uint32_t CallFunction(uintptr_t address, const std::vector<uint32_t>& args)
{
    switch (args.size())
    {
        case 0:
        {
            auto fn = reinterpret_cast<Fn0>(address);
            return fn();
        }
        case 1:
        {
            auto fn = reinterpret_cast<Fn1>(address);
            return fn(args[0]);
        }
        case 2:
        {
            auto fn = reinterpret_cast<Fn2>(address);
            return fn(args[0], args[1]);
        }
        case 3:
        {
            auto fn = reinterpret_cast<Fn3>(address);
            return fn(args[0], args[1], args[2]);
        }
        case 4:
        {
            auto fn = reinterpret_cast<Fn4>(address);
            return fn(args[0], args[1], args[2], args[3]);
        }
        case 5:
        {
            auto fn = reinterpret_cast<Fn5>(address);
            return fn(args[0], args[1], args[2], args[3], args[4]);
        }
        case 6:
        {
            auto fn = reinterpret_cast<Fn6>(address);
            return fn(args[0], args[1], args[2], args[3], args[4], args[5]);
        }
    }
}
void Start()
{
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);

    std::cout << "Debugger ready.\n";

    std::string line;

    while (std::getline(std::cin, line))
    {
        std::stringstream ss(line);

        std::string command;
        ss >> command;

        if (command.empty())
        {
            continue;
        }

        if (command == "help")
        {
            std::cout << "call <address> [arg0] [arg1] ... [arg5]\n";
			std::cout << "mill <player> <amount>\n";
			std::cout << "draw <player> <amount>\n";
            std::cout << "add <cardID>\n";
            std::cout << "roll <player> <side> <diceCmd>\n";
			std::cout << "ss <player> <cardID>\n";
            continue;
        }
        if (command == "ss")
        {
			std::string playerIdxString,cardIDString;
            if (!(ss >> playerIdxString >> cardIDString))
            {
				std::cout << "Usage: ss <player> <cardID>\n";
				continue;
            }
			uint8_t playerIdx = static_cast<uint8_t>(std::stoull(playerIdxString, nullptr, 0));
			uint16_t cardID = static_cast<uint16_t>(std::stoull(cardIDString, nullptr, 0));
			SummonCommand(playerIdx, cardID);
            continue;
        }
		if (command == "roll")
		{
			uint32_t playerIdx, sideIdx;
			std::string diceCmdString;
			if (!(ss >> playerIdx >> sideIdx >> diceCmdString))
			{
				std::cout << "Usage: roll <player> <side> <diceCmd>\n";
				continue;
			}
			uint8_t diceCmd = static_cast<uint8_t>(std::stoull(diceCmdString, nullptr, 0));
			uint32_t result = FUN::W_RollDice(playerIdx, sideIdx, diceCmd);
			std::cout << "Rolled: " << result << "\n";
			continue;
		}
		if (command == "add")
		{
			std::string playerString, cardIDString;
			if (!(ss >> playerString >> cardIDString))
			{
				std::cout << "Usage: add <player> <cardID>\n";
				continue;
			}
			uint8_t player = static_cast<uint8_t>(std::stoull(playerString, nullptr, 0));
            uint16_t cardID = static_cast<uint16_t>(std::stoull(cardIDString, nullptr, 0));
			AddCommand(player,cardID);
			continue;
		}
        if (command == "mill")
        {
			uint32_t playerIdx, amount;
			if (!(ss >> playerIdx >> amount))
			{
				std::cout << "Usage: mill <player> <amount>\n";
				continue;
			}
			MillCommand(playerIdx, amount);
			continue;
		}
		if (command == "draw")
		{
			uint32_t playerIdx, amount;
			if (!(ss >> playerIdx >> amount))
			{
				std::cout << "Usage: draw <player> <amount>\n";
				continue;
			}
			DrawCommand(playerIdx, amount);
			continue;
		}
        if (command != "call")
        {
            std::cout << "Unknown command. Type 'help' for usage.\n";
            continue;
        }

        std::string addressString;

        if (!(ss >> addressString))
        {
            std::cout << "Usage: call <address> [arg0] [arg1] ... [arg5]\n";
            continue;
        }
        uintptr_t address = std::stoull(addressString, nullptr, 0);
        std::vector<uint32_t> args;
        std::string argString;

        while (ss >> argString)
        {

            uint32_t value = static_cast<uint32_t>(std::stoull(argString, nullptr, 0));

            args.push_back(value);
        }

        uint32_t result = CallFunction(address, args);
        std::cout << "Return value: 0x"
            << std::hex << result
            << std::dec
            << " (" << result << ")\n";
    }
}
void MillCommand(uint32_t playerIdx, uint32_t amount)
{
	FUN::MillCards(playerIdx, amount, 1);
}
void DrawCommand(uint32_t playerIdx, uint32_t amount)
{
	FUN::DrawCards(playerIdx, amount);
}
void AddCommand(uint8_t player, uint16_t cardID)
{
	uint32_t topcard = Utils::ReadUint32((void*)(GameData::BASE_PLAYER_ADDRESS + GameData::PLAYER_OFFSET * player + 0x810));
	uint16_t cardIntID = FUN::GetCardIntID(cardID);
	topcard = (topcard & 0xFFFFF000) | (cardIntID & 0xFFF);
	Utils::WriteUint32((void*)(GameData::BASE_PLAYER_ADDRESS + GameData::PLAYER_OFFSET * player + 0x810), topcard);

	DrawCommand(player, 1);
}
void SummonCommand(uint8_t player, uint16_t cardID)
{
    uint32_t topcard = duel->players[player].deck[0].fullValue;
    uint16_t cardIntID = FUN::GetCardIntID(cardID);
    topcard = (topcard & 0xFFFFF000) | (cardIntID & 0xFFF);
	duel->players[player].deck[0].fullValue = topcard;

    FUN::SpecialSummon(player, &(duel->players[player].deck[0].fullValue), 1, 0x20, 0x0d, 0);
}