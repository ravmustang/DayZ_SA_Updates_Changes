class CCINotRuinedAndDry extends CCIBase
{
	override bool Can( PlayerBase player, ItemBase item )
	{
		if ( item && !item.IsDamageDestroyed() && item.GetWet() <= 0 ) 
		{
			return true;
		}
		return false;
	}
};