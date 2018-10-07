class ActionInjectTarget: ActionSingleUseBase
{
	void ActionInjectTarget()
	{
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "Player started injecting you.";
		m_MessageSuccess = "Player finished injecting you.";
		m_MessageFail = "Player moved and injecting was canceled.";
		m_MessageCancel = "You stopped injecting.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INJECTIONTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_INJECT_T;
	}
		
	override string GetText()
	{
		return "#inject_target";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		action_data.m_MainItem.Delete();
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};