class CivilianSedan extends CarScript
{
	override int GetAnimInstance()
	{
		return VehicleAnimInstances.SEDAN;
	}

	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		case 2:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 3:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		}

		return 0;
	}

	override bool CrewCanGetThrough( int posIdx )
	{
		switch( posIdx )
		{
			case 0:
				if ( GetCarDoorsState( "CivSedanDriverDoors" ) == CarDoorState.DOORS_CLOSED )
					return false;

				return true;
			break;
			
			case 1:
				if ( GetCarDoorsState( "CivSedanCoDriverDoors" ) == CarDoorState.DOORS_CLOSED )
					return false;

				return true;
			break;

			case 2:
				if ( GetCarDoorsState( "CivSedanCargo1Doors" ) == CarDoorState.DOORS_CLOSED )
					return false;

				return true;
			break;

			case 3:
				if ( GetCarDoorsState( "CivSedanCargo2Doors" ) == CarDoorState.DOORS_CLOSED )
					return false;

				return true;
			break;
		}

		return false;
	}
	
	
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		switch ( ctrl )
		{
			case CarSoundCtrl.DOORS:
				float newValue = 0;
				
				//-----
				CarDoor carDoor;
			 	Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanDriverDoors") );
				if ( carDoor )
				{	
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5)
					{
						newValue = newValue + 0.7;
					}
				}
				else
				{
					newValue = newValue + 0.7;
				}

				//-----

			 	Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCoDriverDoors") );
				if ( carDoor )
				{	
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5)
					{
						newValue = newValue + 0.7;
					}
				}
				else
				{
					newValue = newValue + 0.7;
				}
			
				//-----
			 	Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCargo1Doors") );
				if ( carDoor )
				{	
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5)
					{
						newValue = newValue + 0.7;
					}
				}
				else
				{
					newValue = newValue + 0.7;
				}

				//-----
				Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCargo2Doors") );
				if ( carDoor )
				{	
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5)
					{
						newValue = newValue + 0.7;
					}
				}
				else
				{
					newValue = newValue + 0.7;
				}

				if ( newValue > 1 )
					newValue = 1;
			
				return newValue;
			break;
		}

		return oldValue;
	}
	
	override string GetAnimSourceFromSelection( string selection )
	{
		switch( selection )
		{
		case "doors_driver":
			return "DoorsDriver";
		case "doors_codriver":
			return "DoorsCoDriver";
		case "doors_cargo1":
			return "DoorsCargo1";
		case "doors_cargo2":
			return "DoorsCargo2";
		case "doors_hood":
			return "DoorsHood";
		case "doors_trunk":
			return "DoorsTrunk";
		}

		return "";
	}


	override bool IsVitalTruckBattery()
	{
		return false;
	}

	override bool IsVitalGlowPlug()
	{
		return false;
	}
}