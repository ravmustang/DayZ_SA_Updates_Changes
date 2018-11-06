class ActionNextCombinationLockDialOnTarget: ActionInteractBase
{
	protected CombinationLock 	m_CombinationLock;
	protected int 				m_DialIndex;	
	
	void ActionNextCombinationLockDialOnTarget()
	{
		m_DialIndex = 0;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
	}

	override int GetType()
	{
		return AT_NEXT_COMBINATION_LOCK_DIAL_ON_TARGET;
	}

	override string GetText()
	{
		return "#next_combination_lock_dial_on_target";
	}

	override bool IsInstant()
	{
		return true;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			Fence fence = Fence.Cast( targetObject );
			
			if ( fence && fence.IsLocked() )
			{
				string selection = fence.GetActionComponentName( target.GetComponentIndex() );
				
				if ( selection == "wall_interact" )
				{
					m_CombinationLock = fence.GetCombinationLock();
					
					return true;
				}
			}
		}
		
		return false;
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