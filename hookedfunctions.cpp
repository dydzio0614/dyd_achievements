#include "hookedfunctions.h"

/*jmp_far jump;*/

extern struct dyd_playerdata dyd_data;

void player_die_new(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, int meansOfDeath) //this function is not a hook, it is called on unlinkentity syscall
{
	//PISTOL DUELS
	if (attacker && self != attacker && self->s.number < MAX_CLIENTS && attacker->s.number < MAX_CLIENTS &&
		( dyd_data.weapon_on_death[self->s.number] == WP_BRYAR_PISTOL && (meansOfDeath == MOD_BRYAR_PISTOL || meansOfDeath == MOD_BRYAR_PISTOL_ALT)) && attacker->client->ps.weapon == WP_BRYAR_PISTOL)
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
			achievements_check(attacker, FindAchievementById(A_DUELS_PISTOL1), qfalse);
			achievements_check(attacker, FindAchievementById(A_DUELS_PISTOL2), qfalse);
			achievements_check(attacker, FindAchievementById(A_DUELS_PISTOL3), qfalse);
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
		((dyd_data.weapon_on_death[self->s.number] == WP_BRYAR_PISTOL && (meansOfDeath == MOD_BRYAR_PISTOL || meansOfDeath == MOD_BRYAR_PISTOL_ALT)) || //restrict method of death to nonsplash guns and check if player weapon is same as bullet
			(dyd_data.weapon_on_death[self->s.number] == WP_BLASTER && meansOfDeath == MOD_BLASTER) || (dyd_data.weapon_on_death[self->s.number] == WP_REPEATER && meansOfDeath == MOD_REPEATER) || (dyd_data.weapon_on_death[self->s.number] == WP_BOWCASTER && meansOfDeath == MOD_BOWCASTER)))
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
				achievements_check(self, FindAchievementById(A_FIGHT_SELFSHOT1), qfalse);
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
			achievements_check(attacker, FindAchievementById(A_FIGHT_PKILL1), qfalse);
			achievements_check(attacker, FindAchievementById(A_FIGHT_PKILL2), qfalse);
			achievements_check(attacker, FindAchievementById(A_FIGHT_PKILL3), qfalse);
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
}