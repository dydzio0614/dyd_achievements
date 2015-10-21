#ifndef __DYD_ACHIEVEMENTS__
#define __DYD_ACHIEVEMENTS__

#define MAX_ACHIEVEMENTS 11

enum dyd_achievement_types
{
	ACHIEVEMENT_FIGHT,
	ACHIEVEMENT_DUELS,
	ACHIEVEMENT_MISC
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

struct dyd_achievement achievements[MAX_ACHIEVEMENTS];


//headers of functions defined in main:
void achievements_init();
void achievements_check(gentity_t*, dyd_achievement*);
void achievements_list(gentity_t*, enum dyd_achievement_types, qboolean);
int achievements_progress(gentity_t*, const char*, qboolean);

//utility functions
dyd_achievement* FindAchievementById(int value)
{
	for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
	{
		if (achievements[i].id_numeric == value)
			return &achievements[i];
	}
	return NULL;
}

dyd_achievement* FindAchievementByTextIdentifier(const char *text)
{
	for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
	{
		if (!stricmp(text, achievements[i].identifier))
			return &achievements[i];
	}
	return NULL;
}


#endif //__DYD_ACHIEVEMENTS__

