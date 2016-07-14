#include "hookdata.h"

//EXPORTED FUNCTIONS:
void(*G_Knockdown)(gentity_t* victim, int duration) = (void(*)(gentity_t*, int))KNOCKDOWN; //set ent->client->ps.velocity 1st
void(*G_Damage2)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, float *dir, float *point, int damage, int dflags, int mod) = (void(*)(gentity_t*, gentity_t*, gentity_t*, float*, float*, int, int, int))DAMAGE;
char* (*Accounts_Custom_GetValue)(Account_t* acc, const char *key) = (char*(*)(Account_t*, const char*))GETVALUE; //KEY/VALUE acc data read, data not existing = return NULL
void(*Accounts_Custom_SetValue)(Account_t* acc, const char *key, const char *val) = (void(*)(Account_t*, const char*, const char*)) SETVALUE;//KEY/VALUE acc data write
int(*ClientNumberFromString)(gentity_t* to, const char* s) = (int(*)(gentity_t*, const char*))CLIENTNUMFROMSTRING; //as name says
gentity_t* (*GetEnt)(int index) = (gentity_t*(*)(int))GETENT; //return entity struct for entity with proper index
void(*DispContiguous)(gentity_t* ent, const char* msg) = (void(*)(gentity_t*, const char*))DISPCONTIGUOUS;

int(*Accounts_Stats_GetKills)(Account_t *acc) = (int(*)(Account_t*))STATS_GETKILLS;
int(*Accounts_Stats_GetDeaths)(Account_t *acc) = (int(*)(Account_t*))STATS_GETDEATHS;
int(*Accounts_Stats_GetDuels)(Account_t *acc) = (int(*)(Account_t*))STATS_GETDUELS;
int(*Accounts_Stats_GetDuelsWon)(Account_t *acc) = (int(*)(Account_t*))STATS_GETDUELSWON;
int(*Accounts_Stats_GetStashes)(Account_t *acc) = (int(*)(Account_t*))STATS_GETSTASHES;
int(*Accounts_Stats_GetShots)(Account_t *acc) = (int(*)(Account_t*))STATS_GETSHOTS;
