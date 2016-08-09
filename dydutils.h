#pragma once
#include "hookdata.h"

int Accounts_Stats_GetPlayerKills(Account_t*);
int Accounts_Stats_GetPlayerDefeats(Account_t*);
int Accounts_Stats_GetSelfshots(Account_t*);
int Accounts_Stats_GetPistolDuels(Account_t*);
int Accounts_Stats_GetPistolDuelWins(Account_t*);
void Accounts_Stats_SetPlayerKills(Account_t*, int);
void Accounts_Stats_SetPlayerDefeats(Account_t*, int);
void Accounts_Stats_SetSelfshots(Account_t*, int);
void Accounts_Stats_SetPistolDuels(Account_t*, int);
void Accounts_Stats_SetPistolDuelWins(Account_t*, int);

//not affiliated with new stats, used as achievements completion variable
int Accounts_Stats_GetHoursPlayed(Account_t*);
