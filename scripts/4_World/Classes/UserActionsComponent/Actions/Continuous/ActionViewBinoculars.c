class ActionViewBinoculars : ActionViewOptics
{
	override int GetType()
	{
		return AT_VIEW_BINOCULARS;
	}
		
	override string GetText()
	{
		return "#use_binoculars";
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
				rf.SetPlayer( player );
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
				rf.SetPlayer( null );
			}
			else
			{
				Rangefinder rf_client = Rangefinder.Cast(optic);		
				rf_client.SetPlayer(NULL);
			}
		}
	}
}