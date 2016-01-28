#include "hookedfunctions.h"

BYTE oldPlayerDie[6];
jmp_far jump;

void player_die(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, int meansOfDeath)
{
	//PISTOL DUELS
	if (attacker && self != attacker && self->s.number < MAX_CLIENTS && attacker->s.number < MAX_CLIENTS &&
		(self->client->ps.weapon == WP_BRYAR_PISTOL && (meansOfDeath == MOD_BRYAR_PISTOL || meansOfDeath == MOD_BRYAR_PISTOL_ALT)) && attacker->client->ps.weapon == WP_BRYAR_PISTOL)
	{
		if (attacker->client->pers.Lmd.account)
		{
			int pistol_duels = Accounts_Stats_GetPistolDuels(attacker->client->pers.Lmd.account);

			if (pistol_duels)
			{
				pistol_duels++;
				Accounts_Stats_SetPistolDuels(attacker->client->pers.Lmd.account, pistol_duels);
			}
			else
				Accounts_Stats_SetPistolDuels(attacker->client->pers.Lmd.account, 1);


			int pistol_wins = Accounts_Stats_GetPistolDuelWins(attacker->client->pers.Lmd.account);

			if (pistol_wins)
			{
				pistol_wins++;
				Accounts_Stats_SetPistolDuelWins(attacker->client->pers.Lmd.account, pistol_wins);
			}
			else
				Accounts_Stats_SetPistolDuelWins(attacker->client->pers.Lmd.account, 1);

			//achievement checks
			achievements_check(attacker, FindAchievementByTextIdentifier("A_DUELS_PISTOL1"), qfalse);
			achievements_check(attacker, FindAchievementByTextIdentifier("A_DUELS_PISTOL2"), qfalse);
			achievements_check(attacker, FindAchievementByTextIdentifier("A_DUELS_PISTOL3"), qfalse);
		}

		if (self->client->pers.Lmd.account)
		{
			int pistol_duels = Accounts_Stats_GetPistolDuels(self->client->pers.Lmd.account);

			if (pistol_duels)
			{
				pistol_duels++;
				Accounts_Stats_SetPistolDuels(self->client->pers.Lmd.account, pistol_duels);
			}
			else
				Accounts_Stats_SetPistolDuels(self->client->pers.Lmd.account, 1);
		}
	}


	//SELF SHOT
	if (attacker && self != attacker && self->s.number < MAX_CLIENTS  && self->client->ps.trueJedi == qfalse && //if not suicide and not force user prof
		((self->client->ps.weapon == WP_BRYAR_PISTOL && (meansOfDeath == MOD_BRYAR_PISTOL || meansOfDeath == MOD_BRYAR_PISTOL_ALT)) || //restrict method of death to nonsplash guns and check if player weapon is same as bullet
			(self->client->ps.weapon == WP_BLASTER && meansOfDeath == MOD_BLASTER) || (self->client->ps.weapon == WP_REPEATER && meansOfDeath == MOD_REPEATER) || (self->client->ps.weapon == WP_BOWCASTER && meansOfDeath == MOD_BOWCASTER)))
	{
		if ((attacker->s.number < MAX_CLIENTS && attacker->client->ps.trueJedi == qtrue) || (attacker->NPC && attacker->client->ps.weapon == WP_SABER)) //check if attacker is either force user or NPC with saber
		{
			if (self->client->pers.Lmd.account)
			{
				int selfshots = Accounts_Stats_GetSelfshots(self->client->pers.Lmd.account);
				if (selfshots)
				{
					selfshots++;
					Accounts_Stats_SetSelfshots(self->client->pers.Lmd.account, selfshots);
				}
				else
					Accounts_Stats_SetSelfshots(self->client->pers.Lmd.account, 1);

				//achievement checks
				achievements_check(self, FindAchievementByTextIdentifier("A_FIGHT_SELFSHOT1"), qfalse);
			}
		}
	}

	//PLAYER KILL COUNTING
	if (attacker && self != attacker && self->s.number < MAX_CLIENTS && attacker->s.number < MAX_CLIENTS)
	{
		if (attacker->client->pers.Lmd.account)
		{   //update playerkills
			int killnumber = Accounts_Stats_GetPlayerKills(attacker->client->pers.Lmd.account);

			if (killnumber)
			{
				killnumber++;
				Accounts_Stats_SetPlayerKills(attacker->client->pers.Lmd.account, killnumber);
			}
			else
				Accounts_Stats_SetPlayerKills(attacker->client->pers.Lmd.account, 1);

			//achievement checks
			achievements_check(attacker, FindAchievementByTextIdentifier("A_FIGHT_PKILL1"), qfalse);
			achievements_check(attacker, FindAchievementByTextIdentifier("A_FIGHT_PKILL2"), qfalse);
			achievements_check(attacker, FindAchievementByTextIdentifier("A_FIGHT_PKILL3"), qfalse);
		}

		if (self->client->pers.Lmd.account)
		{	//update playerdeaths
			int deathnumber = Accounts_Stats_GetPlayerDefeats(attacker->client->pers.Lmd.account);

			if (deathnumber)
			{
				deathnumber++;
				Accounts_Stats_SetPlayerDefeats(attacker->client->pers.Lmd.account, deathnumber);
			}
			else
				Accounts_Stats_SetPlayerDefeats(attacker->client->pers.Lmd.account, 1);
		}
	}

	WriteProcessMemory(GetCurrentProcess(), (void*)PLAYER_DIE, (void*)&oldPlayerDie, 6, NULL);
	((void(__cdecl*)(gentity_t*, gentity_t*, gentity_t*, int, int))PLAYER_DIE)(self, inflictor, attacker, damage, meansOfDeath);
	jump.arg = (DWORD)(&player_die);
	WriteProcessMemory(GetCurrentProcess(), (void*)PLAYER_DIE, (void*)&jump, 6, NULL);
}