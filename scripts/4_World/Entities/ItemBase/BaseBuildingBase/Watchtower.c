class Watchtower extends BaseBuildingBase
{	
	typename ATTACHMENT_BARBED_WIRE			= BarbedWire;
	typename ATTACHMENT_CAMONET 			= CamoNet;
	
	void Watchtower()
	{
		CONSTRUCTION_KIT		= "WatchtowerKit";
	}
	
	//--- ATTACHMENT & CONDITIONS
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		string slot_name;
		InventorySlots.GetSelectionForSlotId( slotId , slot_name );
		slot_name.ToLower();
		if ( slot_name.Contains( "material_l2" ) || slot_name.Contains( "level_2" ) )
		{
			if ( !GetConstruction().IsPartConstructed( "level_1_roof" ) )
			{
				return false;
			}
		}
		//level 3
		else if ( slot_name.Contains( "material_L3" ) || slot_name.Contains( "level_3" ) )
		{
			if ( !GetConstruction().IsPartConstructed( "level_2_roof" ) )
			{
				return false;
			}			
		}
		
		if ( attachment.Type() == ATTACHMENT_BARBED_WIRE || attachment.Type() == ATTACHMENT_CAMONET )
		{
			if ( !HasBase() )
			{
				return false;
			}
		}
			
		return true;
	}

	//can put into hands
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}	
	
	//TODO - add proper direction conditions to IsFacingBack and IsFacingFront
	//--- ACTION CONDITIONS
	override bool IsFacingFront( PlayerBase player )
	{
		vector fence_pos = GetPosition();
		vector player_pos = player.GetPosition();
		vector player_dir = player.GetDirection();
		
		vector player_fence_dir = fence_pos - player_pos;
		player_fence_dir.Normalize();
		player_dir.Normalize();
		
		if ( player_dir.Length() != 0 )
		{
			float dot = vector.Dot( player_fence_dir, player_dir );
			
			if ( dot > 0 )
			{
				return true;
			}
		}
		
		return false;
	}
	
	override bool IsFacingBack( PlayerBase player )
	{
		vector fence_pos = GetPosition();
		vector player_pos = player.GetPosition();
		vector player_dir = player.GetDirection();
		
		vector player_fence_dir = fence_pos - player_pos;
		player_fence_dir.Normalize();
		player_dir.Normalize();
		
		if ( player_dir.Length() != 0 )
		{
			float dot = vector.Dot( player_fence_dir, player_dir );
			
			if ( dot < 0 )
			{
				return true;
			}
		}
		
		return false;
	}
}