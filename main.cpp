#include "hookedfunctions.h"

#pragma warning (disable: 4996)

//plugin info
plugininfo_t g_plugininfo = { "dydplugin", "1.0.1", "Lugormod U# 2.4.9.0 Achievement System", "Dydzio", "", 1, 1, 1, JASS_PIFV_MAJOR, JASS_PIFV_MINOR };
pluginres_t* g_result = NULL;
eng_syscall_t g_syscall = NULL;
mod_vmMain_t g_vmMain = NULL;
pluginfuncs_t* g_pluginfuncs = NULL;
int g_vmbase = 0;

//global data
extern struct dyd_achievement achievements[MAX_ACHIEVEMENTS];

extern level_locals_t* g_level;
extern BYTE oldPlayerDie[6];
extern jmp_far jump;

int dydmove_cooldown[MAX_CLIENTS] = { 0 };


C_DLLEXPORT int JASS_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, int iscmd)
{
	JASS_SAVE_VARS();
	achievements_init();

	jump.instr_push = 0x68;
	jump.instr_ret = 0xC3;

	jump.arg = (DWORD)(&player_die);
	ReadProcessMemory(GetCurrentProcess(), (void*)PLAYER_DIE, (void*)&oldPlayerDie, 6, NULL);
	WriteProcessMemory(GetCurrentProcess(), (void*)PLAYER_DIE, (void*)&jump, 6, NULL);

	iscmd = 0;
	return 1;
}

C_DLLEXPORT void JASS_Detach(int iscmd)
{
	WriteProcessMemory(GetCurrentProcess(), (void*)PLAYER_DIE, (void*)&oldPlayerDie, 6, NULL);
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

		if (!stricmp(command, "saberbarrier") && acc && Accounts_Custom_GetValue(acc, "A_DUELS_ENGAGE3") != NULL)
		{
			if (user->client->sess.spectatorState)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1You cannot use this command while spectating.\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			if (user->client->ps.pm_type == PM_DEAD)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1You cannot use this command when you are dead.\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			if (user->client->ps.m_iVehicleNum)
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1You cannot use this command while using a vehicle.\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			if (!(user->client->ps.weapon == WP_SABER && user->client->ps.saberHolstered != 2))
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1You must have saber up to use this skill.\n\"");
				JASS_RET_SUPERCEDE(1);
			}

			if (g_level->time - dydmove_cooldown[arg0] > 20000)
			{
				dydmove_cooldown[arg0] = g_level->time;
				user->client->invulnerableTimer = 0;
				user->client->ps.saberMove = 50; //change to proper one before release
				user->client->ps.saberBlocked = BLOCKED_BOUNCE_MOVE;
			}
			else
				g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1You have to wait %d seconds before using this skill again.\n\"", (20000 - (g_level->time - dydmove_cooldown[arg0]) ) / 1000 ));

			JASS_RET_SUPERCEDE(1);
		}

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
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^5Available categories:\nFight\nDuels\nMisc\nServerhelper\nClaimable\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements claim <ID> - claims achievement completion\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements show <ID> - shows achievement description\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements help - shows detailed description of achievement system\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1Remember that all achievements require 20 hours played on account before they can be completed.\n\"");
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

			else if (!stricmp(arg, "serverhelper"))
			{
				char arg[16];
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));
					if (!stricmp(arg, "ext"))
					{
						achievements_list(user, ACHIEVEMENT_HELPER, qtrue);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_HELPER, qfalse);

				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "claimable")) //logic separated from achievements_list() but similar
			{
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));
				}

				for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
				{
					if (achievements[i].autoclaimable == qfalse)
					{
						if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, achievements[i].identifier) == NULL)
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^3%d. %s\n\"", achievements[i].id_numeric, achievements[i].name));
						}
						else
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^2%d. %s - COMPLETED\n\"", achievements[i].id_numeric, achievements[i].name));
						}

						if (!stricmp(arg, "ext"))
						{
							g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^6Description: %s\n\"", achievements[i].description));
							achievements_progress(user, achievements[i].identifier, qtrue);
						}
					}
				}
				JASS_RET_SUPERCEDE(1);				
			}
				
			else if (!stricmp(arg, "help")) //end of categories
			{
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^6Lugormod achievement system by ^0Dyd^1zio^6, version 1.0\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^5It allows two possible kinds of achievements: Claimable achievements, where player must force completion manually, and automatic achievements, where completion and reward are autogranted.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^5Achievements are assigned to categories, what becomes helpful if number of achievements is large. Special category \'claimable\' shows claimable achievements from all other categories.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3Possible syntax: \nachievements <category> - displays achievements from category.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements help - displays this help page \n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements show <ID> - displays detailed information about specific achievement\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3achievements claim <ID> - use to complete claimable achievements.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1IMPORTANT! ID is number usually shown together with achievement name. Using anything else instead at places where ID is required will not work.\n\"");
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1ALL achievements have requirement of 20 hours played on account before any of them can be completed.\n\"");
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
					for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
					{
						if (achievements[i].id_numeric == strtol(id, NULL, 0))
						{
							if (achievements[i].autoclaimable == qfalse)
							{
								achievements_check(user, &achievements[i], qtrue);
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

					dyd_achievement *x = FindAchievementById(strtol(id, NULL, 0));
					if (x != NULL)
					{
						g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^2%s\n^6%s\n\"", x->name, x->description));
						achievements_progress(user, x->identifier, qtrue);
						JASS_RET_SUPERCEDE(1);
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

	if (cmd == GAME_CLIENT_CONNECT)
	{
		dydmove_cooldown[arg0] = 0;
		JASS_RET_IGNORED(1);
	}

	if (cmd == GAME_CLIENT_DISCONNECT)
	{
		dydmove_cooldown[arg0] = 0;
		JASS_RET_IGNORED(1);
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
