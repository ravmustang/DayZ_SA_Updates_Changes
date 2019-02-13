enum eDisplayElements
{
	DELM_BADGE_STUFFED,
	DELM_BADGE_WET,
	DELM_BADGE_SICK,
	DELM_BADGE_POISONED,
	DELM_BADGE_FRACTURE,
	DELM_BADGE_BLEEDING,
	DELM_BADGE_PILLS,
	//------------------
	DELM_TDCY_HEALTH,
	DELM_TDCY_BLOOD,
	DELM_TDCY_TEMPERATURE,
	DELM_TDCY_ENERGY,
	DELM_TDCY_WATER,
	DELM_TDCY_BACTERIA,
	//------------------
	DELM_STANCE,
	COUNT
}

//----------------------------------------------

// levels - notifiers
const int	DELM_LVL_0	 			= 0;
const int	DELM_LVL_1	 			= 1;
const int	DELM_LVL_2	 			= 2;
const int	DELM_LVL_3	 			= 3;
const int	DELM_LVL_4	 			= 4;
const int	DELM_LVL_5	 			= 5;
const int	DELM_LVL_6	 			= 6;

// modes - badges
const int	DELM_LVL_BLINK 			= 1;
const int	DELM_LVL_NOBLINK 		= 2;
//----------------------------------------------


//internal stuff bellow
// types
const int	TYPE_BADGE	 		= 1;
const int	TYPE_TDCY	 		= 2;
const int	TYPE_NTFR	 		= 3;
const int	TYPE_BAR	 		= 4;
const int	TYPE_STANCE	 	= 5;
const int	TYPE_PROGRESS_BAR	 	= 6;
const int	TYPE_SINGLE_USE_ACTION_NAME = 7;
const int	TYPE_CONTINUOUS_ACTION_NAME = 8;
const int TYPE_SHOW_SINGLE_USE_ACTION = 9;
const int TYPE_SHOW_CONTINUOUS_ACTION = 10;

//constants related to ui
//key constants
const int	NTFKEY_THIRSTY	= 1;
const int	NTFKEY_HUNGRY	= 2;
const int	NTFKEY_WARMTH	= 3;
const int	NTFKEY_WETNESS	= 4;
const int	NTFKEY_FRACTURE	= 5;
const int	NTFKEY_HEALTHY	= 6;
const int	NTFKEY_FEVERISH	= 7;
const int	NTFKEY_SICK		= 8;
const int	NTFKEY_STUFFED	= 9;
const int	NTFKEY_BLEEDISH	= 10;
const int	NTFKEY_BLOOD	= 11;
const int	NTFKEY_LIVES	= 12;
const int	NTFKEY_BACTERIA	= 13;
const int	NTFKEY_POISONED	= 14;
const int	NTFKEY_PILLS	= 15;

//priority constants
const int	NTFP_THIRSTY 	= 1;
const int	NTFP_HUNGRY 	= 2;
const int	NTFP_WARMTH 	= 3;
const int	NTFP_WETNESS 	= 4;
const int	NTFP_FRACTURE 	= 5;
const int	NTFP_HEALTHY 	= 6;
const int	NTFP_FEVERISH 	= 7;
const int	NTFP_SICK	 	= 8;
const int	NTFP_STUFFED	= 9;
const int	NTFP_BLEEDISH 	= 10;

//tendency 
const int	TENDENCY_STABLE 	= 0;
const int	TENDENCY_INC_LOW	= 1;
const int	TENDENCY_INC_MED	= 2;
const int	TENDENCY_INC_HIGH	= 3;
const int	TENDENCY_DEC_LOW 	= 4;
const int	TENDENCY_DEC_MED 	= 5;
const int	TENDENCY_DEC_HIGH	= 6;

// misc
const int STAMINA_UI_RANGE		= 100;

