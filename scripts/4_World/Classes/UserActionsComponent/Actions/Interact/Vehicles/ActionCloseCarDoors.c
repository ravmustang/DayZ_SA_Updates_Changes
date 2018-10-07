class ActionCloseCarDoors: ActionInteractBase
{
	void ActionCloseCarDoors()
	{
		m_MessageSuccess = "";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		//m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_CLOSE_CAR_DOORS;
	}

	override string GetText()
	{
		return "Close Car door";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !target ) return false;
		//if( IsDamageDestroyed(action_data.m_Target) ) return false;
		//if( !IsTransport(action_data.m_Target) ) return false;
		if( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;

		Car car = Car.Cast(target.GetParent());
		CarDoor carDoor = CarDoor.Cast(target.GetObject());
		if ( carDoor && car )
		{
			if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5 )
				return true;
		}
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		CarDoor carDoor;
		if ( Class.CastTo(carDoor, action_data.m_Target.GetObject()) )
		{
			carDoor.SetAnimationPhase("DoorsSource", 0.0 );
		}
	}

	override void OnStartClient( ActionData action_data )
	{
		CarDoor carDoor;
		if ( Class.CastTo(carDoor, action_data.m_Target.GetObject()) )
		{
			carDoor.SetAnimationPhase("DoorsSource", 0.0);
		}
	}
};