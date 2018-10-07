class ActionTogglePlaceObject: ActionSingleUseBase
{

	void ActionTogglePlaceObject()
	{
		/*
		m_CommandUID        	= DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask       		= DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail 		= "";
		m_MessageStart 			= "";
		m_MessageSuccess 		= "";
		m_MessageFail 			= "";
		*/
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
		
	override int GetType()
	{
		return AT_TOGGLE_PLACE_OBJECT;
	}

	override bool HasTarget()
	{
		return false;
	}
	
	override bool IsLocal()
	{
		return true;
	}
	
	override bool IsInstant()
	{
		return true;
	}
	
	override bool RemoveForceTargetAfterUse()
	{
		return false;
	}

	override string GetText()
	{
		return "#toggle_placing";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		// not placeable if liquid is present; exception for barrels, for now
		if ( !item.IsKindOf("Barrel_ColorBase") && item.IsLiquidPresent() )
		{
			return false;
		}
		
		return true;
	}
	
	override void Start( ActionData action_data ) //Setup on start of action
	{
		action_data.m_Player.TogglePlacingLocal();
	}
};