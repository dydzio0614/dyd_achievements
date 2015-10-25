#pragma once
#include "jka_sdk\game\g_local.h"

//generic functions
void execute_address(unsigned int arg);

//exports
extern void(*G_Knockdown)(gentity_t* victim, int duration); //set ent->client->ps.velocity 1st
extern void(*G_Damage2)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, float *dir, float *point, int damage, int dflags, int mod);
extern char* (*Accounts_Custom_GetValue)(Account_t* acc, const char *key); //KEY/VALUE acc data read, data not existing = return NULL
extern void(*Accounts_Custom_SetValue)(Account_t* acc, const char *key, const char *val);//KEY/VALUE acc data write
extern int(*ClientNumberFromString)(gentity_t* to, const char* s); //as name says
extern gentity_t* (*GetEnt)(int index); //return entity struct for entity with proper index

extern int(*Accounts_Stats_GetKills)(Account_t *acc);
extern int(*Accounts_Stats_GetDeaths)(Account_t *acc);
extern int(*Accounts_Stats_GetDuels)(Account_t *acc);
extern int(*Accounts_Stats_GetDuelsWon)(Account_t *acc);
extern int(*Accounts_Stats_GetStashes)(Account_t *acc);
extern int(*Accounts_Stats_GetShots)(Account_t *acc);

//player_die data
#define PLAYER_DIE 0x200d15f0
void player_die_patchdata();
void player_die_entry();
