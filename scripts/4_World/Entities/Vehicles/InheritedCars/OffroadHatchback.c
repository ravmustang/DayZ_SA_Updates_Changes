class OffroadHatchback extends CarScript
{
	override int GetAnimInstance()
	{
		return VehicleAnimInstances.HATCHBACK;
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
		CarDoor carDoor;
		switch( posIdx )
		{
			case 0:
				Class.CastTo( carDoor, FindAttachmentBySlotName("NivaDriverDoors") );
				if ( carDoor )
				{
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5 ) return true;
				}
				else
				{
					return true;
				}
				return false;
			break;
			
			case 1:
				Class.CastTo( carDoor, FindAttachmentBySlotName("NivaCoDriverDoors") );
				if ( carDoor )
				{
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5 ) return true;
				}
				else
				{
					return true;
				}
				return false;
			break;

			case 2:
				Class.CastTo( carDoor, FindAttachmentBySlotName("NivaDriverDoors") );
				if ( carDoor )
				{
					//test the front seat animation
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5 ) return true;
				}
				else
				{
					return true;
				}
				return false;
			break;

			case 3:
				Class.CastTo( carDoor, FindAttachmentBySlotName("NivaCoDriverDoors") );
				if ( carDoor )
				{
					//test the front seat animation	
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5 ) return true;
				}
				else
				{
					return true;
				}
				return false;
			break;
		}

		return false;
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