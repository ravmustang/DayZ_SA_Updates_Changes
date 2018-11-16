class ActionIgniteFireplaceByMatch: ActionIgniteFireplace
{
	
	void ActionIgniteFireplaceByMatch()
	{
		m_CallbackClass = ActionIgniteFireplaceCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIREMATCH;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
 		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting the fireplace with a match.";
		m_MessageSuccess = "I have ignited the fireplace with a match.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINotEmpty;
	}

	override int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_MATCH;
	}
		
	override string GetText()
	{
		return "Ignite by match";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		Object target_object = action_data.m_Target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
		
		//remove match
		action_data.m_MainItem.AddQuantity ( -m_MatchSpentPerUse );
		
		//remove grass
		clutter_cutter = GetGame().CreateObject ( fireplace_target.OBJECT_CLUTTER_CUTTER , target_object.GetPosition() );
		clutter_cutter.SetOrientation ( target_object.GetOrientation() );
		DestroyClutterCutterAfterTime();
		
		//start fire
		fireplace_target.StartFire();

		//add specialty
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}