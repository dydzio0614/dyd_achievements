#include "hookedfunctions.h"

void player_die(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, int meansOfDeath)
{
	//(if attacker has prof jedi or is NPC with saber and self is not jedi and self has proper weapon up and methodofdeath is proper)
	if (attacker && self != attacker && self->s.number < MAX_CLIENTS && ((attacker->s.number < MAX_CLIENTS && attacker->client->ps.trueJedi == qtrue) || (attacker->s.number >= MAX_CLIENTS && attacker->client->ps.weapon == WP_SABER)) && self->client->ps.trueJedi == qfalse &&
		((self->client->ps.weapon == WP_BRYAR_PISTOL && (meansOfDeath == MOD_BRYAR_PISTOL || meansOfDeath == MOD_BRYAR_PISTOL_ALT)) ||
			(self->client->ps.weapon == WP_BLASTER && meansOfDeath == MOD_BLASTER) || (self->client->ps.weapon == WP_REPEATER && meansOfDeath == MOD_REPEATER) || (self->client->ps.weapon == WP_BOWCASTER && meansOfDeath == MOD_BOWCASTER)))
	{	//update selfshots
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

	__asm
	{
		mov esp, ebp
		pop ebp
	} //end function, but without returning to pre-call place

	execute_address((unsigned int)player_die_entry);
}