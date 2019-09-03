class BurlapSackCover extends ClothingBase
{
	PlayerBase m_Player;

	void ~BurlapSackCover()
	{
		if( m_Player )
		{
			OnRemovedFromHead(m_Player);
		}
	}

	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		Class.CastTo(m_Player, parent.GetHierarchyRootPlayer());
		//bool selected = player.IsPlayerSelected();//this is 0 upon player's connection to server

		if ( (GetGame().IsClient() || !GetGame().IsMultiplayer()) && m_Player && m_Player.IsControlledPlayer() && slot_id == InventorySlots.HEADGEAR )
		{
			//GetGame().GetWorld().SetAperture(100000);
			PPEffects.Init();
			PPEffects.EnableBurlapSackBlindness();
			m_Player.SetInventorySoftLock(true);
			m_Player.SetMasterAttenuation("BurlapSackAttenuation");
			
			if ( GetGame().GetUIManager().IsMenuOpen(MENU_INVENTORY) )
			{
				GetGame().GetUIManager().FindMenu(MENU_INVENTORY).Close();
			}
		}
		SetInvisibleRecursive(true,m_Player,{InventorySlots.MASK,InventorySlots.EYEWEAR});
	}

	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		if ( GetGame().IsServer() )
		{
			PlayerBase player;
			Class.CastTo(player, parent.GetHierarchyRootPlayer());
			MiscGameplayFunctions.TurnItemIntoItem(this, "BurlapSack", player);
		}
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( parent != this )
		{
			return true;
		}
		return false;
	}

	override bool CanDetachAttachment( EntityAI parent )
	{
		PlayerBase player;
		Class.CastTo(player, GetHierarchyRootPlayer());
		return (player && !player.IsAlive());
	}

	void OnRemovedFromHead(PlayerBase player)
	{
		if ( player.IsControlledPlayer() )
		{
			PPEffects.Init();
			PPEffects.DisableBurlapSackBlindness();
			//GetGame().GetWorld().SetAperture(0);
			player.SetInventorySoftLock(false);
			player.SetMasterAttenuation("");
		}
		SetInvisibleRecursive(false,player,{InventorySlots.MASK,InventorySlots.EYEWEAR});
	}
}