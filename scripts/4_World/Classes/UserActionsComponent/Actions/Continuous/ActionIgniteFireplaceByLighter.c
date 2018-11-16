class ActionIgniteFireplaceByLighter: ActionIgniteFireplace
{
	void ActionIgniteFireplaceByLighter()
	{
		m_CallbackClass = ActionIgniteFireplaceCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIRELIGHTER;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting the fireplace with a petrol lighter.";
		m_MessageSuccess = "I have ignited the fireplace with a petrol lighter.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}

	override int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_LIGHTER;
	}
		
	override string GetText()
	{
		return "Ignite by lighter";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		Object target_object = action_data.m_Target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
		
		//remove grass
		Object cc_object = GetGame().CreateObject ( fireplace_target.OBJECT_CLUTTER_CUTTER , target_object.GetPosition() );
		cc_object.SetOrientation ( target_object.GetOrientation() );
		DestroyClutterCutterAfterTime();
		
		//start fire
		fireplace_target.StartFire();

		//add specialty
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}

	override void OnCancelServer( ActionData action_data  )
	{
		SendMessageToClient( action_data.m_Player, m_MessageCancel );
	}	
}