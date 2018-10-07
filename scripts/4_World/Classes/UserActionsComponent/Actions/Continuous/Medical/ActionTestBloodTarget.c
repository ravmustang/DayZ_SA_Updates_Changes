class ActionTestBloodTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.TEST_BLOOD);
	}
};

class ActionTestBloodTarget: ActionContinuousBase
{	
	void ActionTestBloodTarget()
	{
		m_CallbackClass = ActionTestBloodTargetCB;
		m_MessageStartFail = "It's used up.";
		m_MessageStart = "Player started testing your blood.";
		m_MessageSuccess = "Player finished testing your blood.";
		m_MessageFail = "Player moved and testing was canceled.";
		m_MessageCancel = "You stopped testing.";
	//	m_Animation = "testblood";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_TEST_BLOOD_T;
	}
		
	override string GetText()
	{
		return "Test blood of target";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		PlayerBase ntraget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		int blood_type = ntraget.GetStatBloodType().Get();
		
		module_lifespan.UpdateBloodTypeVisibility( ntraget, true );
		module_lifespan.UpdateBloodType( ntraget, blood_type );
		
		action_data.m_MainItem.Delete();
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};