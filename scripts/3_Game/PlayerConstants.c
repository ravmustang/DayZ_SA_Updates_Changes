class PlayerConstants 
{
	static const float NORMAL_TEMPERATURE_L = 35.2;
	static const float NORMAL_TEMPERATURE_H = 36.8;
	static const float HIGH_TEMPERATURE_L = 38.5;
	static const float HIGH_TEMPERATURE_H = 39.9;
	//-------------------------------------------------------
	static const float STOMACH_ENERGY_TRANSFERED_PER_SEC 		= 3;	//amount of kcal transfered to energy per second(this should ideally be higher than what player burns under high metabolic load[sprint])
	static const float STOMACH_WATER_TRANSFERED_PER_SEC 		= 6;	//amount of ml transfered to water per second(this should ideally be higher than what player burns under high metabolic load[sprint])
	static const float STOMACH_SOLID_EMPTIED_PER_SEC 			= 7;	//amount of g/ml emptied from stomach per second
	//static const float STOMACH_EMPTIED_LIQUID_PER_SEC	 		= Playerstatic constants.WATER_TRANSFERED_PER_SEC;
	static const float LOW_WATER_THRESHOLD 						= 250;	//threshold from which water affects health
	static const float WATER_DECREMENT_PER_SEC 				= STOMACH_WATER_TRANSFERED_PER_SEC / 4; //used in poisoning for now
	static const float LOW_ENERGY_THRESHOLD 					= 200;	//threshold from which energy affects health
	//static const float ENERGY_DECREMENT_PER_SEC 				= 0.035; //not used?
	//static const float DAMAGE_PER_SEC 							= 1; //not used? (was how much HP is lowered while low on energy/water)
	//static const float DAMAGE_BLOOD_PER_SEC 					= 10; //not used?
	//--------------------------------------------------------
	static const float METABOLIC_SPEED_ENERGY_BASAL	= 0.04;		//energy loss per second while idle
	
	static const float METABOLIC_SPEED_ENERGY_WALK		= 0.1; 		//energy loss per second
	static const float METABOLIC_SPEED_ENERGY_JOG		= 0.32; 	//energy loss per second
	static const float METABOLIC_SPEED_ENERGY_SPRINT	= 0.54; 	//energy loss per second
	
	static const float METABOLIC_SPEED_WATER_BASAL		= 0.070; 	//water loss per second while idle
	
	static const float METABOLIC_SPEED_WATER_WALK		= 0.22; 	//water loss per second
	static const float METABOLIC_SPEED_WATER_JOG		= 0.66; 	//water loss per second
	static const float METABOLIC_SPEED_WATER_SPRINT	= 1.10; 	//water loss per second
	//--------------------------------------------------------	
	static const float	THRESHOLD_ENERGY_WARNING		= 400;		//yellow tendency icon
	static const float	THRESHOLD_ENERGY_CRITICAL		= 200;		//red tendency icon
	static const float	THRESHOLD_ENERGY_EMPTY			= 0;		//red blinking tendency icon
	
	static const float	THRESHOLD_WATER_WARNING			= 500;		//yellow tendency icon
	static const float	THRESHOLD_WATER_CRITICAL		= 250;		//red tendency icon
	static const float	THRESHOLD_WATER_EMPTY			= 0;		//red blinking tendency icon
	
	static const float	THRESHOLD_BLOOD_WARNING			= 4000;		//yellow tendency icon
	static const float	THRESHOLD_BLOOD_CRITICAL		= 3000;		//red tendency icon
	static const float	THRESHOLD_BLOOD_EMPTY			= 2750;		//red blinking tendency icon
	
	static const float	THRESHOLD_HEAT_COMFORT_PLUS_WARNING		= 0.2;	//missing comment
	static const float	THRESHOLD_HEAT_COMFORT_PLUS_CRITICAL 	= 0.5;	//missing comment
	static const float	THRESHOLD_HEAT_COMFORT_PLUS_EMPTY		= 0.9;	//missing comment

	static const float	THRESHOLD_HEAT_COMFORT_MINUS_WARNING	= -0.3;	//missing comment
	static const float	THRESHOLD_HEAT_COMFORT_MINUS_CRITICAL	= -0.5;	//missing comment
	static const float	THRESHOLD_HEAT_COMFORT_MINUS_EMPTY		= -0.9;	//missing comment
	
	
	static const float	THRESHOLD_HEALTH_WARNING		= 60;		//yellow tendency icon
	static const float	THRESHOLD_HEALTH_CRITICAL		= 40;		//red tendency icon
	static const float	THRESHOLD_HEALTH_EMPTY			= 20;		//red blinking tendency icon	
	//--------------------------------------------------------
	static const int BLOOD_THRESHOLD_FATAL 				= 2500;	//fatal blood level
	
	static const float BLOOD_REGEN_SPEED					= 1; 	//base amount of blood regenerated per second (BLOOD_REGEN_RATE_PER_SEC ?)
	static const float DAMAGE_ZONE_BLOOD_REGEN_MODIFIER 	= 1;
	
	static const int BLOOD_REGEN_THRESHOLD_ENERGY_LOW		= 200;
	static const int BLOOD_REGEN_THRESHOLD_ENERGY_MID		= 400;
	//static const int BLOOD_REGEN_THRESHOLD_ENERGY_HIGH		= 2000;
	
	static const float BLOOD_REGEN_MODIFIER_ENERGY_LOW		= 0; 	//multiplier for blood regen rate 
	static const float BLOOD_REGEN_MODIFIER_ENERGY_MID		= 0.5;
	static const float BLOOD_REGEN_MODIFIER_ENERGY_HIGH	= 1;
	
	static const int BLOOD_REGEN_THRESHOLD_WATER_LOW		= 250;
	static const int BLOOD_REGEN_THRESHOLD_WATER_MID		= 500;
	//static const int BLOOD_REGEN_THRESHOLD_WATER_HIGH		= 2000;
	
	static const float BLOOD_REGEN_MODIFIER_WATER_LOW		= 0; 	//multiplier for blood regen rate (BLOOD_REGEN_MULTIPLIER_WATER_LOW ?)
	static const float BLOOD_REGEN_MODIFIER_WATER_MID		= 0.5;
	static const float BLOOD_REGEN_MODIFIER_WATER_HIGH		= 1;
	
	static const float SALINE_BLOOD_REGEN_PER_SEC			= 2;	//boost for blood regen per second, independent on BLOOD_REGEN_SPEED
	static const float SALINE_LIQUID_AMOUNT					= 250;
	
	static const float HEMOLYTIC_BLOOD_DRAIN_PER_SEC		= 2;	//hemolytic reaction blood drain per second
	static const float HEMOLYTIC_BLOODLOSS_AMOUNT			= 250;
	
	static const float WATER_LOSS_THRESHOLD_HC_PLUS_LOW		= THRESHOLD_HEAT_COMFORT_PLUS_WARNING;
	static const float WATER_LOSS_THRESHOLD_HC_PLUS_HIGH	= THRESHOLD_HEAT_COMFORT_PLUS_EMPTY;
	
	static const float ENERGY_LOSS_THRESHOLD_HC_MINUS_LOW	= THRESHOLD_HEAT_COMFORT_MINUS_WARNING;
	static const float ENERGY_LOSS_THRESHOLD_HC_MINUS_HIGH	= THRESHOLD_HEAT_COMFORT_MINUS_EMPTY;
	
	static const float WATER_LOSS_HC_PLUS_LOW	= 0.1;
	static const float WATER_LOSS_HC_PLUS_HIGH	= 1.0;	

	static const float ENERGY_LOSS_HC_MINUS_LOW	= 0.1;
	static const float ENERGY_LOSS_HC_MINUS_HIGH	= 1.0;
	
	static const float HEALTH_LOSS_HC_PLUS_LOW	= 0.01;
	static const float HEALTH_LOSS_HC_PLUS_HIGH	= 0.1;		
	
	static const float HEALTH_LOSS_HC_MINUS_LOW	= 0.01;
	static const float HEALTH_LOSS_HC_MINUS_HIGH = 0.1;	
	
	//--------------------------------------------------------
	static const float LOW_ENERGY_DAMAGE_PER_SEC			= 0.175;	//health loss per second while low on energy
	static const float LOW_WATER_DAMAGE_PER_SEC			= 0.225;	//health loss per second while low on water
	
	static const float HEALTH_REGEN_MIN					= 0.025;	//health regen rate at BLOOD_THRESHOLD_FATAL blood level
	static const float HEALTH_REGEN_MAX					= 0.15;		//health regen rate at MAXIMUM blood level
	
	static const float UNCONSCIOUS_THRESHOLD				= 25.0;		//player goes unconscious when we get under this threshold
	static const float CONSCIOUS_THRESHOLD					= 50.0;		//player regains consciousness when he gets above this threshold
	
	static const float SHOCK_REFILL_CONSCIOUS_SPEED		= 5;		//shock refill speed when the player is conscious
	static const float SHOCK_REFILl_UNCONSCIOUS_SPEED		= 1;		//shock refill speed when the player is unconscious
	
	static const float SHOCK_DAMAGE_BLOOD_THRESHOLD_HIGH	= 3000;		// we start dealing shock damage over time when we get at this value or lower
	static const float SHOCK_DAMAGE_BLOOD_THRESHOLD_LOW	= 2500; 	// the closer we get to this value, the higher the over time shock damage we deal
	
	static const float SHOCK_DAMAGE_HIGH	= 20; // shock damage per second when the blood is near the upper blood threshold
	static const float SHOCK_DAMAGE_LOW	= 20; // shock damage per second when the blood is near the lower blood threshold
	
	static const float SHOCK_REFILL_COOLDOWN_AFTER_HIT = 15; // duration of cooldown during which shock regeneration is paused
	
	static const float UNCONSCIOUS_IN_WATER_TIME_LIMIT_TO_DEATH	=	20;	// how long can player survive while unconscious when in water in secs 
	//----------------------------------------------------------
	static const float BLEEDING_SOURCE_BLOODLOSS_PER_SEC = -6; // amount of blood loss per second from one bleeding source 
	//----------------------------------------------------------
	static const float BREATH_VAPOUR_THRESHOLD_HIGH = -5.0;
	static const float BREATH_VAPOUR_THRESHOLD_LOW = 7.0;
}