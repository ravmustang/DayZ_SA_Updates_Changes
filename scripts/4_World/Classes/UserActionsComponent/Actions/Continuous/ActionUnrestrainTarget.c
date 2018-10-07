class ActionUnrestrainTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.UNRESTRAIN);
	}
};

class ActionUnrestrainTarget: ActionContinuousBase
{
	void ActionUnrestrainTarget()
	{
		m_CallbackClass = ActionUnrestrainTargetCB;
		m_MessageStartFail = "It's broken.";
		m_MessageStart = "Player started unrestraining you using action_data.m_MainItem.";
		m_MessageSuccess = "Player finished unrestraining you using action_data.m_MainItem.";
		m_MessageFail = "Player moved and unrestraining was canceled.";
		m_MessageCancel = "You stopped unrestraining.";
		//m_Animation = "inject";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CUTTIESTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_UNRESTRAIN_T;
	}
		
	override string GetText()
	{
		return "Unrestrain";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		PlayerBase ntarget = PlayerBase.Cast(action_data.m_Target.GetObject());
		//restraintarget
		action_data.m_MainItem.Delete();
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};