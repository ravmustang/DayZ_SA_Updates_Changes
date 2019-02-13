class BurlapSackCover extends ClothingBase
{
	void ~BurlapSackCover()
	{
		PlayerBase player;
		Class.CastTo(player, GetHierarchyRootPlayer());
		if( GetGame() && player )
		{
			OnRemovedFromHead(player);
		}
		
	}
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
			//GetGame().GetSoundScene().SetSoundVolume((g_Game.m_volume_sound/2),1);
			//GetGame().GetSoundScene().SetSpeechExVolume((g_Game.m_volume_speechEX/2),1);
			//GetGame().GetSoundScene().SetMusicVolume((g_Game.m_volume_music/2),1);
			//GetGame().GetSoundScene().SetVOIPVolume((g_Game.m_volume_VOIP/2),1);
			//GetGame().GetSoundScene().SetRadioVolume((g_Game.m_volume_radio/2),1);
			player.SetInventorySoftLock(true);
			player.SetMasterAttenuation("BurlapSackAttenuation");
			
			if ( GetGame().GetUIManager().IsMenuOpen(MENU_INVENTORY) )
			{
				GetGame().GetUIManager().FindMenu(MENU_INVENTORY).Close();
			}
		}
	}

	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		if ( GetGame().IsServer() )
		{
			PlayerBase player;
			Class.CastTo(player, parent.GetHierarchyRootPlayer());
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
		PlayerBase player;
		Class.CastTo(player, GetHierarchyRootPlayer());
		return (player && !player.IsAlive());
	}
	
	void OnRemovedFromHead(PlayerBase player)
	{
		if (  player.IsControlledPlayer() )
		{
			PPEffects.Init();
			PPEffects.DisableBurlapSackBlindness();
			//GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound,1);
			//GetGame().GetSoundScene().SetSpeechExVolume(g_Game.m_volume_speechEX,1);
			//GetGame().GetSoundScene().SetMusicVolume(g_Game.m_volume_music,1);
			//GetGame().GetSoundScene().SetVOIPVolume(g_Game.m_volume_VOIP,1);
			//GetGame().GetSoundScene().SetRadioVolume(g_Game.m_volume_radio,1);
			//GetGame().GetWorld().SetAperture(0);
			player.SetInventorySoftLock(false);
			player.SetMasterAttenuation("");
		}
		/*
		if ( GetGame().IsServer() )
		{
			MiscGameplayFunctions.TurnItemIntoItem(ItemBase.Cast( this ), "BurlapSack", player);
		}
		*/
	}
}