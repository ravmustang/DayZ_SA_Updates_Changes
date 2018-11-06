class ActionBandageSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.BANDAGE);
	}
};

class ActionBandageSelf: ActionContinuousBase
{	
	void ActionBandageSelf()
	{
		m_CallbackClass = ActionBandageSelfCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		
		m_MessageStartFail = "There's nothing to bandage.";
		m_MessageStart = "I have started bandaging myself";
		m_MessageSuccess = "I have bandaged myself.";
		m_MessageFail = "I have moved and bandaging was canceled.";
		m_MessageCancel = "I stopped bandaging.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}

	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}
		
	override int GetType()
	{
		return AT_BANDAGE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "#bandage";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return player.IsBleeding();
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		if (action_data.m_Player.GetBleedingManagerServer() )
		{
			action_data.m_Player.GetBleedingManagerServer().RemoveAnyBleedingSource();	
		}
		
		if (action_data.m_MainItem.GetQuantity() > 0)
		{
			action_data.m_MainItem.AddQuantity(-1,true);
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
