class BurlapSackCover extends ClothingBase
{
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		PlayerBase player;
		Class.CastTo(player, parent.GetHierarchyRootPlayer());
		//bool selected = player.IsPlayerSelected();//this is 0 upon player's connection to server

		if ( (GetGame().IsClient() || !GetGame().IsMultiplayer()) && player && player.IsControlledPlayer() && slot_id == InventorySlots.HEADGEAR )
		{
			//GetGame().GetWorld().SetAperture(100000);
			PPEffects.Init();
			PPEffects.EnableBurlapSackBlindness();
			GetGame().GetSoundScene().SetSoundVolume((g_Game.m_volume_sound/2),1);
			GetGame().GetSoundScene().SetSpeechExVolume((g_Game.m_volume_speechEX/2),1);
			GetGame().GetSoundScene().SetMusicVolume((g_Game.m_volume_music/2),1);
			GetGame().GetSoundScene().SetVOIPVolume((g_Game.m_volume_VOIP/2),1);
			GetGame().GetSoundScene().SetRadioVolume((g_Game.m_volume_radio/2),1);
			player.m_InventorySoftLocked = true;
			
			if ( GetGame().GetUIManager().IsMenuOpen(MENU_INVENTORY) )
			{
				GetGame().GetUIManager().FindMenu(MENU_INVENTORY).Close();
			}
		}
	}

	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		PlayerBase player;
		Class.CastTo(player, parent.GetHierarchyRootPlayer());
		if ( (GetGame().IsClient() || !GetGame().IsMultiplayer()) && player && player.IsControlledPlayer() && slot_id == InventorySlots.HEADGEAR )
		{
			PPEffects.Init();
			PPEffects.DisableBurlapSackBlindness();
			GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound,1);
			GetGame().GetSoundScene().SetSpeechExVolume(g_Game.m_volume_speechEX,1);
			GetGame().GetSoundScene().SetMusicVolume(g_Game.m_volume_music,1);
			GetGame().GetSoundScene().SetVOIPVolume(g_Game.m_volume_VOIP,1);
			GetGame().GetSoundScene().SetRadioVolume(g_Game.m_volume_radio,1);
			//GetGame().GetWorld().SetAperture(0);
			player.m_InventorySoftLocked = false;
		}
		// should change item to non-attachable one
		if (GetGame().IsServer() && player && slot_id == InventorySlots.HEADGEAR)
		{
			MiscGameplayFunctions.TurnItemIntoItem(ItemBase.Cast( this ), "BurlapSack", player);
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
		return false;
	}
}