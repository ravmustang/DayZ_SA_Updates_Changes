class ActionMeasureBattery: ActionSingleUseBase
{

	void ActionMeasureBattery()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_LICKBATTERY;
		m_MessageSuccess = "There's still some juice in it.";
		m_MessageFail = "It's dead.";
		m_MessageStartFail = "It's dead.";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override int GetType()
	{
		return AT_MEASURE_BATTERY;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#lick_battery";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		
		float energy = 0;
		if ( action_data.m_MainItem.HasEnergyManager() ) 
		{
			energy = action_data.m_MainItem.GetCompEM().GetEnergy();
		}
		action_data.m_MainItem.TransferModifiers(action_data.m_Player);
		if(energy > 0) 
		{
			InformPlayers(action_data.m_Player,action_data.m_Target,UA_FINISHED);
		}
		if(energy == 0) 
		{
			InformPlayers(action_data.m_Player,action_data.m_Target,UA_FAILED);
		}
	}
};