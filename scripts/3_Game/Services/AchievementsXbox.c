
//#ifdef PLATFORM_XBOX

enum EAchievementError
{
	ERR_OK,
	ERR_NOT_FOUND, 
	ERR_BAD_DATA,
	ERR_NOT_IMPLEMENTED,
	ERR_UNKNOWN,
};

enum EAchievementTargetId
{
	TARGET_SURVIVOR,
	TARGET_INFECTED,
};

enum EAchievementRankId
{
	RANK_SURVIVOR,				//!< must be TARGET_SURVIVOR
	RANK_INFECTED,				//!< must be TARGET_INFECTED
	RANK_INFECTED_SOLDIER,		//!< must be TARGET_INFECTED
};

enum EAchievementRangeId
{
	RANGE_OTHER,
	RANGE_MELEE,
	RANGE_RANGED,
};

enum EAchievementHitId
{
	HIT_OTHER,
	HIT_HEADSHOT,
};

enum EAchievementActionId
{
	ACTION_EAT,
	ACTION_DRINK,
	ACTION_EQUIP_GEAR,
	ACTION_COOK_STEAK,
	ACTION_IGNITE_FIRE_MATCHBOX,
	ACTION_IGNITE_FIRE_ROAD_FLARE,
	ACTION_IGNITE_FIRE_HAND_DRILL,
	ACTION_SHAVE,
	ACTION_GUT_DEER,
	ACTION_APPLY_MEDS_ON_SURVIVOR,
	ACTION_HANDCUFF_SURVIVOR,
};

class AchievementsXbox
{
	proto static native EAchievementError SendEventAction(EAchievementActionId action_id);
	proto static native EAchievementError SendEventKill(EAchievementTargetId target_id, EAchievementRankId rank_id, EAchievementRangeId range_id, EAchievementHitId hit_id, float distance);
};

//#endif