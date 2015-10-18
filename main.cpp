#include "jassapi.h" 
#include "jka_sdk/game/g_local.h"
#include "achievements.h"

#pragma warning (disable: 4996)

void(*G_Knockdown)(gentity_t* victim, int duration) = (void(*)(gentity_t*, int))0x200ce8b0; //set ent->client->ps.velocity 1st
void (*G_Damage2)(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, float *dir, float *point, int damage, int dflags, int mod) = (void(*)(gentity_t*, gentity_t*, gentity_t*, float*, float*, int, int, int))0x200C58E0;
char* (*Accounts_Custom_GetValue)(Account_t* acc, const char *key) = (char*(*)(Account_t*, const char*))0x201712e0; //KEY/VALUE acc data read, data not existing = return NULL
void(*Accounts_Custom_SetValue)(Account_t* acc, const char *key, const char *val) = (void(*)(Account_t*, const char*, const char*)) 0x20171350;//KEY/VALUE acc data write
int(*ClientNumberFromString)(gentity_t* to, const char* s) = (int(*)(gentity_t*, const char*))0x200b5a10; //as name says
gentity_t* (*GetEnt)(int index) = (gentity_t*(*)(int))0x20193150; //return entity struct for entity with proper index

//int __cdecl Accounts_Stats_GetDeaths(Account_s *acc) ...


plugininfo_t g_plugininfo = { "", "", "", "", "", 1, 1, 1, JASS_PIFV_MAJOR, JASS_PIFV_MINOR };
pluginres_t* g_result = NULL;
eng_syscall_t g_syscall = NULL;
mod_vmMain_t g_vmMain = NULL;
pluginfuncs_t* g_pluginfuncs = NULL;
int g_vmbase = 0;

level_locals_t* g_level = (level_locals_t*)0x20ae90b8;

struct dyd_achievement *achievements[32];


C_DLLEXPORT int JASS_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, int iscmd)
{
	JASS_SAVE_VARS();

	achievements_init();

	iscmd = 0;
	return 1;
}

C_DLLEXPORT void JASS_Detach(int iscmd)
{
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

		if (!stricmp(command, "dydtest")) //stricmp - POSIX caseless string compare
		{
			g_syscall(G_SEND_SERVER_COMMAND, -1, "cp \"Hello from ^0Dyd^1zio^2's plugin\n\"");
			JASS_RET_SUPERCEDE(1);
		}

		if (!stricmp(command, "dyd_help"))
		{
			g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"Availble cmds:\n\"");
			g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"dydtest\n\"");
			g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"dyd_damage\n\"");
			g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1dyd_shoot\n\"");
			g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"dyd_redbull\n\"");
			g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"dyd_slap\n\"");
			g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"dyd_achievements\n\"");
			JASS_RET_SUPERCEDE(1);
		}

		if (!stricmp(command, "dyd_slap"))
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
				victim->client->ps.velocity[1] = 250;
				victim->client->ps.velocity[2] = 150;
				victim->client->ps.velocity[3] = 50;
				G_Knockdown(victim, 500);
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "cp \"%s has been slapped!\n\", victim->client->pers.netname");
			}
			JASS_RET_SUPERCEDE(1);
		}

		if (!stricmp(command, "dyd_knockdown"))
		{
			char buf[16];
			int time;
			if (g_syscall(G_ARGC) < 2)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3Usage: knockdown <time in ms>\n\"");
				JASS_RET_SUPERCEDE(1);
			}
			g_syscall(G_ARGV, 1, buf, sizeof(buf));
			time = strtol(buf, NULL, 0);
			G_Knockdown(user, time);
			g_syscall(G_SEND_SERVER_COMMAND, arg0, "chat \"Knockdown!\n\"");
			JASS_RET_SUPERCEDE(1);
		}

		if (!stricmp(command, "dyd_damage"))
		{
			int damage;
			char buf[16];

			if (g_syscall(G_ARGC) < 2)
			{
				JASS_RET_SUPERCEDE(1);
			}
			g_syscall(G_ARGV, 1, buf, sizeof(buf));
			damage = 2 * strtol(buf, NULL, 0);
			G_Damage2(user, user, user, 0, 0, damage, 0, 0);
			JASS_RET_SUPERCEDE(1);
		}

		if (!stricmp(command, "dyd_shoot"))
		{
			JASS_RET_SUPERCEDE(1);
		}

		if (!stricmp(command, "dyd_redbull"))
		{
			user->client->Lmd.customSpeed.time = g_level->time + 15000; // g_level->time - time since server start
			user->client->Lmd.customSpeed.value = 350;
			JASS_RET_SUPERCEDE(1);
		}

		//achievements

		if (!stricmp(command, "dyd_achievements"))
		{
			char arg[16];

			if (!acc)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^2Register before using this command\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			if (g_syscall(G_ARGC) < 2)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3Usage: dyd_achievements <category> (ext) - list achievements in category, add word \'ext\' at the end to force more detailed description\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^4Available categories:\nFight\nDuels\nMisc\nClaimable\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3dyd_achievements claim <ID> - claims achievement completion\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3dyd_achievements show <ID> - shows achievement description\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3dyd_achievements help - shows detailed description of achievement system\n\"");
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
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Unknown modifier: %s\n\"", arg));
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
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Unknown modifier: %s\n\"", arg));
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
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Unknown modifier: %s\n\"", arg));
				}

				achievements_list(user, ACHIEVEMENT_MISC, qfalse);

				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "claimable")) //logic separated from achievements_list() but similar
			{
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));

					if (stricmp(arg, "ext")) //if NOT "ext"
					{
						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1Unknown modifier: %s\n\"", arg));
					}
				}

				for (int i = 0; achievements[i] != NULL; i++)
				{
					if (achievements[i]->autoclaimable == qfalse)
					{
						if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, achievements[i]->identifier) == NULL)
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3%s (ID: %s)\n\"", achievements[i]->name, achievements[i]->identifier));
						}
						else
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^2%s (ID: %s) - COMPLETED\n\"", achievements[i]->name, achievements[i]->identifier));
						}

						if (!stricmp(arg, "ext"))
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^6 Description: %s\n\"", achievements[i]->description));
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
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3Possible syntax: achievements <category> (ext) - displays achievements from category. Optional word ^2ext^3 at the end forces detailed info for each achievement.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements help - displays this help page \n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements show <ID> - displays detailed information about specific achievement\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements claim <ID> - use to complete claimable achievements.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1IMPORTANT! ID is text identifier usually shown together with achievement name. Using anything else instead at places where ID is required will not work.\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "claim"))
			{
				char name[64];

				if (g_syscall(G_ARGC) < 3)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^4Usage: claim <achievement ID>\n\"");
				}
				else
				{
					g_syscall(G_ARGV, 2, name, sizeof(name));
					for (int i = 0; achievements[i] != NULL; i++)
					{
						if (!stricmp(achievements[i]->identifier, name))
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
				char name[64];

				if (g_syscall(G_ARGC) < 3)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^4Usage: show <achievement ID>\n\"");
				}
				else
				{
					g_syscall(G_ARGV, 2, name, 64);
					for (int i = 0; achievements[i] != NULL; i++)
					{
						if (!stricmp(achievements[i]->identifier, name))
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^2%s\n\"",achievements[i]->description)); 
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
	if (!stricmp(x, "ACHIEVEMENT_MISC_PLAYTIME1"))
	{
		int time = user->client->pers.Lmd.account->time;
		if (time >= 90000)
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/25 hours - you finished the goal\n\"", time / 3600000));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/25 hours\n\"", time / 3600000));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "ACHIEVEMENT_MISC_PLAYTIME2"))
	{
		int time = user->client->pers.Lmd.account->time;
		if (time >= 360000)
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/25 hours - you finished the goal\n\"", time / 3600000));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/100 hours\n\"", time / 3600000));
			}
			return 0;
		}
	}

	else return -1; //achievement not found by identifier
}


void achievements_check(gentity_t *user, dyd_achievement *x) //achievement unlock logic, TODO: check if adding credits to achievements struct will help with scalability
{
	char arg[64]; // arg = x to uppercase
	for (int i = 0; x->identifier[i] != NULL; i++)
	{
		arg[i] = toupper(x->identifier[i]);
	}

	int state = achievements_progress(user, arg, qtrue);

	if (state != -1)
	{	
		if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, arg) == NULL)
		{
			if (state == 1)
			{
				Accounts_Custom_SetValue(user->client->pers.Lmd.account, arg, "1");

				user->client->pers.Lmd.account->credits += x->reward_credits; //extend if new types of rewards added

				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^2Achievement unlocked successfully!\n\"");
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
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%s (ID: %s)\n\"", achievements[i]->name, achievements[i]->identifier));
			}
			else
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%s (ID: %s) - COMPLETED\n\"", achievements[i]->name, achievements[i]->identifier));
			}

			if (extended == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^6 Description: %s\n\"", achievements[i]->description));
				achievements_progress(user, achievements[i]->identifier, qtrue);
			}

			if (achievements[i]->autoclaimable == qfalse)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^5This is claimable achievement.\n\"");
			}
		}
	}
}


void achievements_init() //server start achievement allocation, change achievements_check() too for not autoclaimable achievements if adding new
{
	achievements[0] = (dyd_achievement*) malloc(sizeof dyd_achievement);
	achievements[0]->type = ACHIEVEMENT_MISC;
	achievements[0]->identifier = "ACHIEVEMENT_MISC_PLAYTIME1";
	achievements[0]->name = "Loyal player";
	achievements[0]->description = "Spend 25 hours total on the server. You can check total time spent using /stats command. Reward: 20000 credits";
	achievements[0]->reward_credits = 20000;
	achievements[0]->autoclaimable = qfalse;

	achievements[1] = (dyd_achievement*)malloc(sizeof dyd_achievement);
	achievements[1]->type = ACHIEVEMENT_MISC;
	achievements[1]->identifier = "ACHIEVEMENT_MISC_PLAYTIME2";
	achievements[1]->name = "Follower";
	achievements[1]->description = "Spend 100 hours total on the server. You can check total time spent using /stats command. Reward: 90000 credits";
	achievements[1]->reward_credits = 90000;
	achievements[1]->autoclaimable = qfalse;
}