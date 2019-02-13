class AmanitaMushroom : Edible_Base
{
	override bool CanBeCookedOnStick()
	{
		return true;
	}

	override bool CanBeCooked()
	{
		return true;
	}	
	
	override bool IsMushroom()
	{
		return true;
	}
}
