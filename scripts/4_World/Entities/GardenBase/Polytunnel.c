class Land_Misc_Polytunnel extends GardenBase
{
	void Land_Misc_Polytunnel()
	{
		SetBaseFertility(1.1);
	}
	
	override int GetGardenSlotsCount()
	{
		return 13;
	}
}