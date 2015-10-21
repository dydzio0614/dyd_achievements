#pragma once
#include "dydutils.h"

enum dyd_achievement_types
{
	ACHIEVEMENT_FIGHT,
	ACHIEVEMENT_DUELS,
	ACHIEVEMENT_MISC
};

struct dyd_achievement
{
	enum dyd_achievement_types type; //sorting achievements by types
	int id_numeric; //main achievement ID
	char* identifier; //unique name for preserving in lugormod account data
	char* name; //name to display
	char* description; //description to display in console
	int reward_credits;
	qboolean autoclaimable;
};

#define ACHIEV_COUNT 11 //increase when adding new achievements

struct dyd_achievement achievements[ACHIEV_COUNT]; //TODO: alloc achievements on stack
int numericId = 1;

//utility functions
dyd_achievement* FindAchievementById(int value)
{
	for (int i = 0; i < ACHIEV_COUNT; i++)
	{
		if (achievements[i].id_numeric == value)
			return &achievements[i];
	}
	return NULL;
}

dyd_achievement* FindAchievementByTextIdentifier(const char *text)
{
	for (int i = 0; i < ACHIEV_COUNT; i++)
	{
		if (!stricmp(text, achievements[i].identifier))
			return &achievements[i];
	}
	return NULL;
}


//control flow functions
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
		if (kills >= 5000)
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/5000 kills - you finished the goal\n\"", kills));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/5000 kills\n\"", kills));
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
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/10000 kills - you finished the goal\n\"", kills));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/10000 kills\n\"", kills));
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
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/500 duels - you finished the goal\n\"", duels));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/500 duels\n\"", duels));
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
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/1500 duels - you finished the goal\n\"", duels));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/1500 duels\n\"", duels));
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
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2Your current progress of the achievement: %d/3000 duels, %d/1500 duels won - you finished the goal\n\"", duels, wins));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3Your current progress of the achievement: %d/3000 duels, %d/1500 duels won\n\"", duels, wins));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_DRATIO1"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = (float)wins / (float)(duels - wins);

		if (duels >= 500 && ratio >= 1.0f)
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/500 duels, %.2f duel ratio (1.0 needed) - you finished the goal\n", duels, (float)ratio);
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%s\"", buf));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/500 duels, %.2f duel ratio (1.0 needed)\n", duels, (float)ratio);
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%s\"", buf));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_DRATIO2"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = (float)wins / (float)(duels - wins);

		if (duels >= 1250 && ratio >= 1.5f)
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/1250 duels, %.2f duel ratio (1.5 needed) - you finished the goal\n", duels, (float)ratio);
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%s\"", buf));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/1250 duels, %.2f duel ratio (1.5 needed)\n", duels, (float)ratio);
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%s\"", buf));
			}
			return 0;
		}
	}

	else if (!stricmp(x, "A_DUELS_DRATIO3"))
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = (float)wins / (float)(duels - wins);

		if (duels >= 2500 && ratio >= 2.0f)
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/2500 duels, %.2f duel ratio (2.0 needed) - you finished the goal\n", duels, (float)ratio);
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%s\"", buf));
			}
			return 1;
		}
		else
		{
			if (print == qtrue)
			{
				char buf[256];
				sprintf(buf, "Your current progress of the achievement: %d/2500 duels, %.2f duel ratio (2.0 needed)\n", duels, (float)ratio);
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%s\"", buf));
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
				user->client->pers.Lmd.account->modifiedTime = g_level->time; //ALWAYS modify this field like that if doing direct changes to account fields

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
	for (int i = 0; i < ACHIEV_COUNT; i++)
	{
		if (achievements[i].type == type)
		{
			if (Accounts_Custom_GetValue(user->client->pers.Lmd.account, achievements[i].identifier) == NULL) //checking for completion
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%d. %s ^5%s\"", achievements[i].id_numeric, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)\n" : "\n"));
			}
			else
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%d. %s ^5%s\"", achievements[i].id_numeric, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)^2 - COMPLETED\n" : "^2 - COMPLETED \n"));
			}

			if (extended == qtrue)
			{
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^6Description: %s\n\"", achievements[i].description));
				achievements_progress(user, achievements[i].identifier, qtrue);
			}
		}
	}
}


void achievements_init() //server start achievement allocation, change achievements_progress() too for not autoclaimable achievements if adding new
{
	/*TEMPLATE
	achievements[x].type = ACHIEVEMENT_TYPE enum;
	achievements[x].id_numeric = numericId++;
	achievements[x].identifier = "A_TYPE_NAME";
	achievements[x].name = "NAME";
	achievements[x].reward_credits = NUMBER;
	sprintf(achievements[x].description, "DESCRIPTION, reward: %d, achievements[x]->reward_credits);
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
	sprintf(achievements[4].description, "Kill 10000 players. Reward: %d credits", achievements[4].reward_credits);
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
	sprintf(achievements[7].description, "Play 3000 saber duels and win at least 1500 duels total. Reward: %d credits. You are allowed to perform dual saber kata hit under any condition with 20 seconds cooldown", achievements[7].reward_credits);
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
	achievements[10].reward_credits = 250000;
	sprintf(achievements[10].description, "Play 2500 saber duels. Have at least 2x more wins than defeats on duels you played. Reward: %d credits.", achievements[10].reward_credits);
	achievements[10].autoclaimable = qfalse;
}



