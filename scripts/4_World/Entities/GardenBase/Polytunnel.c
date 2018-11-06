class Polytunnel extends GardenBase
{
	void Polytunnel()
	{
		m_BaseFertility = 1.0;
		InitializeSlots();
		DigAllSlots();
	}
	
	
	override int GetGardenSlotsCount()
	{
		return 13;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		return false;
	}

	override bool CanPutIntoHands( EntityAI player )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		return false;
	}

	override bool CanRemoveFromHands( EntityAI player )
	{
		return false;
	}
}