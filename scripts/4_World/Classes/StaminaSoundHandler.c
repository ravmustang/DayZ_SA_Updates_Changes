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
const float STAMINA_SOUNDS_HEARING_DISTANCE = 50;

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
	float m_LastTick;
	bool m_InHearingRangeOfControlledPlayer;
	bool m_StaminaUpEndPlayed = true;
	ref Timer m_ClientCharacterTick = new Timer;
	ref Timer m_ClientCharacterDistanceCheck = new Timer;
	
	void StaminaSoundHandler(StaminaHandler handler, PlayerBase player)
	{
		m_StaminaHandler = handler;
		m_Player = player;
		m_ClientCharacterTick.Run(0.01, this, "Update", null, true); // calls "Refresh" on "this" after 10 seconds
		
		if( GetGame().IsClient() || !GetGame().IsMultiplayer() ) 
		{
			m_ClientCharacterDistanceCheck.Run(2, this, "CheckDistance", null, true); // calls "Refresh" on "this" after 10 seconds
		}
	}
	
	void ~StaminaSoundHandler()
	{
		
	}
	

	void Update()
	{
		if( !m_Player.IsAlive() )
		{
			m_ClientCharacterTick.Stop();
		}
		if( GetGame().IsServer() || !GetGame().IsMultiplayer() ) 
		{
			ProcessServer();
		}
		if( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			ProcessClient();
		}
	}
	
	void CheckDistance()
	{
		if(GetGame().GetPlayer())
		{
			m_InHearingRangeOfControlledPlayer = vector.Distance(GetGame().GetPlayer().GetPosition(), m_Player.GetPosition()) < STAMINA_SOUNDS_HEARING_DISTANCE;
			//PrintString("distance:" + vector.Distance(GetGame().GetPlayer().GetPosition(), m_Player.GetPosition()).ToString());
		}
	}
	
	void ProcessClient()
	{
		//Print(m_InHearingRangeOfControlledPlayer);
		if(!m_InHearingRangeOfControlledPlayer) return;
		
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
				else
				{
					m_Player.PlaySoundEvent(EPlayerSoundEventID.STAMINA_NORMAL_DUMMY);
				}
			break;
			
			case eStaminaState.ZONE0_DOWN:
				m_Player.PlaySoundEvent(EPlayerSoundEventID.STAMINA_NORMAL_DUMMY);
			break;
			
			
		}
	
	}
	
	void ProcessServer()
	{	
		m_Stamina = m_StaminaHandler.GetStamina();
		float stamina_delta = m_StaminaLastValue - m_Stamina;
		eStaminaTendency stamina_tendency;
		eStaminaZones stamina_zone;
		
		if( stamina_delta == 0 ) return;
		
		if( stamina_delta > 0 )
		{
			stamina_tendency = eStaminaTendency.DOWN;
		}
		if( stamina_delta < 0 )
		{
			stamina_tendency = eStaminaTendency.UP;
		}
		
		m_StaminaLastValue = m_Stamina;
		
		stamina_zone = GetZone(m_Stamina);
		m_Player.SetStaminaState( GetStaminaState(stamina_tendency, stamina_zone) );
		
		//EPlayerSoundEventID sound_event_id = GetPlayerSoundEventID();
		/*
		//PrintString("stamina delta " +stamina_delta.ToString());
		//PrintString("stamina zone " +m_StaminaZone.ToString());
		//PrintString("stamina tendency " +m_StaminaTendency.ToString());
		*/
		//PrintString(" sound event id " +sound_event_id.ToString());
		
		
		//if( sound_event_id !=0 ) m_Player.SendSoundEvent(sound_event_id);
		
		
	}

	eStaminaZones GetZone(float stamina)
	{
		float stamina_normalized = Math.InverseLerp(0, m_StaminaHandler.GetStaminaCap(), stamina);
		stamina_normalized = Math.Clamp(stamina_normalized,0,1);
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
	
	eStaminaState GetStaminaState(eStaminaTendency tendency, eStaminaZones zone)
	{
		eStaminaState stamina_state;
		
		if(zone == eStaminaZones.ZONE1 && tendency == eStaminaTendency.DOWN)
		{
			stamina_state = eStaminaState.ZONE1_DOWN;
		}
		
		if(zone == eStaminaZones.ZONE1 && tendency == eStaminaTendency.UP)
		{
			stamina_state = eStaminaState.ZONE1_UP;
		}
		
		if(zone == eStaminaZones.ZONE2 && tendency == eStaminaTendency.DOWN)
		{
			stamina_state = eStaminaState.ZONE2_DOWN;
		}
		
		if(zone == eStaminaZones.ZONE2 && tendency == eStaminaTendency.UP)
		{
			stamina_state = eStaminaState.ZONE2_UP;
		}
		
		if(zone == eStaminaZones.ZONE0 && tendency == eStaminaTendency.UP)
		{
			stamina_state = eStaminaState.ZONE0_UP;
		}
		
		if(zone == eStaminaZones.ZONE0 && tendency == eStaminaTendency.DOWN)
		{
			stamina_state = eStaminaState.ZONE0_DOWN;
		}
	
		return stamina_state;
	}

}