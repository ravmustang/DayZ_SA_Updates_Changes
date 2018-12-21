class ChernarusMap_Open extends ItemMap
{
	protected bool m_ItemChanged;
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner,new_owner);
		
		PlayerBase player = PlayerBase.Cast(old_owner);
		if (player && !m_ItemChanged && GetGame().IsServer())
		{
			MiscGameplayFunctions.TurnItemIntoItem(this, "ChernarusMap", player); //changes map on player death/drop
			m_ItemChanged = true;
		}
	}
	
	/*override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		MiscGameplayFunctions.TurnItemIntoItem(this, "ChernarusMap", player); //changes map on player death/drop
	}*/
}