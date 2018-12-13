enum eAnimFinishType
{
	SUCCESS,
	FAILURE,
}

class SmptAnimMetaBase
{
	bool	m_IsPlaying;
	SymptomManager m_Manager;
	PlayerBase m_Player;
	int m_AnimID;
	int m_SymptomType;
	bool m_DestroyRequested;
	
	void SmptAnimMetaBase()
	{
	}
	
	void Init(ParamsReadContext ctx, SymptomManager manager, PlayerBase player)
	{
		m_Manager = manager;
		m_Player = player;
	}
	
	bool IsPlaying()
	{
		return m_IsPlaying;
	}
	
	bool IsDestroyReqested()
	{
		return m_DestroyRequested;
	}
	
	void AnimFinished(eAnimFinishType type)
	{
		m_DestroyRequested = true;
		SymptomBase Symptom = m_Manager.GetCurrentPrimaryActiveSymptom();
		
		if( type == eAnimFinishType.FAILURE)//   <--------------- FAILED
		{
			if( m_Player.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER )
			{
				if( Symptom ) 
				{
					Symptom.AnimationPlayFailed();
				}
			}
		}
		else if( type == eAnimFinishType.SUCCESS)//   <--------------- SUCCESS
		{
			if( m_Player.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER )
			{
				if( Symptom )
				{
					Symptom.AnimationFinish();
				}
			}
		}
	}
	
	bool PlayRequest()
	{
		bool played = Play();
		
		if(played)
		{
			
		}
		else
		{
			m_Manager.OnAnimationFinished(eAnimFinishType.FAILURE);
		}
		return played;
	}
	
	protected bool Play();
	void Update(HumanMovementState movement_state);

};

class SmptAnimMetaFB extends SmptAnimMetaBase
{
	int m_StanceMask;
	float m_Duration;
	
	override void Init(ParamsReadContext ctx, SymptomManager manager, PlayerBase player)
	{
		super.Init(ctx, manager, player);
		DayZPlayerSyncJunctures.ReadPlayerSymptomFBParams( ctx,  m_AnimID, m_StanceMask, m_Duration);
	}
	
	override bool Play()
	{
		HumanCommandActionCallback callback = m_Player.GetCommand_Action();
		
		if (!callback)
		{
			callback = m_Player.GetCommandModifier_Action();
		}
		if( callback )
		{
			callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_INTERRUPT);
		}
		
		SymptomCB anim_callback = SymptomCB.Cast(m_Player.StartCommand_Action(m_AnimID, SymptomCB, m_StanceMask));
		
		if(anim_callback)
		{
			anim_callback.Init(m_Duration, m_Player);
			m_IsPlaying = true;
			return true;
		}
		return false;
	}
}

class SmptAnimMetaADD extends SmptAnimMetaBase
{
	HumanCommandModifierAdditive m_Hcma;
	
	override void Init(ParamsReadContext ctx, SymptomManager manager, PlayerBase player)
	{
		super.Init(ctx, manager, player);
		DayZPlayerSyncJunctures.ReadPlayerSymptomADDParams( ctx, m_AnimID);
	}
	
	override bool Play()
	{
		HumanCommandActionCallback callback = m_Player.GetCommand_Action();
		if (!callback)
		{
			callback = m_Player.GetCommandModifier_Action();
		}
		if( !callback )
		{
			m_Hcma = m_Player.AddCommandModifier_Modifier(m_AnimID);
			
			if(m_Hcma)
			{
				m_IsPlaying = true;
				return true;
			}
		}
		return false;
	}
	
	override void Update(HumanMovementState movement_state)
	{
		if( m_IsPlaying )
		{
			if(!m_Player.GetCommandModifier_Modifier())
			{
				m_Manager.OnAnimationFinished();
			}
		}
	}
}


class HeatComfortmMetaADD extends SmptAnimMetaADD
{
	override void Update(HumanMovementState movement_state)
	{
		super.Update(movement_state);
		if( movement_state.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE)
		{
			m_Player.DeleteCommandModifier_Modifier(m_Hcma);
		}

	}
}