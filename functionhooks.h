#pragma once
#include "achievements.h"

void player_die(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, int meansOfDeath)
{
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
			if (achievements_progress(attacker, "A_FIGHT_PKILL1", qfalse) == 1 && Accounts_Custom_GetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL1") == NULL)
			{
				Accounts_Custom_SetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL1", "1");
				dyd_achievement *x = FindAchievementByTextIdentifier("A_FIGHT_PKILL1");
				attacker->client->pers.Lmd.account->credits += x->reward_credits; //maybe further refactor using achievements_claim?
				attacker->client->pers.Lmd.account->modifiedTime = g_level->time;
				g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("chat \"^7Player %s has completed achievement: %s\n\"", attacker->client->pers.netname, x->name));
			}

			if (achievements_progress(attacker, "A_FIGHT_PKILL2", qfalse) == 1 && Accounts_Custom_GetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL2") == NULL)
			{
				Accounts_Custom_SetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL2", "1");
				dyd_achievement *x = FindAchievementByTextIdentifier("A_FIGHT_PKILL2");
				attacker->client->pers.Lmd.account->credits += x->reward_credits;
				attacker->client->pers.Lmd.account->modifiedTime = g_level->time;
				g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("chat \"^7Player %s has completed achievement: %s\n\"", attacker->client->pers.netname, x->name));
			}

			if (achievements_progress(attacker, "A_FIGHT_PKILL3", qfalse) == 1 && Accounts_Custom_GetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL3") == NULL)
			{
				Accounts_Custom_SetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL3", "1");
				dyd_achievement *x = FindAchievementByTextIdentifier("A_FIGHT_PKILL3");
				attacker->client->pers.Lmd.account->credits += x->reward_credits;
				attacker->client->pers.Lmd.account->modifiedTime = g_level->time;
				g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("chat \"^7Player %s has completed achievement: %s\n\"", attacker->client->pers.netname, x->name));
			}
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