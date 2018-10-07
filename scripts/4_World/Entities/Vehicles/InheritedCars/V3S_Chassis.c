class V3S_Chassis extends CarScript
{
	override int GetAnimInstance()
	{
		return VehicleAnimInstances.V3S;
	}

	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		}

		return 0;
	}

	override bool CrewCanGetThrough( int posIdx )
	{
		CarDoor carDoor;
		switch( posIdx )
		{
			case 0:
				Class.CastTo( carDoor, FindAttachmentBySlotName("V3SDriverDoors") );
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
				Class.CastTo( carDoor, FindAttachmentBySlotName("V3SCoDriverDoors") );
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
		}

		return false;
	}
	
	override bool IsVitalCarBattery()
	{
		return false;
	}

	override bool IsVitalSparkPlug()
	{
		return false;
	}

	override bool IsVitalGlowPlug()
	{
		return false;
	}

	override bool IsVitalEngineBelt()
	{
		return false;
	}
}