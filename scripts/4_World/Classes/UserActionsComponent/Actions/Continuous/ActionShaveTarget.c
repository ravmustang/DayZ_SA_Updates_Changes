class ActionShaveTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SHAVE);
	}
};

class ActionShaveTarget: ActionContinuousBase
{
	void ActionShaveTarget()
	{
		m_CallbackClass = ActionShaveTargetCB;
		m_MessageStartFail = "There's no beard to be shaved.";
		m_MessageStart = "I have started shaving target.";
		m_MessageSuccess = "I have shaved target.";
		m_MessageFail = "I have failed to shave target.";
		m_MessageCancel = "I stopped shaving target.";
		//m_Animation = "ignite";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		PlayerBase man;
		Class.CastTo(man,  target.GetObject() );
		
		if ( man.CanShave() )
		{		
			return true;
		}

		return false;
	}

	override int GetType()
	{
		return AT_SHAVE_T;
	}
		
	override string GetText()
	{
		return "Shave target";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		PlayerBase man;
		if ( action_data.m_Target && Class.CastTo(man, action_data.m_Target.GetObject()) )
		{
			man.ShavePlayer();
			action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
	}
};