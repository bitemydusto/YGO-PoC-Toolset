#include "HookManager.h"
#include "Cards.h"
#include "FUN.h"
using namespace std;

void HookManager::LoadTags()
{
	// Destroy
	vector<uint16_t> cards_destroy;

	cards_destroy.push_back(Cards::TIME_WIZARD); // 0xf
	cards_destroy.push_back(Cards::REAPER_OF_THE_CARDS); // 0x53
	cards_destroy.push_back(Cards::MAN_EATER); // 0x9d
	cards_destroy.push_back(Cards::ANCIENT_JAR); // 0xa6
	cards_destroy.push_back(Cards::DARK_HOLE); // 0x14f
	cards_destroy.push_back(Cards::RAIGEKI); // 0x150
	cards_destroy.push_back(Cards::ZONE_EATER); // 0x188
	cards_destroy.push_back(Cards::STEEL_SCORPION); // 0x189
	cards_destroy.push_back(Cards::BLAST_JUGGLER); // 0x1a0
	cards_destroy.push_back(Cards::ARMED_NINJA); // 0x1d4
	cards_destroy.push_back(Cards::AIR_EATER); // 0x1d7
	cards_destroy.push_back(Cards::TRAP_MASTER); // 0x1df
	cards_destroy.push_back(Cards::DRAGON_SEEKER); // 0x1f3
	cards_destroy.push_back(Cards::MAN_EATER_BUG); // 0x1f4
	cards_destroy.push_back(Cards::ACID_TRAP_HOLE); // 0x2ac
	cards_destroy.push_back(Cards::WIDESPREAD_RUIN); // 0x2ad
	cards_destroy.push_back(Cards::JIGEN_BAKUDAN); // 0x2db
	cards_destroy.push_back(Cards::BLAST_SPHERE); // 0x2df
	cards_destroy.push_back(Cards::BARREL_DRAGON); // 0x2e6
	cards_destroy.push_back(Cards::RING_OF_DESTRUCTION); // 0x3ab
	cards_destroy.push_back(Cards::CHAIN_DESTRUCTION); // 0x3c9
	cards_destroy.push_back(Cards::FISSURE); // 0x3e9
	cards_destroy.push_back(Cards::TRAP_HOLE); // 0x3ea
	cards_destroy.push_back(Cards::REMOVE_TRAP); // 0x3ec
	cards_destroy.push_back(Cards::DE_SPELL); // 0x3f1
	cards_destroy.push_back(Cards::ANTI_RAIGEKI); // 0x3fe
	cards_destroy.push_back(Cards::TRIBUTE_TO_THE_DOOMED); // 0x3ff
	cards_destroy.push_back(Cards::SOLEMN_JUDGMENT); // 0x404
	cards_destroy.push_back(Cards::SEVEN_TOOLS_OF_THE_BANDIT); // 0x406
	cards_destroy.push_back(Cards::HORN_OF_HEAVEN); // 0x407
	cards_destroy.push_back(Cards::EXILE_OF_THE_WICKED); // 0x40d
	cards_destroy.push_back(Cards::MIRROR_FORCE); // 0x420
	cards_destroy.push_back(Cards::HEAVY_STORM); // 0x425
	cards_destroy.push_back(Cards::GRYPHON_WING); // 0x426
	cards_destroy.push_back(Cards::FINAL_DESTINY); // 0x42b
	cards_destroy.push_back(Cards::SNATCH_STEAL); // 0x42c
	cards_destroy.push_back(Cards::MYSTICAL_SPACE_TYPHOON); // 0x437
	cards_destroy.push_back(Cards::CALL_OF_THE_HAUNTED); // 0x447
	cards_destroy.push_back(Cards::DUST_TORNADO); // 0x446
	cards_destroy.push_back(Cards::CYBER_JAR); // 0x452
	cards_destroy.push_back(Cards::KOTODAMA); // 0x464
	cards_destroy.push_back(Cards::NOBLEMAN_OF_CROSSOUT); // 0x485
	cards_destroy.push_back(Cards::PREMATURE_BURIAL); // 0x488
	cards_destroy.push_back(Cards::NOBLEMAN_OF_EXTERMINATION); // 0x486
	cards_destroy.push_back(Cards::THOUSAND_KNIVES); // 0x4bb
	cards_destroy.push_back(Cards::MYSTIC_BOX); // 0x4bd
	cards_destroy.push_back(Cards::MAKIU); // 0x4e0
	cards_destroy.push_back(Cards::MICHIZURE); // 0x515
	cards_destroy.push_back(Cards::INFINITE_DISMISSAL); // 0x52a
	cards_destroy.push_back(Cards::BOMBARDMENT_BEETLE); // 0x539
	cards_destroy.push_back(Cards::_4_STARRED_LADYBUG_OF_DOOM); // 0x53a
	cards_destroy.push_back(Cards::TORRENTIAL_TRIBUTE); // 0x591
	cards_destroy.push_back(Cards::OFFERINGS_TO_THE_DOOMED); // 0x5ab
	cards_destroy.push_back(Cards::JOWGEN_THE_SPIRITUALIST); // 0x5e6
	cards_destroy.push_back(Cards::THE_LAST_WARRIOR_FROM_ANOTHER_PLANET); // 0x5f6
	cards_destroy.push_back(Cards::SKULL_LAIR); // 0x5fc
	cards_destroy.push_back(Cards::DESTRUCTION_PUNCH); // 0x5ff
	cards_destroy.push_back(Cards::BLIND_DESTRUCTION); // 0x600
	cards_destroy.push_back(Cards::EKIBYO_DRAKMORD); // 0x60c}
	HookManager::cardTags[CardTag::DESTROY] = cards_destroy;

	// Search
	vector<uint16_t> cards_search;

	cards_search.push_back(Cards::SANGAN);
	cards_search.push_back(Cards::WITCH_OF_THE_BLACK_FOREST);
	cards_search.push_back(Cards::SENJU_OF_THE_THOUSAND_HANDS);
	cards_search.push_back(Cards::SONIC_BIRD);
	cards_search.push_back(Cards::THUNDER_DRAGON);
	cards_search.push_back(Cards::FUSION_SAGE);
	cards_search.push_back(Cards::PAINFUL_CHOICE);
	cards_search.push_back(Cards::DARK_SAGE);
	HookManager::cardTags[CardTag::SEARCH] = cards_search;

	// Draw
	vector<uint16_t> cards_draw;

	cards_draw.push_back(Cards::SKELENGEL); // 0x21b
	cards_draw.push_back(Cards::MORPHING_JAR); // 0x24e
	cards_draw.push_back(Cards::GRACEFUL_CHARITY); // 0x3c8
	cards_draw.push_back(Cards::POT_OF_GREED); // 0x3f2
	cards_draw.push_back(Cards::UPSTART_GOBLIN); // 0x429
	cards_draw.push_back(Cards::CARD_DESTRUCTION); // 0x4c5
	cards_draw.push_back(Cards::JAR_OF_GREED); // 0x5a7
	HookManager::cardTags[CardTag::DRAW] = cards_draw;

	// Summon from deck
	vector<uint16_t> cards_summon_from_deck;

	cards_summon_from_deck.push_back(Cards::MAGICAL_HATS);          // 0x3B1
	cards_summon_from_deck.push_back(Cards::LAST_WILL);             // 0x41E
	cards_summon_from_deck.push_back(Cards::CYBER_JAR);             // 0x452
	cards_summon_from_deck.push_back(Cards::GIANT_RAT);             // 0x454
	cards_summon_from_deck.push_back(Cards::UFO_TURTLE);            // 0x456
	cards_summon_from_deck.push_back(Cards::GIANT_GERM);            // 0x45A
	cards_summon_from_deck.push_back(Cards::NIMBLE_MOMONGA);        // 0x45B
	cards_summon_from_deck.push_back(Cards::SHINING_ANGEL);         // 0x45D
	cards_summon_from_deck.push_back(Cards::MOTHER_GRIZZLY);        // 0x45F
	cards_summon_from_deck.push_back(Cards::FLYING_KAMAKIRI_No1);     // 0x460
	cards_summon_from_deck.push_back(Cards::MYSTIC_TOMATO);         // 0x463
	cards_summon_from_deck.push_back(Cards::DARK_MAGIC_CURTAIN);    // 0x4BC
	cards_summon_from_deck.push_back(Cards::INSECT_IMITATION);      // 0x526
	cards_summon_from_deck.push_back(Cards::BUBONIC_VERMIN);        // 0x51B
	cards_summon_from_deck.push_back(Cards::DRILL_BUG);             // 0x530
	HookManager::cardTags[CardTag::SUMMON_DECK] = cards_summon_from_deck;

	// Send from deck
	vector<uint16_t> cards_send_deck_to_grave;

	cards_send_deck_to_grave.push_back(Cards::NEEDLE_WORM);          // 0x231
	cards_send_deck_to_grave.push_back(Cards::PAINFUL_CHOICE);      // 0x439
	cards_send_deck_to_grave.push_back(Cards::GRAVEKEEPERS_SERVANT); // 0x427
	cards_send_deck_to_grave.push_back(Cards::CYBER_JAR);            // 0x452
	cards_send_deck_to_grave.push_back(Cards::MORPHING_JAR_No2);       // 0x48B
	HookManager::cardTags[CardTag::SEND_FROM_DECK] = cards_send_deck_to_grave;
}