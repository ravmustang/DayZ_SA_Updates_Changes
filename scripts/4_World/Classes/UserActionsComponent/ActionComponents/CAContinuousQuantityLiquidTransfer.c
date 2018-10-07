class CAContinuousQuantityLiquidTransfer : CAContinuousBase
{
	protected float 				m_ItemQuantity;
	protected float 				m_SpentQuantity;
	protected float 				m_SpentQuantity_total;
	protected float 				m_ItemMaxQuantity; //basically free capacity
	protected float					m_TimeElpased;
	protected float 				m_QuantityUsedPerSecond;
	protected float 				m_AdjustedQuantityUsedPerSecond;
	protected float 				m_DefaultTimeStep;
	protected ref Param1<float>		m_SpentUnits;
	
	protected PlayerBase 			m_Player;
	
	void CAContinuousQuantityLiquidTransfer( float quantity_used_per_second, float time_to_progress )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeStep = time_to_progress;
	}
	
	override void Setup( ActionData action_data )
	{
		m_Player = action_data.m_Player;
		
		ItemBase target_item = ItemBase.Cast(action_data.m_Target.GetObject());
		
		m_TimeElpased = 0;
		m_SpentQuantity = 0;
		
		if ( !m_SpentUnits )
		{
			m_SpentUnits = new Param1<float>( 0 );
		}
		else
		{
			m_SpentUnits.param1 = 0;
		}
		
		if ( action_data.m_Player.GetLiquidTendencyDrain() )
		{
			if ( target_item.GetQuantity() > (action_data.m_MainItem.GetQuantityMax() - action_data.m_MainItem.GetQuantity()) )
			{
				m_ItemMaxQuantity = action_data.m_MainItem.GetQuantityMax() - action_data.m_MainItem.GetQuantity();
				m_ItemQuantity = m_ItemMaxQuantity; //target_item.GetQuantity();
			}
			else //
			{
				m_ItemMaxQuantity = target_item.GetQuantity();
				m_ItemQuantity = m_ItemMaxQuantity; //target_item.GetQuantity();
			}
		}
		else
		{
			if ( action_data.m_MainItem.GetQuantity() > (target_item.GetQuantityMax() - target_item.GetQuantity()) )
			{
				m_ItemMaxQuantity = target_item.GetQuantityMax() - target_item.GetQuantity();
				m_ItemQuantity = m_ItemMaxQuantity; //action_data.m_MainItem.GetQuantity();
			}
			else //
			{
				m_ItemMaxQuantity = action_data.m_MainItem.GetQuantity();
				m_ItemQuantity = m_ItemMaxQuantity; //target_item.GetQuantity();
			}
		}
		//m_ItemMaxQuantity = action_data.m_MainItem.GetQuantityMax();
		//m_ItemQuantity = action_data.m_MainItem.GetQuantity();
	}
	
	
	override int Execute( ActionData action_data  )
	{
		ItemBase target_item = ItemBase.Cast(action_data.m_Target.GetObject());
		
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		if ( m_ItemQuantity <= 0 )
		{
			return UA_FINISHED;
		}
		else
		{
			if ( m_SpentQuantity_total < m_ItemQuantity )
			{
				m_AdjustedQuantityUsedPerSecond = action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( m_QuantityUsedPerSecond, m_Action.GetSpecialtyWeight(), true);
				m_SpentQuantity += m_AdjustedQuantityUsedPerSecond * action_data.m_Player.GetDeltaT();
				m_TimeElpased += action_data.m_Player.GetDeltaT();
				
				if ( m_TimeElpased >= m_DefaultTimeStep )
				{
					CalcAndSetQuantity( action_data );
					m_TimeElpased = 0;
					//Setup(action_data);	//reset data after repeat
				}
				
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity( action_data );
				return UA_FINISHED;
			}
		}
	}
	
	override int Cancel( ActionData action_data )
	{
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity( action_data );
		return UA_INTERRUPT;
	}	
	
	override float GetProgress()
	{
		//float progress = ( m_ItemQuantity - m_SpentQuantity ) / m_ItemMaxQuantity;
		//float progress = ( m_ItemQuantity - m_SpentQuantity ) / m_ItemMaxQuantity;
		//float progress2 = m_ItemQuantity - m_SpentQuantity / m_ItemMaxQuantity;;
		
		//return (m_ItemQuantity - m_SpentQuantity) / m_ItemMaxQuantity;
		
		if ( m_Player.GetLiquidTendencyDrain() )
		{
			return m_SpentQuantity_total / m_ItemMaxQuantity;
		}
		else
		{
			return -(m_SpentQuantity_total / m_ItemMaxQuantity);
		}
	}
	
	//---------------------------------------------------------------------------
	
	void CalcAndSetQuantity( ActionData action_data )
	{
		ItemBase target_item = ItemBase.Cast(action_data.m_Target.GetObject());
		m_SpentQuantity_total += m_SpentQuantity;
		
		if ( GetGame().IsServer() )
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_SpentQuantity;
				SetACData(m_SpentUnits);
			}
			
			//could move following stuff to action itself, if needed
			if ( action_data.m_Player.GetLiquidTendencyDrain() )
			{
				Liquid.Transfer(target_item, action_data.m_MainItem, m_SpentQuantity);
			}
			else
			{
				Liquid.Transfer(action_data.m_MainItem, target_item, m_SpentQuantity);
			}
			//action_data.m_MainItem.AddQuantity( -m_SpentQuantity, false, false );
		}
		m_SpentQuantity = 0;
	}
}