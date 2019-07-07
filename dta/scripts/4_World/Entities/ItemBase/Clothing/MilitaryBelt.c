class MilitaryBelt: Clothing
{
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo( parent ) )
		{
			return false;
		}
		
		if( GetInventory().AttachmentCount() == 0 )
		{
			return true;
		}
		return false;
	}
};
