class ActionBurnSewSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_WOUNDS);
	}
};

class ActionBurnSewSelf: ActionContinuousBase
{
	void ActionBurnSewSelf()
	{
		m_CallbackClass = ActionBurnSewSelfCB;
		m_MessageStartFail = "It's not possible.";
		m_MessageStart = "I have started closing my wounds";
		m_MessageSuccess = "I have closed my wounds.";
		m_MessageFail = "I have moved and healing was canceled.";
		m_MessageCancel = "I stopped closing my wounds.";
		//m_Animation = "bandage";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_BURN_SEW_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Close wounds";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( item.GetTemperature() > 80 ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		const float ITEM_DAMAGE = 0.05;
		const float SHOCK_AMOUNT = 1000;
		action_data.m_MainItem.DecreaseHealth ( "", "", action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( ITEM_DAMAGE, this.GetSpecialtyWeight() )*100 );
		action_data.m_Player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_BLEEDING);		
		//OlD_SHOCK//action_data.m_Player.GetStatShock().Add( action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( SHOCK_AMOUNT, this.GetSpecialtyWeight() ) );
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};