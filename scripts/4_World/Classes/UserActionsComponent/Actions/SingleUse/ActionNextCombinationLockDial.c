class ActionNextCombinationLockDial: ActionSingleUseBase
{
	protected CombinationLock 	m_CombinationLock;
	protected int 				m_DialIndex;
	
	void ActionNextCombinationLockDial()
	{
		m_DialIndex = 0;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool HasTarget()
	{
		return false;
	}
	
	override bool IsInstant()
	{
		return true;
	}
	
	override bool RemoveForceTargetAfterUse()
	{
		return false;
	}
	
	override int GetType()
	{
		return AT_NEXT_COMBINATION_LOCK_DIAL;
	}
	
	override string GetText()
	{
		return "#next_combination_lock_dial";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_CombinationLock = CombinationLock.Cast( item );
		
		return true;
	}

	override void Start( ActionData action_data )
	{
		SetNextDialIndex();
	}	
	
	int GetDialIndex()
	{
		return m_DialIndex;
	}
	
	CombinationLock GetCombinationLock()
	{
		return m_CombinationLock;
	}
	
	protected void SetNextDialIndex()
	{
		if ( m_CombinationLock.COMBINATION_LENGTH > 1 )
		{
			if ( m_DialIndex <= m_CombinationLock.COMBINATION_LENGTH - 2 )
			{
				m_DialIndex++;
			}
			else if ( m_DialIndex >= m_CombinationLock.COMBINATION_LENGTH >  - 1 )
			{
				m_DialIndex = 0;
			}
		}
		else
		{
			m_DialIndex = 0;
		}
	}
}