class SlicedPumpkin : Edible_Base
{
	override bool CanBeCooked()
	{
		return true;
	}		
	
	override bool CanBeCookedOnStick()
	{
		return true;
	}	
	
	override bool IsFruit()
	{
		return true;
	}
}