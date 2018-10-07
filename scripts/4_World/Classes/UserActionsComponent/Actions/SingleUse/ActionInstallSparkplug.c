/*
	This is a generic user action for attaching sparkplug on any EntityAI object. The receiver must have a "sparkplug" selection in its View Geometry that the action_data.m_Player can look at.
*/


class ActionInsertSparkplug: ActionSingleUseBase
{
	void ActionInsertSparkplug()
	{
		m_MessageSuccess = "I've inserted the sparkplug.";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_INSERT_SPARKPLUG;
	}
		
	override string GetText()
	{
		return "#insert_sparkplug";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_EAI = EntityAI.Cast( target.GetObject() );
		string selection = target_EAI.GetActionComponentName(target.GetComponentIndex());
		
		if ( selection == "sparkplug"  &&  target_EAI.GetInventory().CanAddAttachment(item) )
		{
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		EntityAI target_EAI = EntityAI.Cast( action_data.m_Target.GetObject() ); // cast to ItemBase
		target_EAI.LocalTakeEntityAsAttachment (action_data.m_MainItem);
	}
	override void OnCompleteClient( ActionData action_data )
	{
		EntityAI target_EAI = EntityAI.Cast( action_data.m_Target.GetObject() ); // cast to ItemBase
		target_EAI.LocalTakeEntityAsAttachment (action_data.m_MainItem);
	}
};