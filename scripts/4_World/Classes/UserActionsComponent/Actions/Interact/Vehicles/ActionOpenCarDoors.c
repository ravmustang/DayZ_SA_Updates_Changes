class ActionOpenCarDoors: ActionInteractBase
{
	string m_AnimSource = "";
	
	void ActionOpenCarDoors()
	{
		m_MessageSuccess = "";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		//m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTParent(10);
	}

	override int GetType()
	{
		return AT_OPEN_CAR_DOORS;
	}

	override string GetText()
	{
		return "#open_car_door";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if( !target ) return false;
		//if( IsDamageDestroyed(action_data.m_Target) ) return false;
		//if( !IsTransport(action_data.m_Target) ) return false;
		if( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;
		
		CarScript car;
		if ( Class.CastTo(car, target.GetParent()) )
		{
			array<string> selections = new array<string>();

			CarDoor carDoor = CarDoor.Cast(target.GetObject());
			if (carDoor)
			{
				carDoor.GetActionComponentNameList(target.GetComponentIndex(), selections);
				
				for (int i = 0; i < selections.Count(); i++)
				{
					m_AnimSource = car.GetAnimSourceFromSelection( selections[i]);
					if ( m_AnimSource != "" )
					{
						if ( car.GetAnimationPhase( m_AnimSource ) <= 0.5 )
							return true;
					}
				}
			}
		}
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		Car car = Car.Cast(action_data.m_Target.GetParent());
		if( car )
		{
			car.SetAnimationPhase( m_AnimSource, 1.0);
		}
	}

	override void OnStartClient( ActionData action_data )
	{
		Car car = Car.Cast(action_data.m_Target.GetParent());
		if( car )
		{
			car.SetAnimationPhase( m_AnimSource, 1.0);
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};