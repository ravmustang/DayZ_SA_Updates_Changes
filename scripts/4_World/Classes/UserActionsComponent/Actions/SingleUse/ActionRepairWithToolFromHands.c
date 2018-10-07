// This is just a test action!
class ActionRepairWithToolFromHands: ActionSingleUseBase
{
	void ActionRepairWithToolFromHands()
	{
		Print("ActionRepairWithToolFromHands");
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		
		//m_Animation = "open";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return 0; //AT_REPAIR;
	}

	override string GetText()
	{
		return "#repair";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject /*&&  targetObject.IsKindOf("ItemBase")*/ )
		{
			ItemBase item_to_repair = ItemBase.Cast( targetObject );
			bool can_repair = item_to_repair.CanRepair(item);
			Print(can_repair);
			return can_repair;
		}
		
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		if ( targetObject  /* &&  targetObject.IsKindOf("ItemBase")*/ )
		{
			ItemBase item_to_repair = ItemBase.Cast( targetObject );
			Param1<float> nacdata = Param1<float>.Cast( action_data.m_ActionComponent.GetACData() );
			bool was_repaired = item_to_repair.Repair(action_data.m_Player, action_data.m_MainItem, nacdata.param1);
			Print(was_repaired);
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
