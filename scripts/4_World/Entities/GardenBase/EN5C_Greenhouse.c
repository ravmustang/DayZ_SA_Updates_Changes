class Land_Misc_Greenhouse extends GardenBase
{
	void Land_Misc_Greenhouse()
	{
		SetBaseFertility(1.0);
	}
	
	override int GetGardenSlotsCount()
	{
		return 9;
	}
}