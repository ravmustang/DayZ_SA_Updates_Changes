class ActionUnpin extends ActionSingleUseBase
{
	void ActionUnpin()
	{
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

	override string GetText()
	{
		return "#unpin";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsInherited(Grenade_Base) )
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_UNPINGRENAGE;
		}
		else
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_ON;
		}
		
		Grenade_Base grenade = Grenade_Base.Cast(item);
		if( grenade )
		{
			if( grenade.IsPinned() )
			{
				return true;
			}
		}

		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
	}
	
	override void OnStartClient( ActionData action_data )
	{
		//! sound handling ?
	}
	
	override void OnStartServer( ActionData action_data )
	{
		Grenade_Base grenade = Grenade_Base.Cast(action_data.m_MainItem);
		if( grenade )
		{
			grenade.Unpin();
		}
	}
}