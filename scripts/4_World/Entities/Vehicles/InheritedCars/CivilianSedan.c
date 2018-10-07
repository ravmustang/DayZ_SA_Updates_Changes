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
		CarDoor carDoor;
		switch( posIdx )
		{
			case 0:
				Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanDriverDoors") );
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
				Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCoDriverDoors") );
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
				Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCargo1Doors") );
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

			case 3:
				Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCargo2Doors") );
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
						newValue = newValue + 0.25;
					}
				}
				else
				{
					newValue = newValue + 0.25;
				}

				//-----

			 	Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCoDriverDoors") );
				if ( carDoor )
				{	
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5)
					{
						newValue = newValue + 0.25;
					}
				}
				else
				{
					newValue = newValue + 0.25;
				}
			
				//-----
			 	Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCargo1Doors") );
				if ( carDoor )
				{	
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5)
					{
						newValue = newValue + 0.25;
					}
				}
				else
				{
					newValue = newValue + 0.25;
				}

				//-----
				Class.CastTo( carDoor, FindAttachmentBySlotName("CivSedanCargo2Doors") );
				if ( carDoor )
				{	
					if ( carDoor.GetAnimationPhase("DoorsSource") > 0.5)
					{
						newValue = newValue + 0.25;
					}
				}
				else
				{
					newValue = newValue + 0.25;
				}

				return newValue;
			break;
		}

		return oldValue;
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