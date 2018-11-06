class ActionAttachToCharger: ActionSingleUseBase
{
	void ActionAttachToCharger()
	{
		m_MessageSuccess = "I've attached the battery to the charger";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_ATTACH_TO_CHARGER;
	}
		
	override string GetText()
	{
		return "#attach_battery_to_charger";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_EAI;
		if ( Class.CastTo(target_EAI, target.GetObject()) &&  item )
		{
			if ( target_EAI.HasEnergyManager()  &&  target_EAI.GetCompEM().CanReceivePlugFrom(item) )
			{
				ItemBase battery = ItemBase.Cast( target_EAI.GetCompEM().GetPluggedDevice() );
				
				if (battery)
				{
					return false; // Something is already being charged from battery charger
				}
				else
				{
					return true;
				}
			}
		}
		
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		ItemBase target_IB;
		Class.CastTo(target_IB,  action_data.m_Target.GetObject() ); // cast to ItemBase
		target_IB.PredictiveTakeEntityAsAttachment(action_data.m_MainItem);
	}

	override void OnExecuteClient( ActionData action_data )
	{
		ItemBase target_IB;
		Class.CastTo(target_IB,  action_data.m_Target.GetObject() ); // cast to ItemBase
		target_IB.PredictiveTakeEntityAsAttachment(action_data.m_MainItem);
	}
};