class ActionDisinfectSelfCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CASingleUseQuantity(UAQuantityConsumed.DISINFECT);
	}
};


class ActionDisinfectSelf: ActionSingleUseBase
{
	float m_GramsConsumedPerUse;
	
	void ActionDisinfectSelf()
	{
		m_CallbackClass = ActionDisinfectSelfCB;
		m_MessageSuccess = "I disinfected myself";
		m_MessageStartFail = "Its' empty";
		//m_Animation = "lick";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_DISINFECT_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "#disinfect";
	}

	override void OnCompleteServer( ActionData action_data )
	{
		//RemoveModifiers(target, item); ?

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		if ( action_data.m_MainItem && action_data.m_MainItem.GetQuantity() <= 0 ) 
		{
			action_data.m_MainItem.SetQuantity(0);
		}
	}
};