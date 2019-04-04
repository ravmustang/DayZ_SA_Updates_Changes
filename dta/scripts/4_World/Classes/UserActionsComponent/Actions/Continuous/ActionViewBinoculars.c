class ActionViewBinoculars : ActionViewOptics
{
	bool m_workingRangefinder;
	override int GetType()
	{
		return AT_VIEW_BINOCULARS;
	}
		
	override string GetText()
	{
		if (!m_workingRangefinder)
			return "#Look_Through";
		return "#use_range_finder";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, item) && !optic.IsInOptics() )
		{
			Rangefinder rangefinder = Rangefinder.Cast(item);
			if ( rangefinder && rangefinder.GetCompEM().CanWork() )
			{
				m_workingRangefinder = true;
			}
			else
			{
				m_workingRangefinder = false;
			}
			return true;
		}
		return false;
	}
	
	override void EnterOptics(ItemOptics optic, PlayerBase player)
	{
		player.SetHandheldOpticsInUse(true);
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
		player.SetHandheldOpticsInUse(false);
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