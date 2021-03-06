#include "hookedfunctions.h"

#pragma warning (disable: 4996)

//plugin info
plugininfo_t g_plugininfo = { "dydplugin", "1.2.2", "Lugormod U# 2.4.9 Achievement System", "Dydzio", "", 1, 1, 1, JASS_PIFV_MAJOR, JASS_PIFV_MINOR }; //when changing version, update /achievements help version too
pluginres_t* g_result = NULL;
eng_syscall_t g_syscall = NULL;
mod_vmMain_t g_vmMain = NULL;
pluginfuncs_t* g_pluginfuncs = NULL;
int g_vmbase = 0;

//global data
extern jmp_far jump;

extern struct dyd_achievement achievements[MAX_ACHIEVEMENTS];

extern level_locals_t* g_level;

extern struct dyd_playerdata dyd_data;



C_DLLEXPORT int JASS_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, int iscmd)
{
	JASS_SAVE_VARS();
	achievements_init();

	/*jump.instr_push = 0x68;
	jump.instr_ret = 0xC3;*/

	memset(&dyd_data, 0, sizeof(dyd_data));
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

		if (!user || !user->client || user->s.number >= MAX_CLIENTS || !user->client->pers.connected) JASS_RET_IGNORED(1); //anti-exploit

		Account_t* acc = user->client->pers.Lmd.account;

		g_syscall(G_ARGV, 0, command, sizeof(command)); //getting cmd

		//flavor commands

		if (!stricmp(command, "removeachievementfromacc") && acc)
		{
			if (Auths_GetPlayerRank(user) == 1)
			{
				if (g_syscall(G_ARGC) > 2)
				{
					char arg1[40];
					char arg2[16];
					g_syscall(G_ARGV, 1, arg1, sizeof(arg1));
					g_syscall(G_ARGV, 2, arg2, sizeof(arg2));
					Account_t* targetAccount = Accounts_GetByUsername(arg1);
					if (targetAccount != NULL)
					{
						int ID = strtol(arg2, NULL, 0);
						if (ID)
						{
							dyd_achievement *achievementToRemove = FindAchievementById(ID);
							if (achievementToRemove != NULL)
							{
								char* bitmaskValue = Accounts_Custom_GetValue(targetAccount, "ACHIEVEMENTS");
								if (bitmaskValue != NULL && (unsigned long)strtol(bitmaskValue, NULL, 0) & GetAchievementBitmaskFromID(ID))
								{
									unsigned long newBitmask = (unsigned long)strtol(bitmaskValue, NULL, 0) & ~(GetAchievementBitmaskFromID(ID));
									Accounts_Custom_SetValue(targetAccount, "ACHIEVEMENTS", JASS_VARARGS("%lu", newBitmask));
									g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1Achievement removed from account.\n\"");

									if (targetAccount->credits >= achievementToRemove->reward_credits)
									{
										targetAccount->credits -= achievementToRemove->reward_credits;
										targetAccount->modifiedTime = g_level->time;
									}
									else
										g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^6Credits were not taken away - insufficient amount.\n\"");
								}
								else
									g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1Player did not complete mentioned achievement.\n\"");
							}
							else
								g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1Wrong achievement ID.\n\"");							
						}
						else
							g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1Wrong achievement ID.\n\"");
					}
					else
						g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1Account not found.\n\"");
				}
				else
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1Usage: removeachievementfromacc (username) (achievement ID).\n\"");

				JASS_RET_SUPERCEDE(1);
			}
			JASS_RET_IGNORED(1);
		}

		if (!stricmp(command, "saberbarrier") && acc)
		{
			char* bitmaskValue = Accounts_Custom_GetValue(user->client->pers.Lmd.account, "ACHIEVEMENTS");
			if (bitmaskValue != NULL && (unsigned long)strtol(bitmaskValue, NULL, 0) & GetAchievementBitmaskFromID(A_DUELS_ENGAGE3))
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

				if (g_level->time - dyd_data.saberbarrier_cooldown[arg0] > 20000)
				{
					dyd_data.saberbarrier_cooldown[arg0] = g_level->time;
					user->client->invulnerableTimer = 0;
					user->client->ps.saberMove = 50; //change to proper one before release
					user->client->ps.saberBlocked = BLOCKED_BOUNCE_MOVE;
				}
				else
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1You have to wait %d seconds before using this skill again.\n\"", (20000 - (g_level->time - dyd_data.saberbarrier_cooldown[arg0])) / 1000));
				JASS_RET_SUPERCEDE(1);
			}
			JASS_RET_IGNORED(1);
		}

		if (!stricmp(command, "mentalshield") && acc)
		{
			char* bitmaskValue = Accounts_Custom_GetValue(user->client->pers.Lmd.account, "ACHIEVEMENTS");
			if (bitmaskValue != NULL && (unsigned long)strtol(bitmaskValue, NULL, 0) & GetAchievementBitmaskFromID(A_MISC_PLAYTIME3))
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

				if (user->client->ps.duelInProgress)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1You cannot use this command while dueling.\n\"");
					JASS_RET_SUPERCEDE(1);
				}

				if ((user->client->ps.fd.forceGripUseTime > g_level->time || user->client->ps.forceHandExtend != HANDEXTEND_NONE
					|| (user->client->ps.weaponTime > 0) && (!(user->client->Lmd.restrict & 16) || user->client->pers.Lmd.persistantFlags & 16))
					|| (user->client->ps.weapon != WP_SABER && (user->client->ps.weaponstate == WEAPON_CHARGING || user->client->ps.weaponstate == WEAPON_CHARGING_ALT)))
					// im aware i shud kill self for dis
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1You cannot use this command while busy.\n\"");
					JASS_RET_SUPERCEDE(1);
				}

				if (g_level->time - dyd_data.mentalshield_cooldown[arg0] > 60000)
				{
					dyd_data.mentalshield_cooldown[arg0] = g_level->time;
					user->client->invulnerableTimer = g_level->time + 3000;
					user->client->ps.eFlags |= EF_INVULNERABLE;
				}
				else
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1You have to wait %d seconds before using this skill again.\n\"", (60000 - (g_level->time - dyd_data.mentalshield_cooldown[arg0])) / 1000));
				JASS_RET_SUPERCEDE(1);
			}
		}

		if (!stricmp(command, "bladetornado") && acc)
		{
			char* bitmaskValue = Accounts_Custom_GetValue(user->client->pers.Lmd.account, "ACHIEVEMENTS");
			if (bitmaskValue != NULL && (unsigned long)strtol(bitmaskValue, NULL, 0) & GetAchievementBitmaskFromID(A_DUELS_ENGAGE4))
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

				if (user->client->ps.duelInProgress)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1You cannot use this command while dueling.\n\"");
					JASS_RET_SUPERCEDE(1);
				}

				if (fabs(user->client->ps.velocity[0]) > 50.0f || fabs(user->client->ps.velocity[1]) > 50.0f || fabs(user->client->ps.velocity[2]) > 50.0f)
				{
					g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^1You have to stand still when using this skill.\n\"");
					JASS_RET_SUPERCEDE(1);
				}

				if (g_level->time - dyd_data.bladetornado_cooldown[arg0] > 60000)
				{
					dyd_data.bladetornado_cooldown[arg0] = g_level->time;
					user->client->invulnerableTimer = 0;
					user->client->ps.saberMove = 58; //change to proper one before release
					user->client->ps.saberBlocked = BLOCKED_BOUNCE_MOVE;
				}
				else
					g_syscall(G_SEND_SERVER_COMMAND, arg0, JASS_VARARGS("print \"^1You have to wait %d seconds before using this skill again.\n\"", (60000 - (g_level->time - dyd_data.bladetornado_cooldown[arg0])) / 1000));
				JASS_RET_SUPERCEDE(1);
			}
			JASS_RET_IGNORED(1);
		}

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
				//change to dispcontiguous in future version
				g_syscall(G_SEND_SERVER_COMMAND, arg0, "print \"^3Usage: achievements <category> - list achievements in category\n^5Available categories: Fight, Duels, Misc, Serverhelper, Claimable, ^6Legendary\n^3achievements claim <ID> - claims achievement completion\n^3achievements show <ID> - shows achievement description\n^3achievements help - shows detailed description of achievement system\n^1Remember that all achievements require 20 hours played on account before they can be completed.\n\"");
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
						achievements_list(user, ACHIEVEMENT_FIGHT, qtrue, qfalse);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_FIGHT, qfalse, qfalse);

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
						achievements_list(user, ACHIEVEMENT_DUELS, qtrue, qfalse);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_DUELS, qfalse, qfalse);

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
						achievements_list(user, ACHIEVEMENT_MISC, qtrue, qfalse);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_MISC, qfalse, qfalse);

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
						achievements_list(user, ACHIEVEMENT_HELPER, qtrue, qfalse);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_HELPER, qfalse, qfalse);

				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "legendary"))
			{
				char arg[16];
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));
					if (!stricmp(arg, "ext"))
					{
						achievements_list(user, ACHIEVEMENT_LEGENDARY, qtrue, qfalse);
						JASS_RET_SUPERCEDE(1);
					}
				}

				achievements_list(user, ACHIEVEMENT_LEGENDARY, qfalse, qfalse);

				JASS_RET_SUPERCEDE(1);
			}

			else if (!stricmp(arg, "claimable"))
			{
				char arg[16];
				if (g_syscall(G_ARGC) > 2)
				{
					g_syscall(G_ARGV, 2, arg, sizeof(arg));
					if (!stricmp(arg, "ext"))
					{
						achievements_list(user, ACHIEVEMENT_NONE, qtrue, qtrue);
						JASS_RET_SUPERCEDE(1);
					}
				}
				achievements_list(user, ACHIEVEMENT_NONE, qfalse, qtrue);
				JASS_RET_SUPERCEDE(1);				
			}
				
			else if (!stricmp(arg, "help")) //end of categories
			{
				DispContiguous(user, "^6Lugormod achievement system by ^0Dyd^1zio^6, version 1.2.2");
				DispContiguous(user, "^5It allows two possible kinds of achievements: Claimable achievements, where player must force completion manually, and automatic achievements, where completion and reward are autogranted.");
				DispContiguous(user, "^5Achievements are assigned to categories, what becomes helpful if number of achievements is large. Special category \'claimable\' shows claimable achievements from all other categories.");
				DispContiguous(user, "^3Possible syntax: \nachievements <category> - displays achievements from category.");
				DispContiguous(user, "^3achievements help - displays this help page");
				DispContiguous(user, "^3achievements show <ID> - displays detailed information about specific achievement");
				DispContiguous(user, "^3achievements claim <ID> - use to complete claimable achievements.");
				DispContiguous(user, "^1IMPORTANT! ID is number usually shown together with achievement name. Using anything else instead at places where ID is required will not work.");
				DispContiguous(user, "^1ALL achievements have requirement of 20 hours played on account before any of them can be completed.");
				DispContiguous(user, NULL);
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
						if (achievements[i].id == strtol(id, NULL, 0))
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
						achievements_progress(user, x->id, qtrue);
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

	/*if (cmd == GAME_CLIENT_CONNECT)
	{
		JASS_RET_IGNORED(1);
	}*/

	if (cmd == GAME_CLIENT_DISCONNECT)
	{
		dyd_data.saberbarrier_cooldown[arg0] = 0;
		dyd_data.weapon_on_death[arg0] = 0;
		dyd_data.mentalshield_cooldown[arg0] = 0;
		dyd_data.bladetornado_cooldown[arg0] = 0;
		JASS_RET_IGNORED(1);
	}

	if (cmd == GAME_CLIENT_THINK)
	{
		gentity_t* user = g_level->gentities + arg0;
		if (!user || !user->client || user->s.number >= MAX_CLIENTS || !user->client->pers.connected) JASS_RET_IGNORED(1); //anti-exploit

		if (user->health > 0 && user->client->ps.weapon != WP_NONE) //WP_NONE - invalid weapon
		{
			dyd_data.weapon_on_death[arg0] = user->client->ps.weapon;
		}

		JASS_RET_IGNORED(1);
	}

	JASS_RET_IGNORED(1);
}

C_DLLEXPORT int JASS_syscall(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12)
{
	if (cmd == G_UNLINKENTITY)
	{
		if (((gentity_t*)arg0)->s.eType == 111)
		{
			gentity_t* self = GetEnt(((gentity_t*)arg0)->s.otherEntityNum);
			gentity_t* attacker = GetEnt(((gentity_t*)arg0)->s.otherEntityNum2);
			int meansOfDeath = ((gentity_t*)arg0)->s.eventParm;
			player_die_new(self, attacker, attacker, 0, meansOfDeath);
		}
	}

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
