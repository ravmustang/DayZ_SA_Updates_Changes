class ActionDefibrilateSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFIBRILATE);
	}
};

class ActionDefibrilateSelf: ActionContinuousBase
{	
	void ActionDefibrilateSelf()
	{
		m_CallbackClass = ActionDefibrilateSelfCB;
		m_MessageStartFail = "It's out of juice.";
		m_MessageStart = "I have started defibrilating myself";
		m_MessageSuccess = "I have defibrilated myself.";
		m_MessageFail = "I have moved and defibrilating was canceled.";
		m_MessageCancel = "I stopped defibrilating.";
		//m_Animation = "defibrilate";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_DEFIBRILATE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Defibrilate yourself";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Defibrillator defib;
		Class.CastTo(defib, item);
		
		if ( defib.IsCharged() ) 
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
		Defibrillator defib;
		Class.CastTo(defib, action_data.m_MainItem);
		
		if ( defib.IsCharged() ) 
		{
			defib.Discharge(action_data.m_Player);
			
			float regain_energy = action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( defib.GetEnergyNeededToCharge(), this.GetSpecialtyWeight() );
			regain_energy = defib.GetEnergyNeededToCharge() - regain_energy;
			
			ItemBase battery;
			
			if (Class.CastTo(battery, action_data.m_MainItem.GetCompEM().GetEnergySource()))
			{
				battery.GetCompEM().AddEnergy( regain_energy );
			}
			else
			{
				DPrint("ERROR! Defibrillator has no battery! Defibrillator softskill bonus can't be applied!");
			}
		}
		else
		{
			action_data.m_Player.MessageImportant ( m_MessageStartFail );		
		}
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
