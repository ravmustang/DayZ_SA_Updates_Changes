class HeatComfortAnimHandler
{
	PlayerBase m_Player;
	float m_SoundTime;
	protected ref HumanMovementState m_MovementState = new HumanMovementState();
	
	const float TIME_INTERVAL_HC_MINUS_LOW_MIN = 5000; const float TIME_INTERVAL_HC_MINUS_LOW_MAX = 12000;
	const float TIME_INTERVAL_HC_MINUS_HIGH_MIN = 15000; const float TIME_INTERVAL_HC_MINUS_HIGH_MAX = 25000;
	
	const float TIME_INTERVAL_HC_PLUS_LOW_MIN = 5000; const float TIME_INTERVAL_HC_PLUS_LOW_MAX = 12000;
	const float TIME_INTERVAL_HC_PLUS_HIGH_MIN = 15000; const float TIME_INTERVAL_HC_PLUS_HIGH_MAX = 25000;
	
	void HeatComfortAnimHandler(PlayerBase player)
	{
		m_Player = player;
	}

	void Update(float deltaT, HumanMovementState hms)
	{
		if( GetGame().IsServer() || !GetGame().IsMultiplayer() )
		{
			float hc = m_Player.GetStatHeatComfort().Get();
			float inv_value;
			float value_min;
			float value_max;
			float offset_time;
			
			if ( hc <= PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_WARNING )
			{
				if( GetGame().GetTime() > m_SoundTime )
				{
					inv_value = Math.InverseLerp(PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_EMPTY,PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_WARNING, hc);
					value_min = Math.Lerp(TIME_INTERVAL_HC_MINUS_LOW_MIN,TIME_INTERVAL_HC_MINUS_HIGH_MIN,inv_value);
					value_max = Math.Lerp(TIME_INTERVAL_HC_MINUS_LOW_MAX,TIME_INTERVAL_HC_MINUS_HIGH_MAX,inv_value);
					offset_time = Math.RandomFloatInclusive(value_min,value_max);
					
					m_SoundTime = GetGame().GetTime() + offset_time;
					m_Player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_FREEZE);
					/*
					Print("-----======== freezing ========-------");
					Print(inv_value);
					Print(value_min);
					Print(value_max);
					Print(offset_time);
					Print("-----======== freezing ========-------");
					*/
				}	
			}
			/*
			else if ( hc >= PlayerConstants.THRESHOLD_HEAT_COMFORT_PLUS_WARNING )
			{
				if( GetGame().GetTime() > m_SoundTime )
				{
					inv_value = Math.InverseLerp(PlayerConstants.THRESHOLD_HEAT_COMFORT_PLUS_EMPTY,PlayerConstants.THRESHOLD_HEAT_COMFORT_PLUS_WARNING, hc);
					value_min = Math.Lerp(TIME_INTERVAL_HC_PLUS_LOW_MIN,TIME_INTERVAL_HC_PLUS_HIGH_MIN, inv_value);
					value_max = Math.Lerp(TIME_INTERVAL_HC_PLUS_LOW_MAX,TIME_INTERVAL_HC_PLUS_HIGH_MAX, inv_value);
					offset_time = Math.RandomFloatInclusive(value_min,value_max);
					
					m_SoundTime = GetGame().GetTime() + offset_time;
					m_Player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_HOT);
					
					//Print("-----======== freezing ========-------");
					//Print(inv_value);
					//Print(value_min);
					//Print(value_max);
					//Print(offset_time);
					//Print("-----======== freezing ========-------");
					
				}	
			}
			*/
		}
	}
}