class PickAxe extends ItemBase
{
	override bool CanMakeGardenplot()
	{
		return true;
	}

	override bool IsMeleeFinisher()
	{
		return true;
	}
}	