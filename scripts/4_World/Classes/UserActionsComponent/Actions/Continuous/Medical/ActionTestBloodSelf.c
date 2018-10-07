class ActionTestBloodSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.TEST_BLOOD);
	}
};

class ActionTestBloodSelf: ActionContinuousBase
{
	void ActionTestBloodSelf()
	{
		m_CallbackClass = ActionTestBloodSelfCB;
		m_MessageStartFail = "It's used up.";
		m_MessageStart = "I have started testing my blood.";
		m_MessageSuccess = "I have tested my blood.";
		m_MessageFail = "I have moved and testing was canceled.";
		m_MessageCancel = "I stopped testing my blood.";
		//m_Animation = "INJECTS";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_TEST_BLOOD_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Test blood";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		int blood_type = action_data.m_Player.GetStatBloodType().Get();
		
		module_lifespan.UpdateBloodType( action_data.m_Player, blood_type );
		module_lifespan.UpdateBloodTypeVisibility( action_data.m_Player, true );
		
		action_data.m_MainItem.Delete();
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};