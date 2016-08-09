#include "achievements.h"

struct dyd_achievement achievements[MAX_ACHIEVEMENTS];

level_locals_t* g_level = (level_locals_t*)LEVEL;

struct dyd_playerdata dyd_data;
int numericId = 1;

dyd_achievement* FindAchievementById(int value)
{
	for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
	{
		if (achievements[i].id == value)
			return &achievements[i];
	}
	return NULL;
}

long GetAchievementBitmaskFromID(long ID)
{
	if (ID == 1337) ID = 12;
	return 1 << (ID - 1);
}

int achievements_progress(gentity_t *user, dyd_achievement_identifiers x, qboolean print, qboolean delayprint) //check progress, can be used in unlock to check requirements, can be used to display progress, use delayprint to reduce output frequency by holding data in buffer waiting for dispcontiguous to be called externally
{
	dyd_achievement *achievementToCheck = FindAchievementById(x);

	if (achievementToCheck->progressVariable.varName != NULL) //code logic for achievements with 1 completion variable
	{
		int progressVariable = achievementToCheck->progressVariable.GetStatFunction(user->client->pers.Lmd.account);
		int conditionsMet = (progressVariable >= achievementToCheck->progressVariable.counter);

		if (print == qtrue)
		{
			char *msg;
			if(conditionsMet)
				msg = JASS_VARARGS("^2Your current progress of the achievement: %d/%d %s - you finished the goal",
					progressVariable, achievementToCheck->progressVariable.counter, achievementToCheck->progressVariable.varName);
			else
				msg = JASS_VARARGS("^3Your current progress of the achievement: %d/%d %s",
					progressVariable, achievementToCheck->progressVariable.counter, achievementToCheck->progressVariable.varName);

			if (delayprint == qtrue)
				DispContiguous(user, msg);
			else
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"%s\n\"", msg));
		}
		return conditionsMet;
	}

	else if (x == A_DUELS_ENGAGE3)
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		int conditionsMet = (duels >= 3000 && wins >= 1500);

		if (print == qtrue)
		{
			char *msg;
			if(conditionsMet)
				msg = JASS_VARARGS("^2Your current progress of the achievement: %d/3000 duels, %d/1500 duels won - you finished the goal", duels, wins);
			else
				msg = JASS_VARARGS("^3Your current progress of the achievement: %d/3000 duels, %d/1500 duels won", duels, wins);

			if (delayprint == qtrue)
				DispContiguous(user, msg);
			else
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"%s\n\"", msg));
		}
		return conditionsMet;
	}

	else if (x == A_DUELS_ENGAGE4)
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		int conditionsMet = (duels >= 7500 && wins >= 5000);

		if (print == qtrue)
		{
			char *msg;
			if (conditionsMet)
				msg = JASS_VARARGS("^2Your current progress of the achievement: %d/7500 duels, %d/5000 duels won - you finished the goal", duels, wins);
			else
				msg = JASS_VARARGS("^3Your current progress of the achievement: %d/7500 duels, %d/5000 duels won", duels, wins);

			if (delayprint == qtrue)
				DispContiguous(user, msg);
			else
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"%s\n\"", msg));
		}
		return conditionsMet;
	}

	else if (x == A_DUELS_DRATIO1)
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = 0;

		if ( (duels - wins) != 0) 
			ratio = (float)wins / (float)(duels - wins);

		int conditionsMet = (duels >= 500 && ratio >= 1.0f);

		if (print == qtrue)
		{
			char buf[256];
			char *msg;
			if (conditionsMet)
			{
				sprintf(buf, "Your current progress of the achievement: %d/500 duels, %.2f duel ratio (1.0 needed) - you finished the goal", duels, (float)ratio);
				msg = JASS_VARARGS("^2%s", buf);
			}
			else
			{
				sprintf(buf, "Your current progress of the achievement: %d/500 duels, %.2f duel ratio (1.0 needed)", duels, (float)ratio);
				msg = JASS_VARARGS("^3%s", buf);
			}

			if (delayprint == qtrue)
				DispContiguous(user, msg);
			else
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"%s\n\"", msg));
		}
		return conditionsMet;
	}

	else if (x == A_DUELS_DRATIO2)
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = 0;

		if ((duels - wins) != 0)
			ratio = (float)wins / (float)(duels - wins);

		int conditionsMet = (duels >= 1250 && ratio >= 1.5f);

		if (print == qtrue)
		{
			char buf[256];
			char *msg;
			if (conditionsMet)
			{
				sprintf(buf, "Your current progress of the achievement: %d/1250 duels, %.2f duel ratio (1.5 needed) - you finished the goal", duels, (float)ratio);
				msg = JASS_VARARGS("^2%s", buf);
			}
			else
			{
				sprintf(buf, "Your current progress of the achievement: %d/1250 duels, %.2f duel ratio (1.5 needed)", duels, (float)ratio);
				msg = JASS_VARARGS("^3%s", buf);
			}

			if (delayprint == qtrue)
				DispContiguous(user, msg);
			else
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"%s\n\"", msg));
		}
		return conditionsMet;
	}

	else if (x == A_DUELS_DRATIO3)
	{
		int duels = Accounts_Stats_GetDuels(user->client->pers.Lmd.account);
		int wins = Accounts_Stats_GetDuelsWon(user->client->pers.Lmd.account);
		float ratio = 0;

		if ((duels - wins) != 0)
			ratio = (float)wins / (float)(duels - wins);

		int conditionsMet = (duels >= 2500 && ratio >= 2.0f);

		if (print == qtrue)
		{
			char buf[256];
			char *msg;
			if (conditionsMet)
			{
				sprintf(buf, "Your current progress of the achievement: %d/2500 duels, %.2f duel ratio (2.0 needed) - you finished the goal", duels, (float)ratio);
				msg = JASS_VARARGS("^2%s", buf);
			}
			else
			{
				sprintf(buf, "Your current progress of the achievement: %d/2500 duels, %.2f duel ratio (2.0 needed)", duels, (float)ratio);
				msg = JASS_VARARGS("^3%s", buf);
			}

			if (delayprint == qtrue)
				DispContiguous(user, msg);
			else
				g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"%s\n\"", msg));
		}
		return conditionsMet;
	}

	else return -1; //achievement not found by identifier
}

int achievements_progress(gentity_t *user, dyd_achievement_identifiers x, qboolean print)
{
	return achievements_progress(user, x, print, qfalse);
}


void achievements_check(gentity_t *user, dyd_achievement *x, qboolean print) //achievement unlock logic
{
	int state = achievements_progress(user, x->id, print);

	if (state != -1)
	{
		char* rawValue = Accounts_Custom_GetValue(user->client->pers.Lmd.account, "ACHIEVEMENTS");
		unsigned long value = (rawValue != NULL) ? strtol(rawValue, 0, NULL) : 0;
		if ((value & GetAchievementBitmaskFromID(x->id)) == 0)
		{
			if (state == 1)
			{
				if (user->client->pers.Lmd.account->time >= 72000)
				{
					Accounts_Custom_SetValue(user->client->pers.Lmd.account, "ACHIEVEMENTS", JASS_VARARGS("%lu", value | GetAchievementBitmaskFromID(x->id)));

					user->client->pers.Lmd.account->credits += x->reward_credits; //extend if new types of rewards added
					user->client->pers.Lmd.account->modifiedTime = g_level->time; //ALWAYS modify this field like that if doing direct changes to account fields

					if (print == qtrue)
					{
						g_syscall(G_SEND_SERVER_COMMAND, user->s.number, "print \"^2Achievement unlocked successfully!\n\"");
						g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2You received %d credits!\n\"", x->reward_credits));
					}
					g_syscall(G_SEND_SERVER_COMMAND, -1, JASS_VARARGS("print \"^7Player %s^7 has completed achievement: %s\n\"", user->client->pers.netname, x->name));
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



void achievements_list(gentity_t *user, enum dyd_achievement_types type, qboolean extended, qboolean claimableonly) //this does NOT handle "claimable" category
{
	for (int i = 0; i < MAX_ACHIEVEMENTS; i++)
	{
		if (( type == 0 && achievements[i].type != ACHIEVEMENT_NONE) || achievements[i].type == type)
		{
			if (!claimableonly || (claimableonly && achievements[i].autoclaimable == qfalse))
			{
				char* bitmaskValue = Accounts_Custom_GetValue(user->client->pers.Lmd.account, "ACHIEVEMENTS");
				if (bitmaskValue != NULL && ((unsigned long)strtol(bitmaskValue, NULL, 0) & GetAchievementBitmaskFromID(achievements[i].id))) //checking for completion
				{
					DispContiguous(user, JASS_VARARGS("^2%d. %s ^5%s", achievements[i].id, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)^2 - COMPLETED" : "^2 - COMPLETED"));
					//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^3%d. %s ^5%s\"", achievements[i].id, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)\n" : "\n"));
				}
				else
				{
					DispContiguous(user, JASS_VARARGS("^3%d. %s ^5%s", achievements[i].id, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)" : ""));
					//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^2%d. %s ^5%s\"", achievements[i].id, achievements[i].name, (achievements[i].autoclaimable == qfalse) ? "(claimable)^2 - COMPLETED\n" : "^2 - COMPLETED \n"));
				}

				if (extended == qtrue)
				{
					DispContiguous(user, JASS_VARARGS("^6Description: %s", achievements[i].description));
					//g_syscall(G_SEND_SERVER_COMMAND, user->s.number, JASS_VARARGS("print \"^6Description: %s\n\"", achievements[i].description));
					achievements_progress(user, achievements[i].id, qtrue, qtrue);
				}
			}
		}
	}
	DispContiguous(user, NULL);
}


void achievements_init() //server start achievement allocation, change achievements_progress() too for not autoclaimable achievements if adding new
{
	/*TEMPLATE
	achievements[x].type = ACHIEVEMENT_TYPE enum;
	achievements[x].id = numericId++;
	achievements[x].name = "NAME";
	achievements[x].reward_credits = NUMBER;
	sprintf(achievements[x].description, "DESCRIPTION, reward: %d, achievements[x].reward_credits); //change sprintf to JASS_VARARGS ?
	achievements[x].autoclaimable = qfalse / qtrue;
	*/

	achievements[0].type = ACHIEVEMENT_MISC;
	achievements[0].id = A_MISC_PLAYTIME1;
	achievements[0].name = "Loyal player";
	achievements[0].reward_credits = 20000;
	sprintf(achievements[0].description, "Spend 25 hours total on the server. Reward: %d credits", achievements[0].reward_credits);
	achievements[0].autoclaimable = qfalse;
	achievements[0].progressVariable.varName = "hours"; achievements[0].progressVariable.counter = 25; achievements[0].progressVariable.GetStatFunction = Accounts_Stats_GetHoursPlayed;

	achievements[1].type = ACHIEVEMENT_MISC;
	achievements[1].id = A_MISC_PLAYTIME2;
	achievements[1].name = "Follower";
	achievements[1].reward_credits = 90000;
	sprintf(achievements[1].description, "Spend 100 hours total on the server. Reward: %d credits", achievements[1].reward_credits);
	achievements[1].autoclaimable = qfalse;
	achievements[1].progressVariable.varName = "hours"; achievements[1].progressVariable.counter = 100; achievements[1].progressVariable.GetStatFunction = Accounts_Stats_GetHoursPlayed;

	achievements[2].type = ACHIEVEMENT_FIGHT;
	achievements[2].id = A_FIGHT_PKILL1;
	achievements[2].name = "Soldier";
	achievements[2].reward_credits = 25000;
	sprintf(achievements[2].description, "Kill 1000 players. Reward: %d credits", achievements[2].reward_credits);
	achievements[2].autoclaimable = qtrue;
	achievements[2].progressVariable.varName = "kills"; achievements[2].progressVariable.counter = 1000; achievements[2].progressVariable.GetStatFunction = Accounts_Stats_GetPlayerKills;

	achievements[3].type = ACHIEVEMENT_FIGHT;
	achievements[3].id = A_FIGHT_PKILL2;
	achievements[3].name = "Battlemaster";
	achievements[3].reward_credits = 100000;
	sprintf(achievements[3].description, "Kill 5000 players. Reward: %d credits", achievements[3].reward_credits);
	achievements[3].autoclaimable = qtrue;
	achievements[3].progressVariable.varName = "kills"; achievements[3].progressVariable.counter = 5000; achievements[3].progressVariable.GetStatFunction = Accounts_Stats_GetPlayerKills;

	achievements[4].type = ACHIEVEMENT_FIGHT;
	achievements[4].id = A_FIGHT_PKILL3;
	achievements[4].name = "Conqueror";
	achievements[4].reward_credits = 160000;
	sprintf(achievements[4].description, "Kill 10000 players. Reward: (hidden)");
	achievements[4].autoclaimable = qtrue;
	achievements[4].progressVariable.varName = "kills"; achievements[4].progressVariable.counter = 10000; achievements[4].progressVariable.GetStatFunction = Accounts_Stats_GetPlayerKills;

	achievements[5].type = ACHIEVEMENT_DUELS;
	achievements[5].id = A_DUELS_ENGAGE1;
	achievements[5].name = "Saberfighter";
	achievements[5].reward_credits = 25000;
	sprintf(achievements[5].description, "Play 500 saber duels. Reward: %d credits.", achievements[5].reward_credits);
	achievements[5].autoclaimable = qfalse;
	achievements[5].progressVariable.varName = "duels"; achievements[5].progressVariable.counter = 500; achievements[5].progressVariable.GetStatFunction = Accounts_Stats_GetDuels;

	achievements[6].type = ACHIEVEMENT_DUELS;
	achievements[6].id = A_DUELS_ENGAGE2;
	achievements[6].name = "Saber fan";
	achievements[6].reward_credits = 60000;
	sprintf(achievements[6].description, "Play 1500 saber duels. Reward: %d credits.", achievements[6].reward_credits);
	achievements[6].autoclaimable = qfalse;
	achievements[6].progressVariable.varName = "duels"; achievements[6].progressVariable.counter = 1500; achievements[6].progressVariable.GetStatFunction = Accounts_Stats_GetDuels;

	achievements[7].type = ACHIEVEMENT_DUELS;
	achievements[7].id = A_DUELS_ENGAGE3;
	achievements[7].name = "Saber freak";
	achievements[7].reward_credits = 100000;
	sprintf(achievements[7].description, "Play 3000 saber duels and win at least 1500 duels total. Reward: %d credits. You are allowed to perform special saber attack with any saber type under any condition with 20 seconds cooldown (/saberbarrier command)", achievements[7].reward_credits);
	achievements[7].autoclaimable = qfalse;

	achievements[8].type = ACHIEVEMENT_DUELS;
	achievements[8].id = A_DUELS_DRATIO1;
	achievements[8].name = "Aspiring saberist";
	achievements[8].reward_credits = 35000;
	sprintf(achievements[8].description, "Play 500 saber duels. Win at least half of duels you played. Reward: %d credits.", achievements[8].reward_credits);
	achievements[8].autoclaimable = qfalse;

	achievements[9].type = ACHIEVEMENT_DUELS;
	achievements[9].id = A_DUELS_DRATIO2;
	achievements[9].name = "Competitive saberist";
	achievements[9].reward_credits = 80000;
	sprintf(achievements[9].description, "Play 1250 saber duels. Have at least 1.5x more wins than defeats on duels you played. Reward: %d credits.", achievements[9].reward_credits);
	achievements[9].autoclaimable = qfalse;

	achievements[10].type = ACHIEVEMENT_DUELS;
	achievements[10].id = A_DUELS_DRATIO3;
	achievements[10].name = "Duel master";
	achievements[10].reward_credits = 200000;
	sprintf(achievements[10].description, "Play 2500 saber duels. Have at least 2x more wins than defeats on duels you played. Reward: %d credits.", achievements[10].reward_credits);
	achievements[10].autoclaimable = qfalse;

	achievements[11].type = ACHIEVEMENT_NONE;
	achievements[11].id = A_NONE_RAGEDYDZIO;
	achievements[11].name = "Trolled by own system user";
	achievements[11].reward_credits = -10000;
	sprintf(achievements[11].description, "Make ^0Dyd^1zio^6 ragequit. Reward: ^1%d^6 credits.", achievements[11].reward_credits);
	achievements[11].autoclaimable = qtrue;

	achievements[12].type = ACHIEVEMENT_HELPER;
	achievements[12].id = A_HELPER_DONATOR;
	achievements[12].name = "Wallet warrior";
	achievements[12].reward_credits = 250000;
	sprintf(achievements[12].description, "Donate at least $5 to help the server stay. That allows you to get one-time special reward on one of your accounts. Reward: %d credits.", achievements[12].reward_credits);
	achievements[12].autoclaimable = qtrue;

	achievements[13].type = ACHIEVEMENT_FIGHT;
	achievements[13].id = A_FIGHT_SELFSHOT1;
	achievements[13].name = "Brainless shooter";
	achievements[13].reward_credits = 25000;
	sprintf(achievements[13].description, "Die 100 times from your own non-splash bullet. This achievement is not something to be proud of. Reward: %d credits.", achievements[13].reward_credits);
	achievements[13].autoclaimable = qtrue;
	achievements[13].progressVariable.varName = "suicidal shots"; achievements[13].progressVariable.counter = 100; achievements[13].progressVariable.GetStatFunction = Accounts_Stats_GetSelfshots;

	achievements[14].type = ACHIEVEMENT_FIGHT;
	achievements[14].id = A_FIGHT_AMMO1;
	achievements[14].name = "Ammo consumer";
	achievements[14].reward_credits = 5000;
	sprintf(achievements[14].description, "Fire 20000 bullets. Reward: %d credits.", achievements[14].reward_credits);
	achievements[14].autoclaimable = qfalse;
	achievements[14].progressVariable.varName = "ammo fired"; achievements[14].progressVariable.counter = 20000; achievements[14].progressVariable.GetStatFunction = Accounts_Stats_GetShots;

	achievements[15].type = ACHIEVEMENT_FIGHT;
	achievements[15].id = A_FIGHT_AMMO2;
	achievements[15].name = "Metal waster";
	achievements[15].reward_credits = 15000;
	sprintf(achievements[15].description, "Fire 50000 bullets. Reward: %d credits.", achievements[15].reward_credits);
	achievements[15].autoclaimable = qfalse;
	achievements[15].progressVariable.varName = "ammo fired"; achievements[15].progressVariable.counter = 50000; achievements[15].progressVariable.GetStatFunction = Accounts_Stats_GetShots;

	achievements[16].type = ACHIEVEMENT_FIGHT;
	achievements[16].id = A_FIGHT_AMMO3;
	achievements[16].name = "Ammo, ammo everywhere";
	achievements[16].reward_credits = 40000;
	sprintf(achievements[16].description, "Fire 150000 bullets. Reward: %d credits.", achievements[16].reward_credits);
	achievements[16].autoclaimable = qfalse;
	achievements[16].progressVariable.varName = "ammo fired"; achievements[16].progressVariable.counter = 150000; achievements[16].progressVariable.GetStatFunction = Accounts_Stats_GetShots;

	achievements[17].type = ACHIEVEMENT_MISC;
	achievements[17].id = A_MISC_STASH1;
	achievements[17].name = "Stash finder";
	achievements[17].reward_credits = 5000;
	sprintf(achievements[17].description, "Find and deposit 50 stashes. Reward: %d credits.", achievements[17].reward_credits);
	achievements[17].autoclaimable = qfalse;
	achievements[17].progressVariable.varName = "stashes"; achievements[17].progressVariable.counter = 50; achievements[17].progressVariable.GetStatFunction = Accounts_Stats_GetStashes;

	achievements[18].type = ACHIEVEMENT_MISC;
	achievements[18].id = A_MISC_STASH2;
	achievements[18].name = "Stash lover";
	achievements[18].reward_credits = 35000;
	sprintf(achievements[18].description, "Find and deposit 200 stashes. Reward: %d credits.", achievements[18].reward_credits);
	achievements[18].autoclaimable = qfalse;
	achievements[18].progressVariable.varName = "stashes"; achievements[18].progressVariable.counter = 200; achievements[18].progressVariable.GetStatFunction = Accounts_Stats_GetStashes;

	achievements[19].type = ACHIEVEMENT_DUELS;
	achievements[19].id = A_DUELS_PISTOL1;
	achievements[19].name = "Pistol dueler";
	achievements[19].reward_credits = 30000;
	sprintf(achievements[19].description, "Win 250 blaster pistol duels against another player. Reward: %d credits.", achievements[19].reward_credits);
	achievements[19].autoclaimable = qtrue;
	achievements[19].progressVariable.varName = "pistol duels"; achievements[19].progressVariable.counter = 250; achievements[19].progressVariable.GetStatFunction = Accounts_Stats_GetPistolDuelWins;

	achievements[20].type = ACHIEVEMENT_DUELS;
	achievements[20].id = A_DUELS_PISTOL2;
	achievements[20].name = "Precise killer";
	achievements[20].reward_credits = 75000;
	sprintf(achievements[20].description, "Win 750 blaster pistol duels against another player. Reward: %d credits.", achievements[20].reward_credits);
	achievements[20].autoclaimable = qtrue;
	achievements[20].progressVariable.varName = "pistol duels"; achievements[20].progressVariable.counter = 750; achievements[20].progressVariable.GetStatFunction = Accounts_Stats_GetPistolDuelWins;

	achievements[21].type = ACHIEVEMENT_DUELS;
	achievements[21].id = A_DUELS_PISTOL3;
	achievements[21].name = "Famous shooter";
	achievements[21].reward_credits = 115000;
	sprintf(achievements[21].description, "Win 1500 blaster pistol duels against another player. Reward: %d credits.", achievements[21].reward_credits);
	achievements[21].autoclaimable = qtrue;
	achievements[21].progressVariable.varName = "pistol duels"; achievements[21].progressVariable.counter = 1500; achievements[21].progressVariable.GetStatFunction = Accounts_Stats_GetPistolDuelWins;

	achievements[22].type = ACHIEVEMENT_LEGENDARY;
	achievements[22].id = A_MISC_PLAYTIME3;
	achievements[22].name = "Loyal to death";
	achievements[22].reward_credits = 150000;
	sprintf(achievements[22].description, "Spend 600 hours total on the server. Reward: %d credits. You unlock /mentalshield command that protects player from taking damage for 3 seconds.", achievements[22].reward_credits);
	achievements[22].autoclaimable = qfalse;
	achievements[22].progressVariable.varName = "hours"; achievements[22].progressVariable.counter = 600; achievements[22].progressVariable.GetStatFunction = Accounts_Stats_GetHoursPlayed;

	achievements[23].type = ACHIEVEMENT_LEGENDARY;
	achievements[23].id = A_DUELS_ENGAGE4;
	achievements[23].name = "Saber god";
	achievements[23].reward_credits = 160000;
	sprintf(achievements[23].description, "Play 7500 saber duels and win at least 5000 duels total. Reward: %d credits. You are allowed to perform special attack with any saber type while standing still with 60 seconds cooldown (/bladetornado command)", achievements[23].reward_credits);
	achievements[23].autoclaimable = qfalse;

}