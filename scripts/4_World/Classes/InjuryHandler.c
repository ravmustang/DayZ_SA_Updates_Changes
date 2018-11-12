class InjuryHandlerThresholds
{
	const float WORN = 0.8;
	const float DAMAGED = 0.6;
	const float BADLY_DAMAGED = 0.4;
	const float RUINED = 0.2;
};

class InjuryHandlerValues
{
	const float PRISTINE = 0;
	const float WORN = 0.3;
	const float DAMAGED = 0.6;
	const float BADLY_DAMAGED = 0.8;
	const float RUINED = 1;
};

enum eInjuryHandlerLevels
{
	PRISTINE,
	WORN,
	DAMAGED,
	BADLY_DAMAGED,
	RUINED,
}

class InjuryHandler
{

	const float VALUE_CHECK_INTERVAL	 	= 5;
	const float SENSITIVTY_PERCENTAGE 		= 1;//how much the value needs to change up/down from previous update to trigger a new update(in percent)

	ref ScriptInvoker m_ChangedStateInvoker	= new ScriptInvoker();
	
	float m_TimeSinceLastTick = VALUE_CHECK_INTERVAL + 1;
	float m_LastUpdate;
	eInjuryHandlerLevels m_LastHealthUpdate;
	float m_HealthMaxValue;
	
	
	private PlayerBase m_Player; //! owner
	private bool m_AnimationChange = false;
	private bool m_InjuryAnimEnabled = false;
	private float m_InjuryAnimDamageValue = 0;
	
	
	void InjuryHandler(PlayerBase player)
	{
		m_Player = player;
		m_HealthMaxValue 	= m_Player.GetMaxHealth("", "Health");
	}
	
	ScriptInvoker GetInvoker()
	{
		return m_ChangedStateInvoker;
	}
	
	void Update(float deltaT)
	{
		if( m_AnimationChange )
		{
			m_Player.GetCommandModifier_Additives().SetInjured(m_InjuryAnimDamageValue, m_InjuryAnimEnabled);
			m_AnimationChange = false;
		}

		if( GetGame().IsClient() ) return;
		m_TimeSinceLastTick += deltaT;

		if( m_TimeSinceLastTick > VALUE_CHECK_INTERVAL )
		{
			CheckValue();
			m_TimeSinceLastTick = 0;
		}
		
	}

/*
	void CheckValue()
	{
		float health_current = m_Player.GetHealth("","Health");
		float health_normalized = Math.Clamp( (health_current / m_HealthMaxValue) * 2, 0, 1);
		float difference_normalized = health_normalized - m_LastHealthUpdate;
		float diff_abs = Math.AbsFloat(difference_normalized);
		
		if( diff_abs > ( SENSITIVTY_PERCENTAGE /100 ) )
		{
			SendValue(health_normalized);
			m_LastHealthUpdate = health_normalized;
		}
		
	}
	*/
	
	void CheckValue(bool forceUpdate = false)
	{
		float health_current_normalized = m_Player.GetHealth("","Health") / m_HealthMaxValue;
		eInjuryHandlerLevels injury_level = GetInjuryLevel(health_current_normalized);
		
		if( m_LastHealthUpdate != injury_level || forceUpdate )
		{
			SendValue(injury_level);
			m_ChangedStateInvoker.Invoke(injury_level);
			m_LastHealthUpdate = injury_level;
			Synchronize(injury_level);
		}
		
	}
	
	void Synchronize(eInjuryHandlerLevels level)
	{
		m_Player.m_HealthLevel = level;
		Print("m_Player.m_HealthLevel:" + m_Player.m_HealthLevel);
		m_Player.SetSynchDirty();
	}

	void SendValue(eInjuryHandlerLevels level)
	{
		DayZPlayerSyncJunctures.SendInjury(m_Player, true, level);
	}
	
	void SetInjuryCommandParams(bool enable, eInjuryHandlerLevels level)
	{
		m_AnimationChange = true;
		m_InjuryAnimEnabled = enable;
		m_InjuryAnimDamageValue = GetInjuryValue(level);
	}
	
	/*
	float GetInjuryLevel(float health)
	{
		if ( health  < InjuryHandlerThresholds.RUINED ) 
		{
			return InjuryHandlerLevels.RUINED;
		}
		
		if ( health  < InjuryHandlerThresholds.BADLY_DAMAGED ) 
		{
			return InjuryHandlerLevels.BADLY_DAMAGED;
		}
		
		if ( health  < InjuryHandlerThresholds.DAMAGED ) 			
		{
			return InjuryHandlerLevels.DAMAGED;
		}
		
		if ( health  < InjuryHandlerThresholds.WORN ) 		
		{
			return InjuryHandlerLevels.WORN;
		}
		
		return InjuryHandlerLevels.PRISTINE;
	}
	*/
	
	eInjuryHandlerLevels GetInjuryLevel(float health)
	{
		if ( health  < InjuryHandlerThresholds.RUINED ) 
		{
			return eInjuryHandlerLevels.RUINED;
		}
		
		if ( health  < InjuryHandlerThresholds.BADLY_DAMAGED ) 
		{
			return eInjuryHandlerLevels.BADLY_DAMAGED;
		}
		
		if ( health  < InjuryHandlerThresholds.DAMAGED ) 			
		{
			return eInjuryHandlerLevels.DAMAGED;
		}
		
		if ( health  < InjuryHandlerThresholds.WORN ) 		
		{
			return eInjuryHandlerLevels.WORN;
		}
		
		return eInjuryHandlerLevels.PRISTINE;
	}
	
	
	float GetInjuryValue(eInjuryHandlerLevels level)
	{
		switch(level)
		{
			case eInjuryHandlerLevels.RUINED:
			{
				return InjuryHandlerValues.RUINED;
			}
			case eInjuryHandlerLevels.BADLY_DAMAGED:
			{
				return InjuryHandlerValues.BADLY_DAMAGED;
			}
			case eInjuryHandlerLevels.DAMAGED:
			{
				return InjuryHandlerValues.DAMAGED;
			}
			case eInjuryHandlerLevels.WORN:
			{
				return InjuryHandlerValues.WORN;
			}
			case eInjuryHandlerLevels.PRISTINE:
			{
				return InjuryHandlerValues.PRISTINE;
			}
			default:
				Error("Undefined Injury level");
		}
		return 0;
	}

}