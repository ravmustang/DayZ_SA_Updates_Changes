class ActionDefibrilateTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFIBRILATE);
	}
};

class ActionDefibrilateTarget: ActionContinuousBase
{	
	void ActionDefibrilateTarget()
	{
		m_CallbackClass = ActionDefibrilateTargetCB;
		m_MessageStartFail = "It's out of juice.";
		m_MessageStart = "Player started defibrilating you.";
		m_MessageSuccess = "Player finished defibrilating you.";
		m_MessageFail = "Player moved and defibrilating was canceled.";
		m_MessageCancel = "You stopped defibrilating.";
		//m_Animation = "defibrilate";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_DEFIBRILATE_T;
	}
	
	override string GetText()
	{
		return "Defibrilate target";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Defibrillator defib = Defibrillator.Cast( item );
		
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
		Defibrillator defib = Defibrillator.Cast( action_data.m_MainItem );
		PlayerBase target_player = PlayerBase.Cast( action_data.m_Target.GetObject() );
		
		if ( target_player )
		{	
			if ( defib.IsCharged() ) 
			{
				defib.Discharge(target_player);
				
				float regain_energy = action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( defib.GetEnergyNeededToCharge(), this.GetSpecialtyWeight() );
				regain_energy = defib.GetEnergyNeededToCharge() - regain_energy;
				
				ItemBase battery = ItemBase.Cast( action_data.m_MainItem.GetCompEM().GetEnergySource() );
				
				if (battery)
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
		}
		else
		{
			action_data.m_Player.MessageImportant ( m_MessageFail );
		}
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
