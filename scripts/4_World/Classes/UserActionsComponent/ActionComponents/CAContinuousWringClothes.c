class CAContinuousWringClothes : CAContinuousQuantityRepeat
{
	protected float 	m_InitItemQuantity;
	protected float 	m_SpentQuantityTotal;
	
	void CAContinuousWringClothes( float quantity_used_per_second, float time_to_repeat )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeToRepeat = time_to_repeat;
		m_InitItemQuantity = -1;
	}
	
	override void Setup( ActionData action_data )
	{
		super.Setup( action_data );
		
		if (m_InitItemQuantity < 0)
		{
			m_InitItemQuantity = action_data.m_MainItem.GetWet();
		}
	}
	
	
	override int Execute( ActionData action_data  )
	{
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		if ( action_data.m_MainItem.GetWet() <= 0 )
		{
			return UA_FINISHED;
		}
		else
		{
			if ( m_SpentQuantityTotal < m_InitItemQuantity )
			{
				m_SpentQuantity += m_QuantityUsedPerSecond * action_data.m_Player.GetDeltaT();
				m_TimeElpased += action_data.m_Player.GetDeltaT();

				if (m_TimeElpased > m_DefaultTimeToRepeat)
					m_TimeElpased -= m_DefaultTimeToRepeat;
				
				if ( GetGame().IsServer() )
				{
					if ( m_SpentUnits )
					{
						m_SpentUnits.param1 = m_SpentQuantity;
						SetACData(m_SpentUnits);
					}

					action_data.m_MainItem.AddWet(-m_SpentQuantity);
				}
				
				m_SpentQuantityTotal += m_SpentQuantity;
				m_SpentQuantity = 0;

				return UA_PROCESSING;
			}
			else
			{
				return UA_FINISHED;
			}
		}
	}
	
	override float GetProgress()
	{
		return ((m_TimeElpased - m_DefaultTimeToRepeat) / m_DefaultTimeToRepeat) + 1;
	}
};