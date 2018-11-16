class ActionIgniteFireplaceHandDrillCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT_CHECK = 2;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTimeIgnite( UATimeSpent.FIREPLACE_HANDDRILL, TIME_TO_REPEAT_CHECK );
	}
}

class ActionIgniteFireplaceByHandDrill: ActionIgniteFireplace
{	
	void ActionIgniteFireplaceByHandDrill()
	{
		m_CallbackClass = ActionIgniteFireplaceHandDrillCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIREDRILL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;		
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting with hand drill.";
		m_MessageSuccess = "I have ignited the fireplace with a hand drill.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
	}

	override int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_HANDDRILL;
	}
		
	override string GetText()
	{
		return "Ignite by hand drill";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		Object target_object = action_data.m_Target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
				
		//add damage to hand drill
		action_data.m_MainItem.DecreaseHealth( "", "", m_HandDrillDamagePerUse, true );
		
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