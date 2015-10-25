#pragma once
#include <Windows.h>
#include "jassapi.h"
#include "dydutils.h"
#define MAX_ACHIEVEMENTS 19

enum dyd_achievement_types
{
	ACHIEVEMENT_NONE,
	ACHIEVEMENT_FIGHT,
	ACHIEVEMENT_DUELS,
	ACHIEVEMENT_MISC,
	ACHIEVEMENT_HELPER
};

struct dyd_achievement
{
	enum dyd_achievement_types type; //sorting achievements by types
	int id_numeric; //main achievement ID
	char* identifier; //unique name for preserving in lugormod account data
	char* name; //name to display
	char description[256]; //description to display in console
	int reward_credits;
	qboolean autoclaimable;
};


//headers of achievement management functions
void achievements_init();
void achievements_check(gentity_t*, dyd_achievement*, qboolean);
void achievements_list(gentity_t*, enum dyd_achievement_types, qboolean);
int achievements_progress(gentity_t*, const char*, qboolean);

//utility functions
dyd_achievement* FindAchievementById(int);
dyd_achievement* FindAchievementByTextIdentifier(const char*);