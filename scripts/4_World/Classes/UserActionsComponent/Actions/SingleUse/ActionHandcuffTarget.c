class ActionHandcuffTarget: ActionSingleUseBase
{
	void ActionHandcuffTarget()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HANDCUFFTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Sound = "action_handcuff_0";
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_HANDCUFF_T;
	}
		
	override string GetText()
	{
		return "#handcuff";
	}
	
	override void OnCompleteServer( ActionData action_data )
	{	
		PlayerBase ntarget = PlayerBase.Cast(action_data.m_Target.GetObject());
		//ntarget.SetCaptive();
		action_data.m_MainItem.Delete();

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};