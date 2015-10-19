#include <Windows.h>
#include "jassapi.h" 
#include "jka_sdk/game/g_local.h"
#include "achievements.h"
#include "hookdata.h"

#pragma warning (disable: 4996)

//plugin info
plugininfo_t g_plugininfo = { "dydplugin", "0.1.0", "Lugormod U# 2.4.8.3 Lugormod Achievement System", "Dydzio", "", 1, 1, 1, JASS_PIFV_MAJOR, JASS_PIFV_MINOR };
pluginres_t* g_result = NULL;
eng_syscall_t g_syscall = NULL;
mod_vmMain_t g_vmMain = NULL;
pluginfuncs_t* g_pluginfuncs = NULL;
int g_vmbase = 0;

//hooking data
#define PLAYER_DIE 0x200d15f0
void execute_address(unsigned int arg);
void player_die_patchdata();
void player_die_entry();

//exported functions
void(*G_Knockdown)(gentity_t* victim, int duration) = (void(*)(gentity_t*, int))0x200ce8b0; //set ent->client->ps.velocity 1st
void (*G_Damage2)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, float *dir, float *point, int damage, int dflags, int mod) = (void(*)(gentity_t*, gentity_t*, gentity_t*, float*, float*, int, int, int))0x200C58E0;
char* (*Accounts_Custom_GetValue)(Account_t* acc, const char *key) = (char*(*)(Account_t*, const char*))0x201712e0; //KEY/VALUE acc data read, data not existing = return NULL
void(*Accounts_Custom_SetValue)(Account_t* acc, const char *key, const char *val) = (void(*)(Account_t*, const char*, const char*)) 0x20171350;//KEY/VALUE acc data write
int(*ClientNumberFromString)(gentity_t* to, const char* s) = (int(*)(gentity_t*, const char*))0x200b5a10; //as name says
gentity_t* (*GetEnt)(int index) = (gentity_t*(*)(int))0x20193150; //return entity struct for entity with proper index

int(*Accounts_Stats_GetKills)(Account_t *acc) = (int(*)(Account_t*))0x20174F30;
int(*Accounts_Stats_GetDeaths)(Account_t *acc) = (int(*)(Account_t*))0x20174DF0;
int(*Accounts_Stats_GetDuels)(Account_t *acc) = (int(*)(Account_t*))0x20174E40;
int(*Accounts_Stats_GetDuelsWon)(Account_t *acc) = (int(*)(Account_t*))0x20174E90;
int(*Accounts_Stats_GetStashes)(Account_t *acc) = (int(*)(Account_t*))0x20174FD0;

//game function headers
void player_die(gentity_t*, gentity_t*, gentity_t*, int, int);

//global data
level_locals_t* g_level = (level_locals_t*)0x20ae90b8;

struct dyd_achievement *achievements[32];
int numericId = 1;



int Accounts_Stats_GetPlayerKills(Account_t *acc)
{
	if (acc)
	{
		char data[16];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_PLAYERKILLS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

int Accounts_Stats_GetPlayerDefeats(Account_t *acc)
{
	if (acc)
	{
		char data[16];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_PLAYERDEATHS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

void player_die(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, int meansOfDeath)
{
	if (attacker && self != attacker && self->s.number < MAX_CLIENTS && attacker->s.number < MAX_CLIENTS)
	{
		if (attacker->client->pers.Lmd.account)
		{
			char kills[16];
			sprintf(kills, "%s", Accounts_Custom_GetValue(attacker->client->pers.Lmd.account, "A_DATA_PLAYERKILLS"));

			int killnumber = strtol(kills, NULL, 0);

			if (killnumber)
			{
				killnumber++;
				sprintf(kills, "%d", killnumber);

				Accounts_Custom_SetValue(attacker->client->pers.Lmd.account, "A_DATA_PLAYERKILLS", kills);
			}
			else
				Accounts_Custom_SetValue(attacker->client->pers.Lmd.account, "A_DATA_PLAYERKILLS", "1");

			if (killnumber >= 1000 && Accounts_Custom_GetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL1") == NULL)
			{
				Accounts_Custom_SetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL1", "1");
				for (int i = 0; achievements[i] != NULL; i++)
				{
					if (!stricmp(achievements[i]->identifier, "A_FIGHT_PKILL1"))
					{
						attacker->client->pers.Lmd.account->credits += achievements[i]->reward_credits;
						g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("chat \"^7Player %s has completed achievement: %s\n\"", attacker->client->pers.netname, achievements[i]->name));
					}
				}
			}

			if (killnumber >= 4000 && Accounts_Custom_GetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL2") == NULL)
			{
				Accounts_Custom_SetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL2", "1");
				for (int i = 0; achievements[i] != NULL; i++)
				{
					if (!stricmp(achievements[i]->identifier, "A_FIGHT_PKILL2"))
					{
						attacker->client->pers.Lmd.account->credits += achievements[i]->reward_credits;
						g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("chat \"^7Player %s has completed achievement: %s\n\"", attacker->client->pers.netname, achievements[i]->name));
					}
				}
			}

			if (killnumber >= 9000 && Accounts_Custom_GetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL3") == NULL)
			{
				Accounts_Custom_SetValue(attacker->client->pers.Lmd.account, "A_FIGHT_PKILL3", "1");
				for (int i = 0; achievements[i] != NULL; i++)
				{
					if (!stricmp(achievements[i]->identifier, "A_FIGHT_PKILL3"))
					{
						attacker->client->pers.Lmd.account->credits += achievements[i]->reward_credits;
						g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("chat \"^7Player %s has completed achievement: %s\n\"", attacker->client->pers.netname, achievements[i]->name));
					}
				}
			}
		}

		if (self->client->pers.Lmd.account)
		{
			char deaths[16];
			sprintf(deaths, "%s", Accounts_Custom_GetValue(self->client->pers.Lmd.account, "A_DATA_PLAYERDEATHS"));

			int deathnumber = strtol(deaths, NULL, 0);

			if (deathnumber)
			{
				deathnumber++;
				sprintf(deaths, "%d", deathnumber);

				Accounts_Custom_SetValue(self->client->pers.Lmd.account, "A_DATA_PLAYERDEATHS", deaths);
			}
			else
				Accounts_Custom_SetValue(self->client->pers.Lmd.account, "A_DATA_PLAYERDEATHS", "1");
		}
	}

	__asm
	{
		mov esp, ebp
		pop ebp
	} //end function, but without returning to pre-call place

	execute_address((unsigned int)player_die_entry);
}


C_DLLEXPORT int JASS_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, int iscmd)
{
	JASS_SAVE_VARS();
	achievements_init();

	WriteProcessMemory(GetCurrentProcess(), (void*)PLAYER_DIE, player_die_patchdata, 6, NULL);

	iscmd = 0;
	return 1;
}

C_DLLEXPORT void JASS_Detach(int iscmd)
{
	WriteProcessMemory(GetCurrentProcess(), (void*)PLAYER_DIE, player_die_entry, 8, NULL);

	iscmd = 0;
}

C_DLLEXPORT void JASS_Query(plugininfo_t** pinfo)
{
	JASS_GIVE_PINFO();
}

C_DLLEXPORT int JASS_vmMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11)
{
	if (cmd == GAME_CLIENT_COMMAND) //arg0 = clientnum
	{
		char command[32];

		gentity_t* user = g_level->gentities + arg0;
		Account_t* acc = user->client->pers.Lmd.account;

		g_syscall(G_ARGV, 0, command, sizeof(command)); //getting cmd

		//flavor commands

		/*if (!stricmp(command, "slap"))
		{
			char buf[MAX_NETNAME];
			int result;
			if (g_syscall(G_ARGC) < 2)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3Usage: slap <name>\n\"");
				JASS_RET_SUPERCEDE(1);
			}
			g_syscall(G_ARGV, 1, buf, sizeof(buf));
			result = ClientNumberFromString(user, buf);
			if (result != -1)
			{
				gentity_t *victim = GetEnt(result);
				victim->client->ps.velocity[0] = 250;
				victim->client->ps.velocity[1] = 150;
				victim->client->ps.velocity[2] = 50;
				G_Knockdown(victim, 500);
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "cp \"%s has been slapped!\n\", victim->client->pers.netname");
			}
			JASS_RET_SUPERCEDE(1);
		}*/

		//achievements

		if (!stricmp(command, "achievements"))
		{
			char arg[16];

			if (!acc)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^2Register before using this command\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			if (g_syscall(G_ARGC) < 2)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3Usage: achievements <category> - list achievements in category\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^4Available categories:\nFight\nDuels\nMisc\nClaimable\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements claim <ID> - claims achievement completion\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements show <ID> - shows achievement description\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements help - shows detailed description of achievement system\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			g_syscall(G_ARGV, 1, arg, 16);

			if (!stricmp(arg, "fight"))
			{
				char arg[16];
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));
					if (!stricmp(arg, "ext"))
					{
						achievements_list(user, ACHIEVEMENT_FIGHT, qtrue);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_FIGHT, qfalse);

				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "duels"))
			{
				char arg[16];
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));
					if (!stricmp(arg, "ext"))
					{
						achievements_list(user, ACHIEVEMENT_DUELS, qtrue);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_DUELS, qfalse);

				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "misc"))
			{
				char arg[16];
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));
					if (!stricmp(arg, "ext"))
					{
						achievements_list(user, ACHIEVEMENT_MISC, qtrue);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_MISC, qfalse);

				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "claimable")) //logic separated from achievements_list() but similar
			{
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));
				}

				for (int i = 0; achievements[i] != NULL; i++)
				{
					if (achievements[i]->autoclaimable == qfalse)
					{
						if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, achievements[i]->identifier) == NULL)
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3%d. %s\n\"", achievements[i]->id_numeric, achievements[i]->name));
						}
						else
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^2%d. %s - COMPLETED\n\"", achievements[i]->id_numeric, achievements[i]->name));
						}

						if (!stricmp(arg, "ext"))
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^6Description: %s\n\"", achievements[i]->description));
							achievements_progress(user, achievements[i]->identifier, qtrue);
						}
					}
				}
				JASS_RET_SUPERCEDE(1);				
			}
				
			else if (!stricmp(arg, "help")) //end of categories
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^6Lugormod achievement system by ^0Dyd^1zio^6, version 0.1\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^5It allows two possible kinds of achievements: Claimable achievements, where player must force completion manually, and automatic achievements, where completion and reward are autogranted.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^5Achievements are assigned to categories, what becomes helpful if number of achievements is large. Special category \'claimable\' shows claimable achievements from all other categories.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3Possible syntax: \nachievements <category> - displays achievements from category.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements help - displays this help page \n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements show <ID> - displays detailed information about specific achievement\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements claim <ID> - use to complete claimable achievements.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1IMPORTANT! ID is number usually shown together with achievement name. Using anything else instead at places where ID is required will not work.\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "claim"))
			{
				char id[16];

				if (g_syscall(G_ARGC) < 3)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^4Usage: claim <achievement ID>\n\"");
				}
				else
				{
					g_syscall(G_ARGV, 2, id, sizeof(id));
					for (int i = 0; achievements[i] != NULL; i++)
					{
						if (achievements[i]->id_numeric == strtol(id, NULL, 0))
						{
							if (achievements[i]->autoclaimable == qfalse)
							{
								achievements_check(user, achievements[i]);
								JASS_RET_SUPERCEDE(1);
							}
							g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^7This achievement is not claimable! Its completion is granted automatically.\n\"");
							JASS_RET_SUPERCEDE(1);
						}
					}
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^7Wrong achievement ID!\n\"");
				}
				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "show"))
			{
				char id[16];

				if (g_syscall(G_ARGC) < 3)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^4Usage: show <achievement ID>\n\"");
				}
				else
				{
					g_syscall(G_ARGV, 2, id, sizeof(id));
					for (int i = 0; achievements[i] != NULL; i++)
					{
						if (achievements[i]->id_numeric == strtol(id, NULL, 0))
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^2%s\n^6%s\n\"", achievements[i]->name, achievements[i]->description)); 
							achievements_progress(user, achievements[i]->identifier, qtrue);
							JASS_RET_SUPERCEDE(1);
						}
					}
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^5Achievement not found!\n\"");
				}
				JASS_RET_SUPERCEDE(1);
			}

			else
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1Wrong syntax! Use ^2achievements help^1 for detailed help\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			JASS_RET_SUPERCEDE(1);
		}

	}
	JASS_RET_IGNORED(1);
}

C_DLLEXPORT int JASS_syscall(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12)
{
	JASS_RET_IGNORED(1);
}

C_DLLEXPORT int JASS_vmMain_Post(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11)
{
	JASS_RET_IGNORED(1);
}

C_DLLEXPORT int JASS_syscall_Post(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12)
{
	JASS_RET_IGNORED(1);
}


int achievements_progress(gentity_t *user, const char *x, qboolean print) //check progress, can be used in unlock to check requirements, can be used to display progress
{
	if (!stricmp(x, "A_MISC_PLAYTIME1"))
	{
		int time = user->client->pers.Lmd.account->time; //seconds
		int hours = time / 3600;
		if (time >= 90000)
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/25 hours - you finished the goal\n\"", hours));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/25 hours\n\"", hours));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_MISC_PLAYTIME2"))
	{
		int time = user->client->pers.Lmd.account->time; //seconds
		int hours = time / 3600;
		if (time >= 360000)
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/100 hours - you finished the goal\n\"", hours));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/100 hours\n\"", hours));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_PKILL1"))
	{
		int kills = Accounts_Stats_GetPlayerKills(user->client->pers.Lmd.account); //seconds
		//int deaths = Accounts_Stats_GetPlayerDefeats(user->client->pers.Lmd.account); //in case we want to show kill / death ratio
		if (kills >= 1000)
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/1000 kills - you finished the goal\n\"", kills));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/1000 kills\n\"", kills));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_PKILL2"))
	{
		int kills = Accounts_Stats_GetPlayerKills(user->client->pers.Lmd.account); //seconds
		//int deaths = Accounts_Stats_GetPlayerDefeats(user->client->pers.Lmd.account); //in case we want to show kill / death ratio
		if (kills >= 4000)
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/4000 kills - you finished the goal\n\"", kills));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/4000 kills\n\"", kills));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_PKILL3"))
	{
		int kills = Accounts_Stats_GetPlayerKills(user->client->pers.Lmd.account); //seconds
		//int deaths = Accounts_Stats_GetPlayerDefeats(user->client->pers.Lmd.account); //in case we want to show kill / death ratio
		if (kills >= 9000)
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/9000 kills - you finished the goal\n\"", kills));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/9000 kills\n\"", kills));
			}
			return 0;
		}
	}

	else return -1; //achievement not found by identifier
}


void achievements_check(gentity_t *user, dyd_achievement *x) //achievement unlock logic
{
	int state = achievements_progress(user, x->identifier, qtrue);

	if (state != -1)
	{	
		if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, x->identifier) == NULL)
		{
			if (state == 1)
			{
				Accounts_Custom_SetValue(user->client->pers.Lmd.account, x->identifier, "1");

				user->client->pers.Lmd.account->credits += x->reward_credits; //extend if new types of rewards added

				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^2Achievement unlocked successfully!\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("chat \"^7Player %s has completed achievement: %s\n\"", user->client->pers.netname, x->name));
			}
			else
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^3You do not meet requirements to claim this achievement!\n\"");
		}
		else
			g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^3This achievement is already completed!\n\"");
	}

	else g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^1Internal error! No claiming mechanics for valid achievement\n\""); //achievement not found by identifier
}



void achievements_list(gentity_t *user, enum dyd_achievement_types type, qboolean extended) //this does NOT handle "claimable" category
{
	for (int i = 0; achievements[i] != NULL; i++)
	{
		if (achievements[i]->type == type)
		{
			if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, achievements[i]->identifier) == NULL) //checking for completion
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%d. %s ^5%s\"", achievements[i]->id_numeric, achievements[i]->name, (achievements[i]->autoclaimable == qfalse) ? "(claimable)\n" : "\n") );
			}
			else
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%d. %s ^5%s\"", achievements[i]->id_numeric, achievements[i]->name, (achievements[i]->autoclaimable == qfalse) ? "(claimable)^2 - COMPLETED\n" : "^2 - COMPLETED \n"));
			}

			if (extended == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^6Description: %s\n\"", achievements[i]->description));
				achievements_progress(user, achievements[i]->identifier, qtrue);
			}
		}
	}
}


void achievements_init() //server start achievement allocation, change achievements_progress() too for not autoclaimable achievements if adding new
{
	/*TEMPLATE
	achievements[x] = (dyd_achievement*)malloc(sizeof dyd_achievement);
	achievements[x]->type = ACHIEVEMENT_TYPE enum;
	achievements[x]->id_numeric = numericId++;
	achievements[x]->identifier = "A_TYPE_NAME";
	achievements[x]->name = "NAME";
	achievements[x]->description = "DESCRIPTION";
	achievements[x]->reward_credits = NUMBER;
	achievements[x]->autoclaimable = qfalse / qtrue;
	*/

	achievements[0] = (dyd_achievement*) malloc(sizeof dyd_achievement);
	achievements[0]->type = ACHIEVEMENT_MISC;
	achievements[0]->id_numeric = numericId++;
	achievements[0]->identifier = "A_MISC_PLAYTIME1";
	achievements[0]->name = "Loyal player";
	achievements[0]->description = "Spend 25 hours total on the server. You can check total time spent using /stats command. Reward: 20000 credits";
	achievements[0]->reward_credits = 20000;
	achievements[0]->autoclaimable = qfalse;

	achievements[1] = (dyd_achievement*)malloc(sizeof dyd_achievement);
	achievements[1]->type = ACHIEVEMENT_MISC;
	achievements[1]->id_numeric = numericId++;
	achievements[1]->identifier = "A_MISC_PLAYTIME2";
	achievements[1]->name = "Follower";
	achievements[1]->description = "Spend 100 hours total on the server. You can check total time spent using /stats command. Reward: 90000 credits";
	achievements[1]->reward_credits = 90000;
	achievements[1]->autoclaimable = qfalse;

	achievements[2] = (dyd_achievement*)malloc(sizeof dyd_achievement);
	achievements[2]->type = ACHIEVEMENT_FIGHT;
	achievements[2]->id_numeric = numericId++;
	achievements[2]->identifier = "A_FIGHT_PKILL1";
	achievements[2]->name = "Soldier";
	achievements[2]->description = "Kill 1000 players. Reward: 25000 credits";
	achievements[2]->reward_credits = 25000;
	achievements[2]->autoclaimable = qtrue;

	achievements[3] = (dyd_achievement*)malloc(sizeof dyd_achievement);
	achievements[3]->type = ACHIEVEMENT_FIGHT;
	achievements[3]->id_numeric = numericId++;
	achievements[3]->identifier = "A_FIGHT_PKILL2";
	achievements[3]->name = "Battlemaster";
	achievements[3]->description = "Kill 4000 players. Reward: 100000 credits";
	achievements[3]->reward_credits = 100000;
	achievements[3]->autoclaimable = qtrue;

	achievements[4] = (dyd_achievement*)malloc(sizeof dyd_achievement);
	achievements[4]->type = ACHIEVEMENT_FIGHT;
	achievements[4]->id_numeric = numericId++;
	achievements[4]->identifier = "A_FIGHT_PKILL3";
	achievements[4]->name = "Conqueror";
	achievements[4]->description = "Kill 9000 players. Reward: 160000 credits";
	achievements[4]->reward_credits = 160000;
	achievements[4]->autoclaimable = qtrue;

}