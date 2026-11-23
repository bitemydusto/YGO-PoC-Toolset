#include "Debugger.h"

GameData::Duel* duel = GameData::GetDuel();
std::vector<Command> commands;

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
		default:
		{
			std::cout << "Error: Too many arguments for function call.\n";
			return 0;
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

    commands.push_back({"help", "", 0, (uintptr_t)HelpCommand});
	commands.push_back({"call", "call <address> [arg0] [arg1] ... [arg5]", 2, (uintptr_t)CallCommand});
    commands.push_back({"disrand", "disrand <player> <amount>", 2, (uintptr_t)RandomDiscardCommand});
    commands.push_back({"mill", "mill <player> <amount>", 2, (uintptr_t)MillCommand});
    commands.push_back({"draw", "draw <player> <amount>", 2, (uintptr_t)DrawCommand});
    commands.push_back({"add", "add <player> <cardID>", 2, (uintptr_t)AddCommand});
    commands.push_back({"roll", "roll <player> <side> <diceCmd>", 3, (uintptr_t)RollCommand});
    commands.push_back({"ss", "ss <player> <cardID>", 2, (uintptr_t)SummonCommand});
    commands.push_back({"play", "play <player> <handIdx>", 2, (uintptr_t)PlayCommand});

    std::string line;

    while (std::getline(std::cin, line))
    {
        std::stringstream ss(line);

        std::string command;
        ss >> command;

        if (command.empty()) continue;
		bool found = false;
        for (const auto& cmd : commands)
        {
            if (command == cmd.name)
            {
                found = true;

                vector<uint32_t> args;
                for (std::string argString; ss >> argString;)
                {
                    args.push_back(static_cast<uint32_t>(std::stoull(argString, nullptr, 0)));
                }
                if (args.size() < cmd.args) goto invalid_usage;
                if (cmd.name == "call")
                {
                    vector<uint32_t> argsNew, funArgs;
					argsNew.push_back(args[0]);
					for (size_t i = 1; i < args.size(); i++)
					{
						funArgs.push_back(args[i]);
					}
                    argsNew.push_back(reinterpret_cast<uint32_t>(&funArgs));
					args = argsNew;
                }
                if (args.size() > cmd.args)
                {
                invalid_usage:
                    std::cout << "Invalid use of command, usage: " << cmd.usage << "\n";
                    break;
                }

                CallFunction(cmd.fun, args);
                break;
            }
        }
        if (!found)
        {
            std::cout << "Unknown command. Type 'help' for usage.\n";
        }
    }
}
void HelpCommand()
{
    for (const auto& cmd : commands)
    {
		if (cmd.name == "help") continue;
        std::cout << cmd.usage << "\n";
    }
}
void CallCommand(uintptr_t address, uint32_t argsAddress)
{
	const std::vector<uint32_t>& args = *reinterpret_cast<const std::vector<uint32_t>*>(argsAddress);

    uint32_t result = CallFunction(address, args);
    std::cout << "Return value: 0x"
        << std::hex << result
        << std::dec
        << " (" << result << ")\n";
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

    FUN::SpecialSummon(player, &(duel->players[player].deck[0].fullValue), 1, 0x20, 0x0d, player);
}
void PlayCommand(uint8_t player, uint16_t handIdx)
{
    uint8_t hand = handIdx;
    uint8_t turn = player;

    uint32_t pack = 0x45C | 0x200000 | (hand << 16) | ((player ^ turn) << 24) | (player << 31);

	FUN::QueueEffect(pack, duel->players[player].hand[hand].GetInstance(), 0);
}
void RandomDiscardCommand(uint8_t player, uint32_t amount)
{
	FUN::DiscardRandomCard(player, 0, amount);
}
void RollCommand(uint32_t playerIdx, uint32_t sideIdx, uint8_t diceCmd)
{
    uint32_t result = FUN::W_RollDice(playerIdx, sideIdx, diceCmd);
    std::cout << "Rolled: " << result << "\n";
}