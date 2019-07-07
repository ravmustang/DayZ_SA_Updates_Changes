class AgentBase extends MessageReceiverBase
{
	float			m_Type = 0;
	float			m_Invasibility;//how fast the agent grows when potent enough to grow
	float 			m_TransferabilityIn;//to the player
	float			m_TransferabilityOut;//from the player
	float			m_Digestibility = 0.1;//multiplier for agents digested in the player stomach from an infected item(agents_transfered = digested_amount(in grams or mls) * m_Digestibility)
	int				m_MaxCount = 1;
	float 			m_AutoinfectProbability = -1;//probability of autoinfect as percentage per hour (50 means 50% chance autoinfect with this agent will happen 1x times within an hour of gameplay)(also dependent on CanAutoinfectPlayer check)
	//int				m_ChanceOfInfection = 1;//chance of transmission from enviro sources like ponds etc. [0..1]
	float 			m_TransferabilityAirOut; // transferibility airborne out
	float 			m_AntibioticsResistance = 1;//[0..1], 0 means antibiotics have full effect, 1 means no effect
	
	EStatLevels m_Potency = EStatLevels.MEDIUM;//grow when player's immune system is at this level or lower
	float m_DieOffSpeed = 1;//how fast the agent dies off when not potent enough to grow(per sec)
	
	void AgentBase()
	{
		Init();
	}
	
	void Init();
	
	int GetAgentType()
	{
		return m_Type;
	}
	
	EStatLevels GetPotency()
	{
		return m_Potency;
	}	
	
	float GetDieOffSpeed()
	{
		return m_DieOffSpeed;
	}
	
	float GetAntiboticsResistance()
	{
		return m_AntibioticsResistance;
	}
	
	float GetInvasibility()
	{
		return m_Invasibility;
	}
	
	float GetDigestibility()
	{
		return m_Digestibility;
	}

	bool AutoinfectCheck(float deltaT, PlayerBase player)
	{
		if ( m_AutoinfectProbability == -1 ) 
			return false;
		
		float probability = (m_AutoinfectProbability / 3600) * deltaT * 1000;
		float dice_throw = Math.RandomFloat(0,100 * 1000);
		/*
		Print(probability);
		Print(dice_throw);
		Print("-----------");
		*/
		if( dice_throw < probability )
		{
			return CanAutoinfectPlayer(player);
		}
		else 
		{
			return false;
		}
	}

	bool CanAutoinfectPlayer(PlayerBase player)
	{
		return false;
	}
	
	float GetTransferabilityIn()
	{
		return m_TransferabilityIn;
	}

	float GetTransferabilityOut()
	{
		return m_TransferabilityOut;
	}	
	
	float GetTransferabilityAirOut()
	{
		return m_TransferabilityAirOut;
	}

	int GetMaxCount()
	{
		return m_MaxCount;
	}

	string GetName()
	{
		return ClassName();
	}
}