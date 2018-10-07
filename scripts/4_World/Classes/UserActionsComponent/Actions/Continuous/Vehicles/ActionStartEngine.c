enum StartingPhase
{
	BATTERY_CHECK,
	SPARK_CHECK,
	BELT_CHECK,
	FUEL_CHECK,
	FAIL
}


class ActionDataStartVehicle: ActionData
{
	int m_VehicleStart;
}


class ActionStartCarCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeatStartEngine( UATimeSpent.DEFAULT );
	}
}

class ActionStartEngine: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.5;
	
	void ActionStartEngine()
	{
		m_CallbackClass = ActionStartCarCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
		m_LockTargetOnUse = false;
	}
	
	override ActionData CreateActionData()
	{
		ActionDataStartVehicle ad = new ActionDataStartVehicle;
		return ad; 
	}

	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extraData = NULL)
	{
		if ( super.SetupAction(player, target, item, action_data, extraData) )
		{
			ActionDataStartVehicle ad = ActionDataStartVehicle.Cast(action_data);
			ad.m_VehicleStart = StartingPhase.BATTERY_CHECK;
			return true;
		}

		return false;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNone;
		m_ConditionItem = new CCINone;
	}

	override int GetType()
	{
		return AT_VEH_ENGINE_START;
	}
		
	override string GetText()
	{
		return "Start Car";
	}
	
	override bool HasTarget()
	{
		return false;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			
			if ( trans )
			{
				Car car;
				if ( Class.CastTo(car, trans) && !car.IsEngineOn() )
				{
					return true;
				}
			}
		}

		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{	
		ActionDataStartVehicle ad = ActionDataStartVehicle.Cast(action_data);
		//if ( ad.m_VehicleStart != StartingPhase.FAIL )
		//{
			return true;
		//}
		
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		
		ActionDataStartVehicle ad = ActionDataStartVehicle.Cast(action_data);
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			if ( trans )
			{
				CarScript car;
				if ( Class.CastTo(car, trans) )
				{
					switch( ad.m_VehicleStart )
					{
						case StartingPhase.BATTERY_CHECK:
						
							EntityAI battery;
							if ( car.IsVitalCarBattery() ) battery = car.FindAttachmentBySlotName("CarBattery");

							if ( car.IsVitalTruckBattery() ) battery = car.FindAttachmentBySlotName("TruckBattery");

							if ( ( battery && !battery.IsRuined() ) || ( !car.IsVitalCarBattery() && !car.IsVitalTruckBattery() ) )
							{
								ad.m_VehicleStart = StartingPhase.SPARK_CHECK;
							}
							else
							{
								ad.m_VehicleStart = StartingPhase.FAIL;
							}
						break;

						case StartingPhase.SPARK_CHECK:
							EntityAI plug;
							if ( car.IsVitalSparkPlug() ) plug = car.FindAttachmentBySlotName("SparkPlug");

							if ( car.IsVitalGlowPlug() ) plug = car.FindAttachmentBySlotName("GlowPlug");

							if ( ( plug && !plug.IsRuined() ) || ( !car.IsVitalSparkPlug() && !car.IsVitalGlowPlug() ) )
							{
								ad.m_VehicleStart = StartingPhase.BELT_CHECK;
							}
							else
							{
								ad.m_VehicleStart = StartingPhase.FAIL;
							}
						break;

						case StartingPhase.BELT_CHECK:
							EntityAI belt;
							if ( car.IsVitalSparkPlug() ) belt = car.FindAttachmentBySlotName("EngineBelt");
							
							if ( ( belt && !belt.IsRuined() ) || !car.IsVitalSparkPlug() )
							{
								ad.m_VehicleStart = StartingPhase.FUEL_CHECK;
							}
							else
							{
								ad.m_VehicleStart = StartingPhase.FAIL;
							}
						break;

						case StartingPhase.FUEL_CHECK:
							car.Fill( CarFluid.FUEL, 150 );
							float fuel = car.GetFluidFraction( CarFluid.FUEL );	
							if (  fuel > 0 )
							{
								car.EngineStart();
								ad.m_VehicleStart = StartingPhase.FAIL;
							}
							else
							{
								ad.m_VehicleStart = StartingPhase.FAIL;
							}
						break;
					}
				}
			}
		}
	}
/*
	override void OnCompleteLoopServer( ActionData action_data )
	{	
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			
			if ( trans )
			{
				Car car;
				if ( Class.CastTo(car, trans) )
				{
					car.EngineStart();
				}
			}
		}
	}

	override void OnCompleteLoopClient( ActionData action_data )
	{	
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			
			if ( trans )
			{
				Car car;
				if ( Class.CastTo(car, trans) )
				{
					car.EngineStart();
				}
			}
		}
	}
*/
};