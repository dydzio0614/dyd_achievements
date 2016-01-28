#pragma once
#include "achievements.h"

#pragma pack (push, 1)
struct jmp_far
{
	BYTE instr_push;
	DWORD arg;
	BYTE instr_ret;
};
#pragma pack (pop)

#define PLAYER_DIE 0x200d17d0
void player_die(gentity_t*, gentity_t*, gentity_t*, int, int);
