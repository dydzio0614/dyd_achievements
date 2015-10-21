#pragma once
#include <Windows.h>
#include "jassapi.h" 
#include "jka_sdk/game/g_local.h"

//EXPORTED ADDRESSES:

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

level_locals_t* g_level = (level_locals_t*)0x20ae90b8;

//GENERAL HOOKING STUFF

__declspec(naked) void execute_address(unsigned int arg) //push arg, naked function so does not disrupt ebp register flow
{
	__asm
	{
		pop eax //destroy new return address
		ret
	}
}

//HOOKED FUNCTION DATA

//PLAYER_DIE

#define PLAYER_DIE 0x200d15f0

__declspec(naked) void player_die_patchdata()
{
	__asm
	{
		push player_die
		ret
	}
}

__declspec(naked) void player_die_entry()
{
	__asm //original func data
	{
		push ebp
		mov ebp, esp
		sub esp, 192
	}

	execute_address(0x200d15f9); //original player_die + offset
}

