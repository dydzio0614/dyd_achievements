#include "achievements.h"

struct dyd_achievement achievements[MAX_ACHIEVEMENTS];

level_locals_t* g_level = (level_locals_t*)LEVEL;
int numericId = 1;

dyd_achievement* FindAchievementById(int value)
{
	for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
	{
		if (achievements[i].id_numeric == value)
			return &achievements[i];
	}
	return NULL;
}

dyd_achievement* FindAchievementByTextIdentifier(const char *text)
{
	for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
	{
		if (!stricmp(text, achievements[i].identifier))
			return &achievements[i];
	}
	return NULL;
}

int achievements_progress(gentity_t *user, const char *x, qboolean print) //check progress, can be used in unlock to check requirements, can be used to display progress, WARNING: USES DispContiguous to display data, need to call DispContiguous(user, NULL) after func call
{
	if (!stricmp(x, "A_MISC_PLAYTIME1"))
	{
		int time = user->client->pers.Lmd.account->time; //seconds
		int hours = time / 3600;
		if (time >= 90000)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement : %d / 25 hours - you finished the goal\n", hours));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/25 hours - you finished the goal\n\"", hours));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/25 hours\n", hours));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/25 hours\n\"", hours));
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
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/100 hours - you finished the goal\n", hours));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/100 hours - you finished the goal\n\"", hours));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/100 hours\n", hours));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/100 hours\n\"", hours));
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
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/1000 kills - you finished the goal\n", kills));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/1000 kills - you finished the goal\n\"", kills));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/1000 kills\n", kills));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/1000 kills\n\"", kills));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_PKILL2"))
	{
		int kills = Accounts_Stats_GetPlayerKills(user->client->pers.Lmd.account); //seconds
																				   //int deaths = Accounts_Stats_GetPlayerDefeats(user->client->pers.Lmd.account); //in case we want to show kill / death ratio
		if (kills >= 5000)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/5000 kills - you finished the goal\n", kills));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/5000 kills - you finished the goal\n\"", kills));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/5000 kills\n", kills));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/5000 kills\n\"", kills));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_PKILL3"))
	{
		int kills = Accounts_Stats_GetPlayerKills(user->client->pers.Lmd.account); //seconds
																				   //int deaths = Accounts_Stats_GetPlayerDefeats(user->client->pers.Lmd.account); //in case we want to show kill / death ratio
		if (kills >= 10000)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/10000 kills - you finished the goal\n", kills));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/10000 kills - you finished the goal\n\"", kills));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/10000 kills\n", kills));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/10000 kills\n\"", kills));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_ENGAGE1"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);

		if (duels >= 500)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/500 duels - you finished the goal\n", duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/500 duels - you finished the goal\n\"", duels));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/500 duels\n", duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/500 duels\n\"", duels));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_ENGAGE2"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);

		if (duels >= 1500)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/1500 duels - you finished the goal\n", duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/1500 duels - you finished the goal\n\"", duels));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/1500 duels\n", duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/1500 duels\n\"", duels));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_ENGAGE3"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);

		if (duels >= 3000 && wins >= 1500)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/3000 duels, %d/1500 duels won - you finished the goal\n", duels, wins));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/3000 duels, %d/1500 duels won - you finished the goal\n\"", duels, wins));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/3000 duels, %d/1500 duels won\n", duels, wins));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/3000 duels, %d/1500 duels won\n\"", duels, wins));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_DRATIO1"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = 0;

		if ( (duels - wins) != 0) 
			ratio = (float)wins / (float)(duels - wins);

		if (duels >= 500 && ratio >= 1.0f)
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/500 duels, %.2f duel ratio (1.0 needed) - you finished the goal\n", duels, (float)ratio);
				DispContiguous(user, JASS_VARARGS("^2%s", buf));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%s\"", buf));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/500 duels, %.2f duel ratio (1.0 needed)\n", duels, (float)ratio);
				DispContiguous(user, JASS_VARARGS("^3%s", buf));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%s\"", buf));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_DRATIO2"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = 0;

		if ((duels - wins) != 0)
			ratio = (float)wins / (float)(duels - wins);

		if (duels >= 1250 && ratio >= 1.5f)
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/1250 duels, %.2f duel ratio (1.5 needed) - you finished the goal\n", duels, (float)ratio);
				DispContiguous(user, JASS_VARARGS("^2%s", buf));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%s\"", buf));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/1250 duels, %.2f duel ratio (1.5 needed)\n", duels, (float)ratio);
				DispContiguous(user, JASS_VARARGS("^3%s", buf));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%s\"", buf));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_DRATIO3"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = 0;

		if ((duels - wins) != 0)
			ratio = (float)wins / (float)(duels - wins);

		if (duels >= 2500 && ratio >= 2.0f)
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/2500 duels, %.2f duel ratio (2.0 needed) - you finished the goal\n", duels, (float)ratio);
				DispContiguous(user, JASS_VARARGS("^2%s", buf));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%s\"", buf));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/2500 duels, %.2f duel ratio (2.0 needed)\n", duels, (float)ratio);
				DispContiguous(user, JASS_VARARGS("^3%s", buf));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%s\"", buf));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_SELFSHOT1"))
	{
		int selfshots = Accounts_Stats_GetSelfshots(user->client->pers.Lmd.account); 
																					
		if (selfshots >= 100)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/100 suicidal shots - you finished the goal\n", selfshots));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/100 suicidal shots - you finished the goal\n\"", selfshots));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/100 suicidal shots\n", selfshots));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/100 suicidal shots\n\"", selfshots));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_AMMO1"))
	{
		int ammo = Accounts_Stats_GetShots(user->client->pers.Lmd.account);
		
		if (ammo >= 20000)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/20000 ammo fired - you finished the goal\n", ammo));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/20000 ammo fired - you finished the goal\n\"", ammo));
			}
			return 1;
		}

		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/20000 ammo fired\n", ammo));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/20000 ammo fired\n\"", ammo));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_AMMO2"))
	{
		int ammo = Accounts_Stats_GetShots(user->client->pers.Lmd.account);

		if (ammo >= 50000)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/50000 ammo fired - you finished the goal\n", ammo));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/50000 ammo fired - you finished the goal\n\"", ammo));
			}
			return 1;
		}

		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/50000 ammo fired\n", ammo));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/50000 ammo fired\n\"", ammo));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_FIGHT_AMMO3"))
	{
		int ammo = Accounts_Stats_GetShots(user->client->pers.Lmd.account);

		if (ammo >= 150000)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/150000 ammo fired - you finished the goal\n", ammo));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/150000 ammo fired - you finished the goal\n\"", ammo));
			}
			return 1;
		}

		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/150000 ammo fired\n", ammo));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/150000 ammo fired\n\"", ammo));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_MISC_STASH1"))
	{
		int stashes = Accounts_Stats_GetStashes(user->client->pers.Lmd.account);

		if (stashes >= 50)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/50 stashes - you finished the goal\n", stashes));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/50 stashes - you finished the goal\n\"", stashes));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/50 stashes\n", stashes));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/50 stashes\n\"", stashes));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_MISC_STASH2"))
	{
		int stashes = Accounts_Stats_GetStashes(user->client->pers.Lmd.account);

		if (stashes >= 200)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/200 stashes - you finished the goal\n", stashes));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/200 stashes - you finished the goal\n\"", stashes));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/200 stashes\n", stashes));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/200 stashes\n\"", stashes));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_PISTOL1"))
	{
		int pistol_duels = Accounts_Stats_GetPistolDuels(user->client->pers.Lmd.account);

		if (pistol_duels >= 250)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/250 pistol duels - you finished the goal\n", pistol_duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/250 pistol duels - you finished the goal\n\"", pistol_duels));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/250 pistol duels\n", pistol_duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/250 pistol duels\n\"", pistol_duels));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_PISTOL2"))
	{
		int pistol_duels = Accounts_Stats_GetPistolDuels(user->client->pers.Lmd.account);

		if (pistol_duels >= 750)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/750 pistol duels - you finished the goal\n", pistol_duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/750 pistol duels - you finished the goal\n\"", pistol_duels));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/750 pistol duels\n", pistol_duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/750 pistol duels\n\"", pistol_duels));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_PISTOL3"))
	{
		int pistol_duels = Accounts_Stats_GetPistolDuels(user->client->pers.Lmd.account);

		if (pistol_duels >= 1500)
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^2Your current progress of the achievement: %d/1500 pistol duels - you finished the goal\n", pistol_duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/1500 pistol duels - you finished the goal\n\"", pistol_duels));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^3Your current progress of the achievement: %d/1500 pistol duels\n", pistol_duels));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/1500 pistol duels\n\"", pistol_duels));
			}
			return 0;
		}
	}

	else return -1; //achievement not found by identifier
}


void achievements_check(gentity_t *user, dyd_achievement *x, qboolean print) //achievement unlock logic
{
	int state = achievements_progress(user, x->identifier, print);

	if (state != -1)
	{
		if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, x->identifier) == NULL)
		{
			if (state == 1)
			{
				if (user->client->pers.Lmd.account->time >= 72000)
				{
					Accounts_Custom_SetValue(user->client->pers.Lmd.account, x->identifier, "1");

					user->client->pers.Lmd.account->credits += x->reward_credits; //extend if new types of rewards added
					user->client->pers.Lmd.account->modifiedTime = g_level->time; //ALWAYS modify this field like that if doing direct changes to account fields

					if (print == qtrue) g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^2Achievement unlocked successfully!\n\"");
					g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("chat \"^7Player %s^7 has completed achievement: %s\n\"", user->client->pers.netname, x->name));
				}
				else
					if(print == qtrue) g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^3You have to play at least 20 hours on account to claim any achievement!\n\"");
			}
			else
				if (print == qtrue) g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^3You do not meet requirements to claim this achievement!\n\"");
		}
		else
			if (print == qtrue) g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^3This achievement is already completed!\n\"");
	}

	else if (print == qtrue) g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^1Internal error! No claiming mechanics for valid achievement\n\""); //achievement not found by identifier
}



void achievements_list(gentity_t *user, enum dyd_achievement_types type, qboolean extended) //this does NOT handle "claimable" category
{
	for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
	{
		if (achievements[i].type == type)
		{
			if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, achievements[i].identifier) == NULL) //checking for completion
			{
				DispContiguous(user, JASS_VARARGS("^3%d. %s ^5%s", achievements[i].id_numeric, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)\n" : "\n"));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%d. %s ^5%s\"", achievements[i].id_numeric, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)\n" : "\n"));
			}
			else
			{
				DispContiguous(user, JASS_VARARGS("^2%d. %s ^5%s", achievements[i].id_numeric, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)^2 - COMPLETED\n" : "^2 - COMPLETED \n"));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%d. %s ^5%s\"", achievements[i].id_numeric, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)^2 - COMPLETED\n" : "^2 - COMPLETED \n"));
			}

			if (extended == qtrue)
			{
				DispContiguous(user, JASS_VARARGS("^6Description: %s\n", achievements[i].description));
				//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^6Description: %s\n\"", achievements[i].description));
				achievements_progress(user, achievements[i].identifier, qtrue);
			}
		}
	}
	DispContiguous(user, NULL);
}


void achievements_init() //server start achievement allocation, change achievements_progress() too for not autoclaimable achievements if adding new
{
	/*TEMPLATE
	achievements[x].type = ACHIEVEMENT_TYPE enum;
	achievements[x].id_numeric = numericId++;
	achievements[x].identifier = "A_TYPE_NAME";
	achievements[x].name = "NAME";
	achievements[x].reward_credits = NUMBER;
	sprintf(achievements[x].description, "DESCRIPTION, reward: %d, achievements[x].reward_credits);
	achievements[x].autoclaimable = qfalse / qtrue;
	*/

	achievements[0].type = ACHIEVEMENT_MISC;
	achievements[0].id_numeric = numericId++;
	achievements[0].identifier = "A_MISC_PLAYTIME1";
	achievements[0].name = "Loyal player";
	achievements[0].reward_credits = 20000;
	sprintf(achievements[0].description, "Spend 25 hours total on the server. You can check total time spent using /stats command. Reward: %d credits", achievements[0].reward_credits);
	achievements[0].autoclaimable = qfalse;

	achievements[1].type = ACHIEVEMENT_MISC;
	achievements[1].id_numeric = numericId++;
	achievements[1].identifier = "A_MISC_PLAYTIME2";
	achievements[1].name = "Follower";
	achievements[1].reward_credits = 90000;
	sprintf(achievements[1].description, "Spend 100 hours total on the server. You can check total time spent using /stats command. Reward: %d credits", achievements[1].reward_credits);
	achievements[1].autoclaimable = qfalse;

	achievements[2].type = ACHIEVEMENT_FIGHT;
	achievements[2].id_numeric = numericId++;
	achievements[2].identifier = "A_FIGHT_PKILL1";
	achievements[2].name = "Soldier";
	achievements[2].reward_credits = 25000;
	sprintf(achievements[2].description, "Kill 1000 players. Reward: %d credits", achievements[2].reward_credits);
	achievements[2].autoclaimable = qtrue;

	achievements[3].type = ACHIEVEMENT_FIGHT;
	achievements[3].id_numeric = numericId++;
	achievements[3].identifier = "A_FIGHT_PKILL2";
	achievements[3].name = "Battlemaster";
	achievements[3].reward_credits = 100000;
	sprintf(achievements[3].description, "Kill 5000 players. Reward: %d credits", achievements[3].reward_credits);
	achievements[3].autoclaimable = qtrue;

	achievements[4].type = ACHIEVEMENT_FIGHT;
	achievements[4].id_numeric = numericId++;
	achievements[4].identifier = "A_FIGHT_PKILL3";
	achievements[4].name = "Conqueror";
	achievements[4].reward_credits = 160000;
	sprintf(achievements[4].description, "Kill 10000 players. Reward: (hidden)");
	achievements[4].autoclaimable = qtrue;

	achievements[5].type = ACHIEVEMENT_DUELS;
	achievements[5].id_numeric = numericId++;
	achievements[5].identifier = "A_DUELS_ENGAGE1";
	achievements[5].name = "Saberfighter";
	achievements[5].reward_credits = 25000;
	sprintf(achievements[5].description, "Play 500 saber duels. Reward: %d credits.", achievements[5].reward_credits);
	achievements[5].autoclaimable = qfalse;

	achievements[6].type = ACHIEVEMENT_DUELS;
	achievements[6].id_numeric = numericId++;
	achievements[6].identifier = "A_DUELS_ENGAGE2";
	achievements[6].name = "Saber fan";
	achievements[6].reward_credits = 60000;
	sprintf(achievements[6].description, "Play 1500 saber duels. Reward: %d credits.", achievements[6].reward_credits);
	achievements[6].autoclaimable = qfalse;

	achievements[7].type = ACHIEVEMENT_DUELS;
	achievements[7].id_numeric = numericId++;
	achievements[7].identifier = "A_DUELS_ENGAGE3";
	achievements[7].name = "Saber freak";
	achievements[7].reward_credits = 100000;
	sprintf(achievements[7].description, "Play 3000 saber duels and win at least 1500 duels total. Reward: %d credits. You are allowed to perform special saber attack with any saber type under any condition with 20 seconds cooldown (/saberbarrier command)", achievements[7].reward_credits);
	achievements[7].autoclaimable = qfalse;

	achievements[8].type = ACHIEVEMENT_DUELS;
	achievements[8].id_numeric = numericId++;
	achievements[8].identifier = "A_DUELS_DRATIO1";
	achievements[8].name = "Aspiring saberist";
	achievements[8].reward_credits = 35000;
	sprintf(achievements[8].description, "Play 500 saber duels. Win at least half of duels you played. Reward: %d credits.", achievements[8].reward_credits);
	achievements[8].autoclaimable = qfalse;

	achievements[9].type = ACHIEVEMENT_DUELS;
	achievements[9].id_numeric = numericId++;
	achievements[9].identifier = "A_DUELS_DRATIO2";
	achievements[9].name = "Competitive saberist";
	achievements[9].reward_credits = 80000;
	sprintf(achievements[9].description, "Play 1250 saber duels. Have at least 1.5x more wins than defeats on duels you played. Reward: %d credits.", achievements[9].reward_credits);
	achievements[9].autoclaimable = qfalse;

	achievements[10].type = ACHIEVEMENT_DUELS;
	achievements[10].id_numeric = numericId++;
	achievements[10].identifier = "A_DUELS_DRATIO3";
	achievements[10].name = "Duel master";
	achievements[10].reward_credits = 200000;
	sprintf(achievements[10].description, "Play 2500 saber duels. Have at least 2x more wins than defeats on duels you played. Reward: %d credits.", achievements[10].reward_credits);
	achievements[10].autoclaimable = qfalse;

	achievements[11].type = ACHIEVEMENT_NONE;
	achievements[11].id_numeric = 1337;
	achievements[11].identifier = "A_NONE_RAGEDYDZIO";
	achievements[11].name = "Trolled by own system user";
	achievements[11].reward_credits = -10000;
	sprintf(achievements[11].description, "Make ^0Dyd^1zio^6 ragequit. Reward: ^1%d^6 credits.", achievements[11].reward_credits);
	achievements[11].autoclaimable = qtrue;

	achievements[12].type = ACHIEVEMENT_HELPER;
	achievements[12].id_numeric = numericId++;
	achievements[12].identifier = "A_HELPER_DONATOR";
	achievements[12].name = "Wallet warrior";
	achievements[12].reward_credits = 250000;
	sprintf(achievements[12].description, "Donate at least $5 to help the server stay. That allows you to get one-time special reward on one of your accounts. Reward: %d credits.", achievements[12].reward_credits);
	achievements[12].autoclaimable = qtrue;

	achievements[13].type = ACHIEVEMENT_FIGHT;
	achievements[13].id_numeric = numericId++;
	achievements[13].identifier = "A_FIGHT_SELFSHOT1";
	achievements[13].name = "Brainless shooter";
	achievements[13].reward_credits = 10000;
	sprintf(achievements[13].description, "Die 100 times from your own non-splash bullet. This achievement is not something to be proud of. Reward: %d credits.", achievements[13].reward_credits);
	achievements[13].autoclaimable = qtrue;

	achievements[14].type = ACHIEVEMENT_FIGHT;
	achievements[14].id_numeric = numericId++;
	achievements[14].identifier = "A_FIGHT_AMMO1";
	achievements[14].name = "Ammo consumer";
	achievements[14].reward_credits = 5000;
	sprintf(achievements[14].description, "Fire 20000 bullets. Reward: %d credits.", achievements[14].reward_credits);
	achievements[14].autoclaimable = qfalse;

	achievements[15].type = ACHIEVEMENT_FIGHT;
	achievements[15].id_numeric = numericId++;
	achievements[15].identifier = "A_FIGHT_AMMO2";
	achievements[15].name = "Metal waster";
	achievements[15].reward_credits = 15000;
	sprintf(achievements[15].description, "Fire 50000 bullets. Reward: %d credits.", achievements[15].reward_credits);
	achievements[15].autoclaimable = qfalse;

	achievements[16].type = ACHIEVEMENT_FIGHT;
	achievements[16].id_numeric = numericId++;
	achievements[16].identifier = "A_FIGHT_AMMO3";
	achievements[16].name = "Ammo, ammo everywhere";
	achievements[16].reward_credits = 40000;
	sprintf(achievements[16].description, "Fire 150000 bullets. Reward: %d credits.", achievements[16].reward_credits);
	achievements[16].autoclaimable = qfalse;

	achievements[17].type = ACHIEVEMENT_MISC;
	achievements[17].id_numeric = numericId++;
	achievements[17].identifier = "A_MISC_STASH1";
	achievements[17].name = "Stash finder";
	achievements[17].reward_credits = 5000;
	sprintf(achievements[17].description, "Find and deposit 50 stashes. Reward: %d credits.", achievements[17].reward_credits);
	achievements[17].autoclaimable = qfalse;

	achievements[18].type = ACHIEVEMENT_MISC;
	achievements[18].id_numeric = numericId++;
	achievements[18].identifier = "A_MISC_STASH2";
	achievements[18].name = "Stash lover";
	achievements[18].reward_credits = 35000;
	sprintf(achievements[18].description, "Find and deposit 200 stashes. Reward: %d credits.", achievements[18].reward_credits);
	achievements[18].autoclaimable = qfalse;

	achievements[19].type = ACHIEVEMENT_DUELS;
	achievements[19].id_numeric = numericId++;
	achievements[19].identifier = "A_DUELS_PISTOL1";
	achievements[19].name = "Pistol dueler";
	achievements[19].reward_credits = 20000;
	sprintf(achievements[19].description, "Win 250 blaster pistol duels with another player. Reward: %d credits.", achievements[19].reward_credits);
	achievements[19].autoclaimable = qtrue;

	achievements[20].type = ACHIEVEMENT_DUELS;
	achievements[20].id_numeric = numericId++;
	achievements[20].identifier = "A_DUELS_PISTOL2";
	achievements[20].name = "Precise killer";
	achievements[20].reward_credits = 45000;
	sprintf(achievements[20].description, "Win 750 blaster pistol duels against another player. Reward: %d credits.", achievements[20].reward_credits);
	achievements[20].autoclaimable = qtrue;

	achievements[21].type = ACHIEVEMENT_DUELS;
	achievements[21].id_numeric = numericId++;
	achievements[21].identifier = "A_DUELS_PISTOL3";
	achievements[21].name = "Famous shooter";
	achievements[21].reward_credits = 75000;
	sprintf(achievements[21].description, "Win 1500 blaster pistol duels against another player. Reward: %d credits.", achievements[21].reward_credits);
	achievements[21].autoclaimable = qtrue;
}