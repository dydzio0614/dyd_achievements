#pragma once
#include "hookdata.h"

int Accounts_Stats_GetPlayerKills(Account_t*);
int Accounts_Stats_GetPlayerDefeats(Account_t*);
int Accounts_Stats_GetSelfshots(Account_t*);
void Accounts_Stats_SetPlayerKills(Account_t*, int);
void Accounts_Stats_SetPlayerDefeats(Account_t*, int);
void Accounts_Stats_SetSelfshots(Account_t*, int);
