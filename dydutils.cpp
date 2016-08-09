#include "dydutils.h"

int Accounts_Stats_GetPlayerKills(Account_t *acc)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_PLAYERKILLS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

int Accounts_Stats_GetPlayerDefeats(Account_t *acc)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_PLAYERDEATHS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

int Accounts_Stats_GetSelfshots(Account_t *acc)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_SELFSHOTS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

int Accounts_Stats_GetPistolDuels(Account_t *acc)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_PDUELS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

int Accounts_Stats_GetPistolDuelWins(Account_t *acc)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%s", Accounts_Custom_GetValue(acc, "A_DATA_PWINS"));
		return strtol(data, NULL, 0);
	}
	else return 0;
}

void Accounts_Stats_SetPlayerKills(Account_t *acc, int value)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%d", value);
		Accounts_Custom_SetValue(acc, "A_DATA_PLAYERKILLS", data);
	}
}

void Accounts_Stats_SetPlayerDefeats(Account_t *acc, int value)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%d", value);
		Accounts_Custom_SetValue(acc, "A_DATA_PLAYERDEATHS", data);
	}
}

void Accounts_Stats_SetSelfshots(Account_t *acc, int value)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%d", value);
		Accounts_Custom_SetValue(acc, "A_DATA_SELFSHOTS", data);
	}
}

void Accounts_Stats_SetPistolDuels(Account_t *acc, int value)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%d", value);
		Accounts_Custom_SetValue(acc, "A_DATA_PDUELS", data);
	}
}

void Accounts_Stats_SetPistolDuelWins(Account_t *acc, int value)
{
	if (acc)
	{
		char data[32];
		sprintf(data, "%d", value);
		Accounts_Custom_SetValue(acc, "A_DATA_PWINS", data);
	}
}

int Accounts_Stats_GetHoursPlayed(Account_t *acc)
{
	if (acc)
	{
		return (acc->time / 3600);
	}
	else return 0;
}