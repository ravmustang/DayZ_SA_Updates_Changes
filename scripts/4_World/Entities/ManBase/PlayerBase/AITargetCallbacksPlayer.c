class AITargetCallbacksPlayer : AITargetCallbacks
{
	void AITargetCallbacksPlayer(PlayerBase pPlayer)
	{
		m_Player = pPlayer;
		
		m_iHeadBoneIndex = m_Player.GetBoneIndexByName("Head");
		m_iChestBoneIndex = m_Player.GetBoneIndexByName("Spine3");
	}
	
	override vector GetHeadPositionWS() 
	{ 
		if( m_iHeadBoneIndex != -1 )
			return m_Player.GetBonePositionWS(m_iHeadBoneIndex);
		else
			return m_Player.GetPosition() + "0 1.7 0";
	}
	
	override vector GetVisionPointPositionWS(EntityAI pApplicant) 
	{ 
		DayZInfected infected = DayZInfected.Cast(pApplicant);
		if( infected )
		{
			DayZInfectedInputController ic = infected.GetInputController();
			if( ic )
			{
				int mindState = ic.GetMindState();
				if( mindState >= DayZInfectedConstants.MINDSTATE_ALERTED )
				{
					return GetHeadPositionWS();
				}
				else
				{
					if( m_iChestBoneIndex != -1 )
						return m_Player.GetBonePositionWS(m_iChestBoneIndex);
				}				
			}
		}
		
		return m_Player.GetPosition() + "0 1 0";
	}
	
	override float GetMaxVisionRangeModifier(EntityAI pApplicant)
	{ 
		HumanMovementState state = new HumanMovementState;
		m_Player.GetMovementState(state);
		
		float mod = 1.0;
		float speedCoef = 1.0;
		float stanceCoef = 1.0;
		float playerVisCoef = 1.0;
		
		//! player speed mofifications
		switch(state.m_iMovement)
		{
			case DayZPlayerConstants.MOVEMENT_RUN:
				speedCoef = 0.66;
				break;
				
			case DayZPlayerConstants.MOVEMENT_WALK:
				speedCoef = 0.33;
				break;
				
			case DayZPlayerConstants.MOVEMENT_IDLE:
				speedCoef = 0.11;
				break;
		}

		//! stance modification
		switch(state.m_iStanceIdx)
		{
			case DayZPlayerConstants.STANCEIDX_CROUCH:
			case DayZPlayerConstants.STANCEIDX_RAISEDCROUCH:
				stanceCoef = 0.66;
				break;
				
			case DayZPlayerConstants.STANCEIDX_PRONE:
			case DayZPlayerConstants.STANCEIDX_RAISEDPRONE:
				stanceCoef = 0.33;
				break;
		}
		
		//! player visibility modification (from cloths)
		playerVisCoef = m_Player.GetVisibilityCoef();

		//! mean value of the coefs
		mod = (speedCoef + stanceCoef + playerVisCoef) / 3;
		
		return mod;
	}

	private PlayerBase m_Player;
	private int m_iHeadBoneIndex;
	private int m_iChestBoneIndex;
};