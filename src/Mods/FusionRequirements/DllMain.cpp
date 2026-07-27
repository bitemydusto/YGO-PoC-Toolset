#include <Windows.h>

#include "Utils.h"
#include "Cards.h"

const uint32_t BASE_OFFSET = 0x005ecf10; // Change this if it's different in your version of the game
const uint32_t TRIPLE_OFFSET = BASE_OFFSET + 0X138;

void LoadFusionRequirements();
struct Fusion2
{
    uint16_t Result;
    uint16_t Materials[2];
};
struct Fusion3
{
    uint16_t Result;
    uint16_t Materials[3];
};

DWORD WINAPI MainThread(LPVOID lpParam)
{
    LoadFusionRequirements();

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


constexpr Fusion2 Recipes2[] =
{
	Cards::FLAME_SWORDSMAN, // Index: [0]
	{
		Cards::FLAME_MANIPULATOR,
		Cards::MASAKI_THE_LEGENDARY_SWORDSMAN
	},
	Cards::ZOMBIE_WARRIOR, // Index: [1]
	{
		Cards::SKULL_SERVANT,
		Cards::BATTLE_WARRIOR
	},
	Cards::GAIA_THE_DRAGON_CHAMPION, // Index: [2]
	{
		Cards::GAIA_THE_FIERCE_KNIGHT,
		Cards::CURSE_OF_DRAGON
	},
	Cards::KARBONALA_WARRIOR, // Index: [3]
	{
		Cards::M_WARRIOR_No1,
		Cards::M_WARRIOR_No2
	},
	Cards::THOUSAND_DRAGON, // Index: [4]
	{
		Cards::BABY_DRAGON,
		Cards::TIME_WIZARD
	},
	Cards::BAROX, // Index: [5]
	{
		Cards::RYU_KISHIN,
		Cards::FRENZIED_PANDA
	},
	Cards::RABID_HORSEMAN, // Index: [6]
	{
		Cards::BATTLE_OX,
		Cards::MYSTIC_HORSEMAN
	},
	Cards::KAMIONWIZARD, // Index: [7]
	{
		Cards::MYSTICAL_ELF,
		Cards::CURTAIN_OF_THE_DARK_ONES
	},
	Cards::CHARUBIN_THE_FIRE_KNIGHT, // Index: [8]
	{
		Cards::MONSTER_EGG,
		Cards::HINOTAMA_SOUL
	},
	Cards::DARKFIRE_DRAGON, // Index: [9]
	{
		Cards::FIREGRASS,
		Cards::PETIT_DRAGON
	},
	Cards::FUSIONIST, // Index: [10]
	{
		Cards::PETIT_ANGEL,
		Cards::MYSTICAL_SHEEP_No2
	},
	Cards::FLAME_GHOST, // Index: [11]
	{
		Cards::SKULL_SERVANT,
		Cards::DISSOLVEROCK
	},
	Cards::B_SKULL_DRAGON, // Index: [12]
	{
		Cards::SUMMONED_SKULL,
		Cards::RED_EYES_B_DRAGON
	},
	Cards::ROARING_OCEAN_SNAKE, // Index: [13]
	{
		Cards::MYSTIC_LAMP,
		Cards::HYOSUBE
	},
	Cards::RARE_FISH, // Index: [14]
	{
		Cards::FUSIONIST,
		Cards::ENCHANTING_MERMAID
	},
	Cards::MAVELUS, // Index: [15]
	{
		Cards::TYHONE,
		Cards::WINGS_OF_WICKED_FLAME
	},
	Cards::DRAGONESS_THE_WICKED_KNIGHT, // Index: [16]
	{
		Cards::ARMAILL,
		Cards::ONE_EYED_SHIELD_DRAGON
	},
	Cards::LABYRINTH_TANK, // Index: [17]
	{
		Cards::GIGA_TECH_WOLF,
		Cards::CANNON_SOLDIER
	},
	Cards::BICKURIBOX, // Index: [18]
	{
		Cards::CRASS_CLOWN,
		Cards::DREAM_CLOWN
	},
	Cards::GILTIA_THE_D_KNIGHT, // Index: [19]
	{
		Cards::GUARDIAN_OF_THE_LABYRINTH,
		Cards::PROTECTOR_OF_THE_THRONE
	},
		Cards::METAL_DRAGON, // Index: [20]
	{
		Cards::STEEL_OGRE_GROTTO_No1,
		Cards::LESSER_DRAGON
	},
	Cards::KAISER_DRAGON, // Index: [21]
	{
		Cards::WINGED_DRAGON_GUARDIAN_OF_THE_FORTRESS_No1,
		Cards::FAIRY_DRAGON
	},
	Cards::DEEPSEA_SHARK, // Index: [22]
	{
		Cards::BOTTOM_DWELLER,
		Cards::TONGYO
	},
	Cards::KAMINARI_ATTACK, // Index: [23]
	{
		Cards::OCUBEAM,
		Cards::MEGA_THUNDERBALL
	},
	Cards::PUNISHED_EAGLE, // Index: [24]
	{
		Cards::NIWATORI,
		Cards::BLUE_WINGED_CROWN
	},
	Cards::CRIMSON_SUNBIRD, // Index: [25]
	{
		Cards::FAITH_BIRD,
		Cards::SKULL_RED_BIRD
	},
	Cards::SOUL_HUNTER, // Index: [26]
	{
		Cards::LORD_OF_THE_LAMP,
		Cards::INVADER_FROM_ANOTHER_DIMENSION
	},
	Cards::VERMILLION_SPARROW, // Index: [27]
	{
		Cards::RHAIMUNDOS_OF_THE_RED_SWORD,
		Cards::FIREYAROU
	},
	Cards::PRAGTICAL, // Index: [28]
	{
		Cards::TRAKADON,
		Cards::FLAME_VIPER
	},
	Cards::FLOWER_WOLF, // Index: [29]
	{
		Cards::SILVER_FANG,
		Cards::DARKWORLD_THORNS
	},
	Cards::MUSICIAN_KING, // Index: [30]
	{
		Cards::WITCH_OF_THE_BLACK_FOREST,
		Cards::LADY_OF_FAITH
	},
	Cards::CYBER_SAURUS, // Index: [31]
	{
		Cards::TWO_HEADED_KING_REX,
		Cards::BLAST_JUGGLER
	},
	Cards::BRACCHIO_RAIDUS, // Index: [32]
	{
		Cards::TWO_HEADED_KING_REX,
		Cards::CRAWLING_DRAGON_No2
	},
	Cards::SKULLBIRD, // Index: [33]
	{
		Cards::TEMPLE_OF_SKULLS,
		Cards::TAKUHEE
	},
	Cards::MYSTICAL_SAND, // Index: [34]
	{
		Cards::GIANT_SOLDIER_OF_STONE,
		Cards::ANCIENT_ELF
	},
	Cards::KWAGAR_HERCULES, // Index: [35]
	{
		Cards::HERCULES_BEETLE,
		Cards::KUWAGATA_ALPHA
	},
	Cards::SKELGON, // Index: [36]
	{
		Cards::BLACKLAND_FIRE_DRAGON,
		Cards::THE_SNAKE_HAIR
	},
	Cards::GREAT_MAMMOTH_OF_GOLDFINE, // Index: [37]
	{
		Cards::THE_SNAKE_HAIR,
		Cards::DRAGON_ZOMBIE
	},
	Cards::EMPRESS_JUDGE, // Index: [38]
	{
		Cards::QUEENS_DOUBLE,
		Cards::HIBIKIME
	},
	Cards::ROSE_SPECTRE_OF_DUNN, // Index: [39]
	{
		Cards::FERAL_IMP,
		Cards::SNAKEYASHI
	},
	Cards::TWIN_HEADED_THUNDER_DRAGON, // Index: [40]
	{
		Cards::THUNDER_DRAGON,
		Cards::THUNDER_DRAGON
	},
	Cards::MARINE_BEAST, // Index: [41]
	{
		Cards::WATER_MAGICIAN,
		Cards::BEHEGON
	},
	Cards::WARRIOR_OF_TRADITION, // Index: [42]
	{
		Cards::BEAUTIFUL_HEADHUNTRESS,
		Cards::SONIC_MAID
	},
	Cards::AMPHIBIOUS_BUGROTH, // Index: [43]
	{
		Cards::GROUND_ATTACKER_BUGROTH,
		Cards::GUARDIAN_OF_THE_SEA
	},
	Cards::SKULL_KNIGHT, // Index: [44]
	{
		Cards::TAINTED_WISDOM,
		Cards::ANCIENT_BRAIN
	},
	Cards::METEOR_B_DRAGON, // Index: [45]
	{
		Cards::RED_EYES_B_DRAGON,
		Cards::METEOR_DRAGON
	},
	Cards::ALLIGATORS_SWORD_DRAGON, // Index: [46]
	{
		Cards::BABY_DRAGON,
		Cards::ALLIGATORS_SWORD
	},
	Cards::CHIMERA_THE_FLYING_MYTHICAL_BEAST, // Index: [47]
	{
		Cards::GAZELLE_THE_KING_OF_MYTHICAL_BEASTS,
		Cards::BERFOMET
	},
	Cards::THOUSAND_EYES_RESTRICT, // Index: [48]
	{
		Cards::RELINQUISHED,
		Cards::THOUSAND_EYES_IDOL
	},
	Cards::HUMANOID_WORM_DRAKE, // Index: [49]
	{
		Cards::WORM_DRAKE,
		Cards::HUMANOID_SLIME
	},
	Cards::ST_JOAN, // Index: [50]
	{
		Cards::THE_FORGIVING_MAIDEN,
		Cards::MARIE_THE_FALLEN_ONE
	},
	Cards::THE_LAST_WARRIOR_FROM_ANOTHER_PLANET, // Index: [51]
	{
		Cards::ZOMBYRA_THE_DARK,
		Cards::MARYOKUTAI
	}
};
constexpr Fusion3 Recipes3[] =
{
	Cards::BLUE_EYES_ULTIMATE_DRAGON, // Index: [0]
	{
		Cards::BLUE_EYES_WHITE_DRAGON,
		Cards::BLUE_EYES_WHITE_DRAGON,
		Cards::BLUE_EYES_WHITE_DRAGON
	},
	Cards::AQUA_DRAGON, // Index: [1]
	{
		Cards::ZONE_EATER,
		Cards::FAIRY_DRAGON,
		Cards::AMAZON_OF_THE_SEAS
	},
	Cards::MAN_EATING_BLACK_SHARK, // Index: [2]
	{
		Cards::SEA_KAMEN,
		Cards::AMAZON_OF_THE_SEAS,
		Cards::GRUESOME_GOO
	}
};
void LoadFusionRequirements()
{
	int address = BASE_OFFSET;
	for (const auto& recipe : Recipes2)
	{
		Utils::WriteUint16((void*)address, recipe.Result);
		address += 2;
		for (int i = 0; i < 2; i++)
		{
			Utils::WriteUint16((void*)address, recipe.Materials[i]);
			address += 2;
		}
	}
	for (const auto& recipe : Recipes3)
	{
		Utils::WriteUint16((void*)address, recipe.Result);
		address += 2;
		for (int i = 0; i < 3; i++)
		{
			Utils::WriteUint16((void*)address, recipe.Materials[i]);
			address += 2;
		}
	}
}

