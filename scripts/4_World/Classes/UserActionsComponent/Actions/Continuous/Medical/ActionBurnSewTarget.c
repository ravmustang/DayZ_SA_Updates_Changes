class ActionBurnSewTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_WOUNDS);
	}
};

class ActionBurnSewTarget: ActionContinuousBase
{	
	void ActionBurnSewTarget()
	{
		m_CallbackClass = ActionBurnSewTargetCB;
		m_MessageStartFail = "It's not possible.";
		m_MessageStart = "Player started closing your wounds.";
		m_MessageSuccess = "Player finished closing your wounds.";
		m_MessageFail = "Player moved and closing wounds was canceled.";
		m_MessageCancel = "You stopped closing target's wounds.";
	//	m_Animation = "sew";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_MEDIUM;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_BURN_SEW_T;
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
		
	override string GetText()
	{
		return "Close wounds";
	}

	override void OnCompleteServer( ActionData action_data )
	{	
		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );
		ntarget.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_BLEEDING);
		//OlD_SHOCK//ntarget.GetStatShock().Add(1000);
		action_data.m_MainItem.DecreaseHealth ( "", "", 5 );

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};