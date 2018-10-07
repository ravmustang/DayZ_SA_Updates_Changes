class ActionTurnOnPowerGenerator: ActionInteractBase
{	
	void ActionTurnOnPowerGenerator()
	{
		m_MessageSuccess = "I've started the generator.";
		m_MessageStart = "";
	}	
	
	override int GetType()
	{
		return AT_TURN_ON_POWER_GENERATOR;
	}

	override string GetText()
	{
		return "Switch on";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		PowerGenerator target_PG = PowerGenerator.Cast( targetObject );

		float c_index = target.GetComponentIndex();		
		string selection = targetObject.GetActionComponentName(c_index);
		
		if ( target_PG.HasEnergyManager()  &&  target_PG.GetCompEM().CanSwitchOn()  &&  selection == "switch"  &&  target_PG.HasSparkplug() )
		{
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		if (targetObject)
		{
			ItemBase target_IB = ItemBase.Cast( targetObject );
			target_IB.GetCompEM().SwitchOn();
		}
	}
};