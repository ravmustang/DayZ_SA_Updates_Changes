enum EPlayerSoundEventID
{
	HOLD_BREATH = 1,
	RELEASE_BREATH,
	STAMINA_DOWN_LIGHT,
	STAMINA_DOWN_HEAVY,
	STAMINA_UP_LIGHT,
	STAMINA_UP_HEAVY,
	STAMINA_UP_END,
	TAKING_DMG_LIGHT,
	SYMPTOM_COUGH,
	SYMPTOM_LAUGHTER,
	SYMPTOM_SNEEZE,
	//--------------
	ENUM_COUNT,
}

class PlayerSoundEventHandler
{
	PlayerBase m_Player;
	static ref PlayerSoundEventBase m_AvailableStates[128];
	ref PlayerSoundEventBase m_CurrentState;
	
	
	void PlayerSoundEventHandler(PlayerBase player)
	{
		m_Player = player;
		
		m_AvailableStates[EPlayerSoundEventID.HOLD_BREATH] = new HoldBreathSoundEvent();
		m_AvailableStates[EPlayerSoundEventID.RELEASE_BREATH] = new ReleaseBreathSoundEvent();
		m_AvailableStates[EPlayerSoundEventID.STAMINA_DOWN_LIGHT] = new StaminaDownLight();
		m_AvailableStates[EPlayerSoundEventID.STAMINA_DOWN_HEAVY] = new StaminaDownHeavy();
		m_AvailableStates[EPlayerSoundEventID.STAMINA_UP_LIGHT] = new StaminaUpLight();
		m_AvailableStates[EPlayerSoundEventID.STAMINA_UP_HEAVY] = new StaminaUpHeavy();
		m_AvailableStates[EPlayerSoundEventID.STAMINA_UP_END] = new StaminaUpEnd();
		m_AvailableStates[EPlayerSoundEventID.TAKING_DMG_LIGHT] = new DamageLightSoundEvent();
		m_AvailableStates[EPlayerSoundEventID.SYMPTOM_SNEEZE] = new SneezeSoundEvent();
		m_AvailableStates[EPlayerSoundEventID.SYMPTOM_LAUGHTER] = new LaugherSoundEvent();
		m_AvailableStates[EPlayerSoundEventID.SYMPTOM_COUGH] = new CoughSoundEvent();
	}
	
	static EPlayerSoundEventType GetPlayerSoundEventType(EPlayerSoundEventID id)
	{
		return m_AvailableStates[id].GetPlayerSoundEventType();
	}
	
	EPlayerSoundEventID GetCurrentStateEventID()
	{
		if(m_CurrentState)
		{
			return m_CurrentState.GetPlayerSoundEventID();
		}
		return -1;
	}
	
	EPlayerSoundEventType GetCurrentStateEventType()
	{
		if(m_CurrentState)
		{
			return m_CurrentState.GetPlayerSoundEventType();
		}
		return -1;
	}
	
	void OnTick(float delta_time)
	{
		if(m_CurrentState)
		{
			m_CurrentState.Tick();
			
			if( m_CurrentState.IsDestroyRequested() )
			{
				delete m_CurrentState;
			}
	
		}
	}
	
	bool PlayRequest(EPlayerSoundEventID id)
	{
		PlayerSoundEventBase requested_state = m_AvailableStates[id].ClassName().ToType().Spawn();
		requested_state.Init(m_Player);
		
		if( !requested_state.CanPlay() )
		{
			return false;
		}
		
		if(m_CurrentState)
		{
			
			if( m_CurrentState.ThisHasPriority(id) )
			{
				// do nothing
				return false;
			}
			else
			{
				m_CurrentState.Stop();
				requested_state.Play();
				m_CurrentState = requested_state;
				return true;
			}
		}
		else
		{
			m_CurrentState = requested_state;
			m_CurrentState.Play();
			return true;
		}
		return false;//should never get here
	}
}