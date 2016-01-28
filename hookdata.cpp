#include "hookdata.h"

//EXPORTED FUNCTIONS:
void(*G_Knockdown)(gentity_t* victim, int duration) = (void(*)(gentity_t*, int))0x200cea90; //set ent->client->ps.velocity 1st
void(*G_Damage2)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, float *dir, float *point, int damage, int dflags, int mod) = (void(*)(gentity_t*, gentity_t*, gentity_t*, float*, float*, int, int, int))0x200c5ac0;
char* (*Accounts_Custom_GetValue)(Account_t* acc, const char *key) = (char*(*)(Account_t*, const char*))0x20171a20; //KEY/VALUE acc data read, data not existing = return NULL
void(*Accounts_Custom_SetValue)(Account_t* acc, const char *key, const char *val) = (void(*)(Account_t*, const char*, const char*)) 0x20171a90;//KEY/VALUE acc data write
int(*ClientNumberFromString)(gentity_t* to, const char* s) = (int(*)(gentity_t*, const char*))0x200b5ab0; //as name says
gentity_t* (*GetEnt)(int index) = (gentity_t*(*)(int))0x201938b0; //return entity struct for entity with proper index

int(*Accounts_Stats_GetKills)(Account_t *acc) = (int(*)(Account_t*))0x20175630;
int(*Accounts_Stats_GetDeaths)(Account_t *acc) = (int(*)(Account_t*))0x201754f0;
int(*Accounts_Stats_GetDuels)(Account_t *acc) = (int(*)(Account_t*))0x20175540;
int(*Accounts_Stats_GetDuelsWon)(Account_t *acc) = (int(*)(Account_t*))0x20175590;
int(*Accounts_Stats_GetStashes)(Account_t *acc) = (int(*)(Account_t*))0x201756d0;
int(*Accounts_Stats_GetShots)(Account_t *acc) = (int(*)(Account_t*))0x20175680;