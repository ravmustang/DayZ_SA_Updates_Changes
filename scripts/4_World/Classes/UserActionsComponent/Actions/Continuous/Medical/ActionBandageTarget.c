class ActionBandageTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.BANDAGE);
	}
};

class ActionBandageTarget: ActionContinuousBase
{
	void ActionBandageTarget()
	{
		m_CallbackClass = ActionBandageTargetCB;
		m_MessageStartFail = "There's no bandage left.";
		m_MessageStart = "Player started bandaging you.";
		m_MessageSuccess = "Player finished bandaging you.";
		m_MessageFail = "Player moved and bandaging was canceled.";
		m_MessageCancel = "You stopped bandaging.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
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
		return AT_BANDAGE_T;
	}
		
	override string GetText()
	{
		return "Bandage target";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		PlayerBase ntarget = PlayerBase.Cast(action_data.m_Target.GetObject());
		if (ntarget.GetBleedingManager() )
		{
			ntarget.GetBleedingManager().RemoveSingleBleedingSource();
		}
		
		if (action_data.m_MainItem.GetQuantity() > 0)
		{
			action_data.m_MainItem.AddQuantity(-1,true);
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
