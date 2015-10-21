#ifndef __DYD_ACHIEVEMENTS__
#define __DYD_ACHIEVEMENTS__

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
	char* description; //description to display in console
	int reward_credits;
	qboolean autoclaimable;
};

struct dyd_achievement *achievements[32]; //TODO: alloc achievements on stack

//exported functions:
void(*G_Knockdown)(gentity_t* victim, int duration) = (void(*)(gentity_t*, int))0x200ce8b0; //set ent->client->ps.velocity 1st
void(*G_Damage2)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, float *dir, float *point, int damage, int dflags, int mod) = (void(*)(gentity_t*, gentity_t*, gentity_t*, float*, float*, int, int, int))0x200C58E0;
char* (*Accounts_Custom_GetValue)(Account_t* acc, const char *key) = (char*(*)(Account_t*, const char*))0x201712e0; //KEY/VALUE acc data read, data not existing = return NULL
void(*Accounts_Custom_SetValue)(Account_t* acc, const char *key, const char *val) = (void(*)(Account_t*, const char*, const char*)) 0x20171350;//KEY/VALUE acc data write
int(*ClientNumberFromString)(gentity_t* to, const char* s) = (int(*)(gentity_t*, const char*))0x200b5a10; //as name says
gentity_t* (*GetEnt)(int index) = (gentity_t*(*)(int))0x20193150; //return entity struct for entity with proper index

int(*Accounts_Stats_GetKills)(Account_t *acc) = (int(*)(Account_t*))0x20174F30;
int(*Accounts_Stats_GetDeaths)(Account_t *acc) = (int(*)(Account_t*))0x20174DF0;
int(*Accounts_Stats_GetDuels)(Account_t *acc) = (int(*)(Account_t*))0x20174E40;
int(*Accounts_Stats_GetDuelsWon)(Account_t *acc) = (int(*)(Account_t*))0x20174E90;
int(*Accounts_Stats_GetStashes)(Account_t *acc) = (int(*)(Account_t*))0x20174FD0;


//headers of functions defined in main:
void achievements_init();
void achievements_check(gentity_t*, dyd_achievement*);
void achievements_list(gentity_t*, enum dyd_achievement_types, qboolean);
int achievements_progress(gentity_t*, const char*, qboolean);

//utility functions
dyd_achievement* FindAchievementById(int value)
{
	for (int i = 0; achievements[i] != NULL; i++)
	{
		if (achievements[i]->id_numeric == value)
			return achievements[i];
	}
	return NULL;
}

dyd_achievement* FindAchievementByTextIdentifier(const char *text)
{
	for (int i = 0; achievements[i] != NULL; i++)
	{
		if (!stricmp(text, achievements[i]->identifier))
			return achievements[i];
	}
	return NULL;
}

int Accounts_Stats_GetPlayerKills(Account_t *acc)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_PLAYERKILLS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

int Accounts_Stats_GetPlayerDefeats(Account_t *acc)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_PLAYERDEATHS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

void Accounts_Stats_SetPlayerKills(Account_t *acc, int value)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%d", value);
		Accounts_Custom_SetValue(acc, "A_DATA_PLAYERKILLS", data);
	}
}

void Accounts_Stats_SetPlayerDefeats(Account_t *acc, int value)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%d", value);
		Accounts_Custom_SetValue(acc, "A_DATA_PLAYERDEATHS", data);
	}
}
#endif //__DYD_ACHIEVEMENTS__

