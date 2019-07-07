class WaterproofBag_ColorBase extends Container_Base
{
	override bool IsContainer()
	{
		return true;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}	
		if ( !(parent.IsKindOf("WaterproofBag_ColorBase")) )
		{
			return true;
		}
		
		return false;
	}
}

class WaterproofBag_Yellow: WaterproofBag_ColorBase {};
class WaterproofBag_Green: WaterproofBag_ColorBase {};
class WaterproofBag_Orange: WaterproofBag_ColorBase {};