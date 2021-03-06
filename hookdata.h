#pragma once
#pragma warning (disable: 4996)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "jka_sdk\game\g_local.h"
#include "lmdaddr.h"

//struct used as asm code snippet to hook functions - performs push addr;ret to change EIP register value
#pragma pack (push, 1) //1 byte struct aligning
struct jmp_far
{
	unsigned char instr_push;
	unsigned long arg;
	unsigned char instr_ret;
};
#pragma pack (pop)


//exports
extern void(*G_Knockdown)(gentity_t* victim, int duration); //set ent->client->ps.velocity 1st
extern void(*G_Damage2)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, float *dir, float *point, int damage, int dflags, int mod);
extern char* (*Accounts_Custom_GetValue)(Account_t* acc, const char *key); //KEY/VALUE acc data read, data not existing = return NULL
extern void(*Accounts_Custom_SetValue)(Account_t* acc, const char *key, const char *val);//KEY/VALUE acc data write
extern int(*ClientNumberFromString)(gentity_t* to, const char* s); //as name says
extern gentity_t* (*GetEnt)(int index); //return entity struct for entity with proper index
extern void (*DispContiguous)(gentity_t* ent, const char* msg); //add string to specific buffer - syscall SEND_SERVER_COMMAND print if msg NULL or buffer size exceeds 1024
extern int(*Auths_GetPlayerRank)(gentity_t *ent);
extern Account_t* (*Accounts_GetByUsername)(const char* str);

extern int(*Accounts_Stats_GetKills)(Account_t *acc);
extern int(*Accounts_Stats_GetDeaths)(Account_t *acc);
extern int(*Accounts_Stats_GetDuels)(Account_t *acc);
extern int(*Accounts_Stats_GetDuelsWon)(Account_t *acc);
extern int(*Accounts_Stats_GetStashes)(Account_t *acc);
extern int(*Accounts_Stats_GetShots)(Account_t *acc);

