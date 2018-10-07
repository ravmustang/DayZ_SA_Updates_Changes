class ActionViewBinoculars : ActionViewOptics
{
	void ActionViewBinoculars()
	{
		m_CallbackClass = ActionRaiseAndViewCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LOOKOPTICS;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_LOOKOPTICS;
	}
	
	override int GetType()
	{
		return AT_VIEW_BINOCULARS;
	}
		
	override string GetText()
	{
		return "Use binoculars";
	}
	
	override void EnterOptics(ItemOptics optic, PlayerBase player)
	{
		optic.EnterOptics();
		optic.HideSelection("hide");
		if ( optic.HasEnergyManager() )
		{
			if ( GetGame().IsServer() )
			{
				Rangefinder rf = Rangefinder.Cast(optic);				
				rf.GetCompEM().SwitchOn();
			}
			else
			{
				Rangefinder rf_client = Rangefinder.Cast(optic);		
				rf_client.SetPlayer(player);
			}
		}
	}
	
	override void ExitOptics(ItemOptics optic, PlayerBase player)
	{
		optic.ShowSelection("hide");
		optic.ExitOptics();
		if ( optic.HasEnergyManager() )
		{
			if ( GetGame().IsServer() )
			{
				Rangefinder rf = Rangefinder.Cast(optic);
				rf.GetCompEM().SwitchOff();
			}
			else
			{
				Rangefinder rf_client = Rangefinder.Cast(optic);		
				rf_client.SetPlayer(NULL);
			}
		}
	}
}