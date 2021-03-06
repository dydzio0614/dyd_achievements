#pragma once
#include <Windows.h>
#include "jassapi.h"
#include "dydutils.h"
#define MAX_ACHIEVEMENTS 24

enum dyd_achievement_types
{
	ACHIEVEMENT_NONE,
	ACHIEVEMENT_FIGHT,
	ACHIEVEMENT_DUELS,
	ACHIEVEMENT_MISC,
	ACHIEVEMENT_HELPER,
	ACHIEVEMENT_LEGENDARY
};

enum dyd_achievement_identifiers //changing enum values will change bitmask order - not desired if trying to maintain compatibility between achievements versions
{
	A_MISC_PLAYTIME1 = 1,
	A_MISC_PLAYTIME2,
	A_FIGHT_PKILL1,
	A_FIGHT_PKILL2,
	A_FIGHT_PKILL3,
	A_DUELS_ENGAGE1,
	A_DUELS_ENGAGE2,
	A_DUELS_ENGAGE3,
	A_DUELS_DRATIO1,
	A_DUELS_DRATIO2,
	A_DUELS_DRATIO3,
	A_NONE_RAGEDYDZIO = 1337,
	A_HELPER_DONATOR = 13,
	A_FIGHT_SELFSHOT1,
	A_FIGHT_AMMO1,
	A_FIGHT_AMMO2,
	A_FIGHT_AMMO3,
	A_MISC_STASH1,
	A_MISC_STASH2,
	A_DUELS_PISTOL1,
	A_DUELS_PISTOL2,
	A_DUELS_PISTOL3,
	A_MISC_PLAYTIME3,
	A_DUELS_ENGAGE4
};

struct dyd_achievement
{
	enum dyd_achievement_types type; //sorting achievements by types
	enum dyd_achievement_identifiers id; //main achievement ID
	char* name; //name to display
	char description[256]; //description to display in console
	int reward_credits;
	qboolean autoclaimable;

	struct //holds variable data for achievements that depend of reaching certain amount of one data, should be NULL for achievements not using this feature
	{
		char* varName; //variable name to display in print
		int counter; //value required to unlock achievement - int should suffice for now
		int(*GetStatFunction)(Account_t*); //function pointer to check current stat progress
	}progressVariable;
};

struct dyd_playerdata
{
	int saberbarrier_cooldown[MAX_CLIENTS];
	int weapon_on_death[MAX_CLIENTS];
	int mentalshield_cooldown[MAX_CLIENTS];
	int bladetornado_cooldown[MAX_CLIENTS];
};


//headers of achievement management functions
void achievements_init();
void achievements_check(gentity_t*, dyd_achievement*, qboolean);
void achievements_list(gentity_t*, enum dyd_achievement_types, qboolean, qboolean);
int achievements_progress(gentity_t*, enum dyd_achievement_identifiers, qboolean, qboolean);
int achievements_progress(gentity_t*, enum dyd_achievement_identifiers, qboolean);

//utility functions
dyd_achievement* FindAchievementById(int);
long GetAchievementBitmaskFromID(long);