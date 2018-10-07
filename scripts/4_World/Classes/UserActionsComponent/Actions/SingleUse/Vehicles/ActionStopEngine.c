class ActionStopEngine: ActionSingleUseBase
{
	void ActionStopEngine()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		//m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}	
	
	override int GetType()
	{
		return AT_VEH_ENGINE_STOP;
	}

	override string GetText()
	{
		return "Stop engine";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		HumanCommandVehicle vehCmd = player.GetCommand_Vehicle();
		Car car;
		
		if ( vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
		{
			Transport trans = vehCmd.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo(car, trans) &&  car.IsEngineOn() )
				{
					return true;
				}
			}
		}
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		HumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_Vehicle();
		Car car;

		if ( vehCmd )
		{
			Transport trans = vehCmd.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo(car, trans) )
				{
					car.Leak( CarFluid.FUEL, 21);
					car.EngineStop();
				}
			}
		}	
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		HumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_Vehicle();
		Car car;

		if ( vehCmd )
		{
			Transport trans = vehCmd.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo(car, trans) )
				{
					car.EngineStop();
				}
			}
		}
	}
};