class ActionRestrainTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.RESTRAIN);
	}
};

class ActionRestrainTarget: ActionContinuousBase
{	
	void ActionRestrainTarget()
	{
		m_CallbackClass = ActionRestrainTargetCB;
		m_MessageStartFail = "Restraints are damage.";
		m_MessageStart = "Player started restraining you.";
		m_MessageSuccess = "Player finished restraining you.";
		m_MessageFail = "Player moved and restraining was canceled.";
		m_MessageCancel = "You stopped restraining.";
		//m_Animation = "restrain";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_TIEUPTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Sound = "action_handcuff_0";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_RESTRAIN_T;
	}
		
	override string GetText()
	{
		return "Restrain";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		PlayerBase ntarget = PlayerBase.Cast(action_data.m_Target.GetObject());
		//ntarget.SetCaptive();
		action_data.m_MainItem.Delete();

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};