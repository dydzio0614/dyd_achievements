#ifndef __DYD_ACHIEVEMENTS__
#define __DYD_ACHIEVEMENTS__

enum dyd_achievement_types
{
	ACHIEVEMENT_FIGHT,
	ACHIEVEMENT_DUELS,
	ACHIEVEMENT_MISC
};

struct dyd_achievement
{
	enum dyd_achievement_types type; //sorting achievements by types
	char* identifier; //unique name for preserving in lugormod account data
	char* name; //name to display in console
	char* description; //description to display in console
	int reward_credits;
	qboolean autoclaimable;
};

void achievements_init();
void achievements_check(gentity_t*, dyd_achievement*);
void achievements_list(gentity_t*, enum dyd_achievement_types, qboolean);
int achievements_progress(gentity_t*, const char*, qboolean);

#endif //__DYD_ACHIEVEMENTS__

