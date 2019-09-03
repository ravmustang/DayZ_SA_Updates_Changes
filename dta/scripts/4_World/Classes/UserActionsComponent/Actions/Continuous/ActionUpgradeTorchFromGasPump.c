class ActionUpgradeTorchFromGasPumpCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.ADD_FUEL_TO_TORCH);
	}
};

class ActionUpgradeTorchFromGasPump: ActionContinuousBase
{	
	void ActionUpgradeTorchFromGasPump()
	{
		m_CallbackClass = ActionUpgradeTorchFromGasPumpCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionTarget = new CCTDummy;
		m_ConditionItem = new CCINonRuined;
	}
		
	override string GetText()
	{
		return "#str_upgrade_torch_fuel";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Land_FuelStation_Feed fuelstation = Land_FuelStation_Feed.Cast(target.GetObject());
		Torch torch = Torch.Cast(item);
		
		Rag rag_on_torch = Rag.Cast(  torch.GetRag()  );
		
		if (rag_on_torch  &&  fuelstation  &&  fuelstation.HasFuelToGive() ) // Check if torch already has rag and if vessel has liquid in it
		{
			float quantity = rag_on_torch.GetQuantity();
			
			if (quantity > 1  &&  !torch.IsIgnited())
			{
				return torch.CanReceiveUpgrade();
			}
		}
		
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		Torch torch = Torch.Cast(action_data.m_MainItem);
		
		if (torch)
		{
			torch.ConsumeRag();
			torch.ConsumeFuelFromGasStation();
		}
	}
};