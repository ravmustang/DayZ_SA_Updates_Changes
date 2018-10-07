const float TICK_INTERVAL = 1;

const float STAMINA_SOUND_TR1 = 0.7;
const float STAMINA_SOUND_TR2 = 0.4;

enum eStaminaZones
{
	ZONE0,
	ZONE1,
	ZONE2,
}

enum eStaminaTendency
{
	UP,
	DOWN,
}

enum eStaminaState
{
	ZONE0_UP = 1,
	ZONE0_DOWN,
	ZONE1_UP,
	ZONE1_DOWN,
	ZONE2_UP,
	ZONE2_DOWN,
}

class StaminaSoundHandler
{
	StaminaHandler m_StaminaHandler;
	eStaminaZones m_StaminaZone;
	eStaminaZones m_LastStaminaZone;
	
	eStaminaTendency m_StaminaTendency;
	
	PlayerBase m_Player;
	
	eStaminaState m_StaminaState;
	
	float m_TimeAccu;
	float m_StaminaLastValue;
	float m_Stamina;
	
	bool m_StaminaUpEndPlayed = true;
	
	void StaminaSoundHandler(StaminaHandler handler, PlayerBase player)
	{
		m_StaminaHandler = handler;
		m_Player = player;
		
	}
	
	void Update(float stamina_value, float deltaT)
	{
		if( GetGame().IsServer() || !GetGame().IsMultiplayer() ) 
		{
			ProcessServer(stamina_value);
		}
		if( GetGame().IsClient() || !GetGame().IsMultiplayer() ) 
		{
			ProcessClient();
		}
	}
	
	void ProcessClient()
	{
		m_Player.GetPlayerSoundEventHandler();
		eStaminaState stamina_state = m_Player.GetStaminaState();
		
		switch(stamina_state)
		{
			case eStaminaState.ZONE1_DOWN:
				if(m_Player.PlaySoundEvent(EPlayerSoundEventID.STAMINA_DOWN_LIGHT))
				{
					m_StaminaUpEndPlayed = false;
				}
			break;
			
			case eStaminaState.ZONE1_UP:
				if(m_Player.PlaySoundEvent(EPlayerSoundEventID.STAMINA_UP_LIGHT))
				{
					m_StaminaUpEndPlayed = false;
				}
			break;
			
			case eStaminaState.ZONE2_DOWN:
				if(m_Player.PlaySoundEvent(EPlayerSoundEventID.STAMINA_DOWN_HEAVY))
				{
					m_StaminaUpEndPlayed = false;
				}
			break;
			
			case eStaminaState.ZONE2_UP:
				if(m_Player.PlaySoundEvent(EPlayerSoundEventID.STAMINA_UP_HEAVY))
				{
					m_StaminaUpEndPlayed = false;
				}
			break;
			
			case eStaminaState.ZONE0_UP:
				if(!m_StaminaUpEndPlayed && m_Player.PlaySoundEvent(EPlayerSoundEventID.STAMINA_UP_END))
				{
					m_StaminaUpEndPlayed = true;
				}
			break;
		}
	
	}
	
	void ProcessServer(float stamina_value)
	{	
		m_Stamina = stamina_value;
		float stamina_delta = m_StaminaLastValue - m_Stamina;
		
		if(stamina_delta > 0)
		{
			m_StaminaTendency = eStaminaTendency.DOWN;
		}
		else
		{
			m_StaminaTendency = eStaminaTendency.UP;
		}
		
		m_StaminaLastValue = stamina_value;
		
		m_StaminaZone = GetZone();
		m_Player.SetStaminaState( GetStaminaState() );
		
		//EPlayerSoundEventID sound_event_id = GetPlayerSoundEventID();
		/*
		PrintString("stamina delta " +stamina_delta.ToString());
		PrintString("stamina zone " +m_StaminaZone.ToString());
		PrintString("stamina tendency " +m_StaminaTendency.ToString());
		PrintString(" sound event id " +sound_event_id.ToString());
		*/
		
		//if( sound_event_id !=0 ) m_Player.SendSoundEvent(sound_event_id);
		
		
	}

	eStaminaZones GetZone()
	{
		float stamina_normalized = Math.InverseLerp(0, m_StaminaHandler.GetStaminaCap(), m_Stamina);
		
		//PrintString(" stamina_normalized " +stamina_normalized.ToString());
		eStaminaZones stamina_zone = eStaminaZones.ZONE0;
		
		if( stamina_normalized < STAMINA_SOUND_TR1)
		{
			stamina_zone = eStaminaZones.ZONE1;
		}
		
		if( stamina_normalized < STAMINA_SOUND_TR2)
		{
			stamina_zone = eStaminaZones.ZONE2;
		}
		
		return stamina_zone;
	}
	
	eStaminaState GetStaminaState()
	{
		eStaminaState stamina_state;
		
		if(m_StaminaZone == eStaminaZones.ZONE1 && m_StaminaTendency == eStaminaTendency.DOWN)
		{
			stamina_state = eStaminaState.ZONE1_DOWN;
		}
		
		if(m_StaminaZone == eStaminaZones.ZONE1 && m_StaminaTendency == eStaminaTendency.UP)
		{
			stamina_state = eStaminaState.ZONE1_UP;
		}
		
		if(m_StaminaZone == eStaminaZones.ZONE2 && m_StaminaTendency == eStaminaTendency.DOWN)
		{
			stamina_state = eStaminaState.ZONE2_DOWN;
		}
		
		if(m_StaminaZone == eStaminaZones.ZONE2 && m_StaminaTendency == eStaminaTendency.UP)
		{
			stamina_state = eStaminaState.ZONE2_UP;
		}
		
		if(m_StaminaZone == eStaminaZones.ZONE0 && m_StaminaTendency == eStaminaTendency.UP)
		{
			stamina_state = eStaminaState.ZONE0_UP;
		}
		
		if(m_StaminaZone == eStaminaZones.ZONE0 && m_StaminaTendency == eStaminaTendency.DOWN)
		{
			stamina_state = eStaminaState.ZONE0_DOWN;
		}
		
		return stamina_state;
	}

}