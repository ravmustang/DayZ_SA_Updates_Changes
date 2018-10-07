class ActionWorldCraftSwitch: ActionSingleUseBase
{
	void ActionWorldCraftSwitch()
	{
		/*
		m_MessageStartFail = "Start failed.";
		m_MessageStart = "Player started placing object.";
		m_MessageSuccess = "Player placed the object."
		m_MessageFail = "Player failed to place the object.";
		m_MessageCancel = "You canceled action.";
		*/
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_WORLD_CRAFT_SWITCH;
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
		return "#next_recipe";
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if( player.GetCraftingManager().GetRecipesCount() > 1 )
		{
			return true;
		}
		return false;
	}
	
	override void Start( ActionData action_data ) //Setup on start of action
	{
		action_data.m_Player.GetCraftingManager().SetNextRecipe();
	}
	
};