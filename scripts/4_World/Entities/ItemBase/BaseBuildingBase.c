//BASE BUILDING BASE
class BaseBuildingBase extends ItemBase
{
	const string 		ANIMATION_DEPLOYED			= "Deployed";
	
	float 				m_ConstructionKitHealth;			//stored health value for used construction kit

	ref Construction 	m_Construction;
	
	bool 				m_HasBase = false;
	//variables for synchronization of base building parts (2x31 is the current limit)
	int 				m_SyncParts01;								//synchronization for already built parts (31 parts)
	int 				m_SyncParts02;								//synchronization for already built parts (+31 parts)
	int 				m_SyncParts03;								//synchronization for already built parts (+31 parts)
	int 				m_InteractedPartId;							//construction part id that an action was performed on
	int					m_PerformedActionId;						//action id that was performed on a construction part
	
	//Sounds
	//build
	const string SOUND_BUILD_WOOD_LOG 			= "putDown_WoodLog_SoundSet";
	const string SOUND_BUILD_WOOD_PLANK			= "putDown_WoodPlank_SoundSet";
	const string SOUND_BUILD_WOOD_STAIRS		= "putDown_WoodStairs_SoundSet";
	const string SOUND_BUILD_METAL				= "putDown_MetalPlank_SoundSet";
	const string SOUND_BUILD_WIRE				= "putDown_BarbedWire_SoundSet";
	//dismantle
	const string SOUND_DISMANTLE_WOOD_LOG 		= "Crash_WoodPlank_SoundSet";
	const string SOUND_DISMANTLE_WOOD_PLANK		= "Crash_WoodPlank_SoundSet";
	const string SOUND_DISMANTLE_WOOD_STAIRS	= "Crash_WoodPlank_SoundSet";
	const string SOUND_DISMANTLE_METAL			= "Crash_MetalPlank_SoundSet";
	const string SOUND_DISMANTLE_WIRE			= "putDown_BarbedWire_SoundSet";
	
	protected EffectSound m_Sound;
		
	ref map<string, ref AreaDamageRegularDeferred> m_DamageTriggers;
	
	// Constructor
	void BaseBuildingBase() 
	{
		m_DamageTriggers = new ref map<string, ref AreaDamageRegularDeferred>;
		
		//synchronized variables
		RegisterNetSyncVariableInt( "m_SyncParts01" );
		RegisterNetSyncVariableInt( "m_SyncParts02" );
		RegisterNetSyncVariableInt( "m_SyncParts03" );
		RegisterNetSyncVariableInt( "m_InteractedPartId" );
		RegisterNetSyncVariableInt( "m_PerformedActionId" );
		RegisterNetSyncVariableBool( "m_HasBase" );
	}

	// --- SYNCHRONIZATION
	void Synchronize()
	{
		if ( GetGame().IsServer() )
		{
			SetSynchDirty();
			
			if ( GetGame().IsMultiplayer() )
			{
				Refresh();
			}
		}
	}
	
	//refresh visual/physics state
	void Refresh()
	{
		UpdateVisuals();
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdatePhysics, 200, false );
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		//update parts
		SetPartsFromSyncData();
		
		//update action on part
		SetActionFromSyncData();
		
		//update visuals (client)
		Refresh();
	}
	
	//parts synchronization
	void RegisterPartForSync( int part_id )
	{
		//part_id must starts from index = 1
		int offset;
		int mask;
		
		if ( part_id >= 1 && part_id <= 31 )		//<1,31> (31 parts)
		{
			offset = part_id - 1;
			mask = 1 << offset;
			
			m_SyncParts01 = m_SyncParts01 | mask;				
		}
		else if ( part_id >= 32 && part_id <= 62  )	//<32,62> (31 parts)
		{
			offset = ( part_id % 32 );
			mask = 1 << offset;
			
			m_SyncParts02 = m_SyncParts02 | mask;				
		}
		else if ( part_id >= 63 && part_id <= 93  )	//<63,93> (31 parts)
		{
			offset = ( part_id % 63 );
			mask = 1 << offset;
			
			m_SyncParts03 = m_SyncParts03 | mask;				
		}
	}
	
	void UnregisterPartForSync( int part_id )
	{
		//part_id must starts from index = 1
		int offset;
		int mask;
		
		if ( part_id >= 1 && part_id <= 31 )		//<1,31> (31 parts)
		{
			offset = part_id - 1;
			mask = 1 << offset;
			
			m_SyncParts01 = m_SyncParts01 & ~mask;			
		}
		else if ( part_id >= 32 && part_id <= 62  )	//<32,62> (31 parts)
		{
			offset = ( part_id % 32 );
			mask = 1 << offset;
			
			m_SyncParts02 = m_SyncParts02 & ~mask;			
		}
		else if ( part_id >= 63 && part_id <= 93  )	//<63,93> (31 parts)
		{
			offset = ( part_id % 63 );
			mask = 1 << offset;
			
			m_SyncParts03 = m_SyncParts03 & ~mask;			
		}
	}	
	
	bool IsPartBuildInSyncData( int part_id )
	{
		//part_id must starts from index = 1
		int offset;
		int mask;
		
		if ( part_id >= 1 && part_id <= 31 )		//<1,31> (31 parts)
		{
			offset = part_id - 1;
			mask = 1 << offset;
			
			if ( ( m_SyncParts01 & mask ) > 0 )
			{
				return true;
			}
		}
		else if ( part_id >= 32 && part_id <= 62  )	//<32,62> (31 parts)
		{
			offset = ( part_id % 32 );
			mask = 1 << offset;
			
			if ( ( m_SyncParts02 & mask ) > 0 )
			{
				return true;
			}
		}
		else if ( part_id >= 63 && part_id <= 93  )	//<63,93> (31 parts)
		{
			offset = ( part_id % 63 );
			mask = 1 << offset;
			
			if ( ( m_SyncParts03 & mask ) > 0 )
			{
				return true;
			}
		}				
	
		return false;
	}

	protected void RegisterActionForSync( int part_id, int action_id )
	{
		m_InteractedPartId 	= part_id;
		m_PerformedActionId = action_id;
	}
	
	protected void ResetActionSyncData()
	{
		//reset data
		m_InteractedPartId 	= -1;
		m_PerformedActionId = -1;
	}
	
	protected void SetActionFromSyncData()
	{
		if ( m_InteractedPartId > -1 && m_PerformedActionId > -1 )
		{
			ConstructionPart constrution_part = GetConstructionPartById( m_InteractedPartId );
			int action_id = m_PerformedActionId;
			
			switch( action_id )
			{
				case AT_BUILD_PART		: OnPartBuiltClient( constrution_part.GetPartName(), action_id ); break;
				case AT_DISMANTLE_PART	: OnPartDismantledClient( constrution_part.GetPartName(), action_id ); break;
				case AT_DESTROY_PART	: OnPartDestroyedClient( constrution_part.GetPartName(), action_id ); break;
			}
		}
	}
	//------
	
	//set construction parts based on synchronized data
	void SetPartsFromSyncData()
	{
		Construction construction = GetConstruction();
		map<string, ref ConstructionPart> construction_parts = construction.GetConstructionParts();
		
		for ( int i = 0; i < construction_parts.Count(); ++i )
		{
			string key = construction_parts.GetKey( i );
			ConstructionPart value = construction_parts.Get( key );
		
			bool is_part_built_sync = IsPartBuildInSyncData( value.GetId() );
			if ( is_part_built_sync )
			{
				if ( !value.IsBuilt() )
				{
					GetConstruction().AddToConstructedParts( key );
				}
			}
			else
			{
				if ( value.IsBuilt() )
				{
					GetConstruction().RemoveFromConstructedParts( key );
				}
			}
		}
	}
	
	protected ConstructionPart GetConstructionPartById( int id )
	{
		Construction construction = GetConstruction();
		map<string, ref ConstructionPart> construction_parts = construction.GetConstructionParts();
		
		for ( int i = 0; i < construction_parts.Count(); ++i )
		{
			string key = construction_parts.GetKey( i );
			ConstructionPart value = construction_parts.Get( key );
		
			if ( value.GetId() == id )
			{
				return value;
			}
		}
		
		return NULL;
	}
	//
	
	//Base
	bool HasBase()
	{
		return m_HasBase;
	}
	
	void SetBaseState( bool has_base )
	{
		m_HasBase = has_base;
	}

	// --- PLACING
	/*override bool IsHeavyBehaviour()
	{
		return true;
	}*/
	
	override bool IsDeployable()
	{
		return true;
	}
	
	//--- CONSTRUCTION KIT
	ItemBase CreateConstructionKit()
	{
		ItemBase construction_kit = ItemBase.Cast( GetGame().CreateObject( GetConstructionKitType(), GetKitSpawnPosition() ) );
		if ( m_ConstructionKitHealth > 0 )
		{
			
			construction_kit.SetHealth( m_ConstructionKitHealth );
		}
		
		return construction_kit;
	}
	
	protected vector GetKitSpawnPosition()
	{
		return GetPosition();
	}
	
	protected string GetConstructionKitType()
	{
		return "";
	}
	
	void DestroyConstructionKit( ItemBase construction_kit )
	{
		m_ConstructionKitHealth = construction_kit.GetHealth();
		GetGame().ObjectDelete( construction_kit );
	}
	
	//--- CONSTRUCTION
	void DestroyConstruction()
	{
		GetGame().ObjectDelete( this );
	}	
	
	// --- EVENTS
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		//sync parts 01
		ctx.Write( m_SyncParts01 );
		ctx.Write( m_SyncParts02 );
		ctx.Write( m_SyncParts03 );
		
		//has base 
		ctx.Write( m_HasBase );
	}
	
	override void OnStoreLoad( ParamsReadContext ctx, int version )
	{
		super.OnStoreLoad( ctx, version );
		
		//Restore synced parts data
		//sync parts 01
		ctx.Read( m_SyncParts01 );
		ctx.Read( m_SyncParts02 );
		ctx.Read( m_SyncParts03 );
		
		//has base
		ctx.Read( m_HasBase );
		
		//update server data
		SetPartsFromSyncData();
		
		//synchronize after load
		Synchronize();
	}
	
	override void EEInit()
	{
		super.EEInit();
		
		//Construction init
		ConstructionInit();
		
		//update visuals and physics
		Refresh();
	}

	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.OnItemLocationChanged( old_owner, new_owner );
		
		//update visuals after location change
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdatePhysics, 200, false );
	}
	
	override void EEItemAttached ( EntityAI item, string slot_name )
	{
		super.EEItemAttached ( item, slot_name );
		
		//update visuals and physics
		Refresh();
	}
	
	override void EEItemDetached ( EntityAI item, string slot_name )
	{
		super.EEItemDetached ( item, slot_name );
		
		//update visuals and physics
		Refresh();
	}
	
	//CONSTRUCTION EVENTS
	void OnPartBuiltServer( string part_name, int action_id )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		
		//check base state
		if ( constrution_part.IsBase() )
		{
			SetBaseState( true );
			
			//spawn kit
			CreateConstructionKit();
		}
			
		//register constructed parts for synchronization
		RegisterPartForSync( constrution_part.GetId() );
		
		//register action that was performed on part
		RegisterActionForSync( constrution_part.GetId(), action_id );
		
		//synchronize
		Synchronize();
		
		//reset action sync data
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( ResetActionSyncData, 100, false, this );
	}
	
	void OnPartBuiltClient( string part_name, int action_id )
	{
		//play sound
		SoundBuildStart( part_name );
	}	
	
	void OnPartDismantledServer( string part_name, int action_id )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		
		//check base state
		if ( constrution_part.IsBase() )
		{
			//Destroy construction
			DestroyConstruction();
		}
					
		//register constructed parts for synchronization
		UnregisterPartForSync( constrution_part.GetId() );
		
		//register action that was performed on part
		RegisterActionForSync( constrution_part.GetId(), action_id );
		
		//synchronize
		Synchronize();
		
		//reset action sync data
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( ResetActionSyncData, 100, false, this );
	}
	
	void OnPartDismantledClient( string part_name, int action_id )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		
		//play sound
		SoundDismantleStart( part_name );
	}	
	
	void OnPartDestroyedServer( string part_name, int action_id )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		
		//check base state
		if ( constrution_part.IsBase() )
		{
			//Destroy construction
			DestroyConstruction();
		}
					
		//register constructed parts for synchronization
		UnregisterPartForSync( constrution_part.GetId() );
		
		//register action that was performed on part
		RegisterActionForSync( constrution_part.GetId(), action_id );
		
		//synchronize
		Synchronize();
		
		//reset action sync data
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( ResetActionSyncData, 100, false, this );
	}
	
	void OnPartDestroyedClient( string part_name, int action_id )
	{
		//play sound
		SoundDestroyStart( part_name );
	}	
	
	// --- UPDATE
	void UpdateVisuals()
	{
		//update attachments visuals
		ref array<string> attachment_slots = new ref array<string>;
		GetAttachmentSlots( this, attachment_slots );
		for ( int i = 0; i < attachment_slots.Count(); i++ )
		{
			string slot_name = attachment_slots.Get( i );
			EntityAI attachment = FindAttachmentBySlotName( slot_name );
			string slot_name_mounted = slot_name + "_Mounted";
		
			if ( attachment )
			{
				if ( attachment.IsInherited( BarbedWire ) )
				{
					BarbedWire barbed_wire = BarbedWire.Cast( attachment );
					if ( barbed_wire.IsMounted() )
					{
						SetAnimationPhase( slot_name, 1 );
						SetAnimationPhase( slot_name_mounted, 0 );
						
						CreateAreaDamage( slot_name_mounted );			//create damage trigger if barbed wire is mounted
					}
					else
					{
						SetAnimationPhase( slot_name_mounted, 1 );
						SetAnimationPhase( slot_name, 0 );
						
						DestroyAreaDamage( slot_name_mounted );			//destroy damage trigger if barbed wire is not mounted
					}
				}
				else
				{
					if ( IsAttachmentSlotLocked( attachment ) )
					{
						SetAnimationPhase( slot_name_mounted, 1 );
						SetAnimationPhase( slot_name, 1 );
					}
					else
					{
						SetAnimationPhase( slot_name, 0 );
					}			
				}
			}
			else
			{
				SetAnimationPhase( slot_name_mounted, 1 );
				SetAnimationPhase( slot_name, 1 );
				
				DestroyAreaDamage( slot_name_mounted );			//try to destroy damage trigger if barbed wire is not present
			}
		}	
	
		//check base
		if ( !HasBase() )
		{
			SetAnimationPhase( ANIMATION_DEPLOYED, 0 );
		}
		else
		{
			SetAnimationPhase( ANIMATION_DEPLOYED, 1 );
		}
		
		GetConstruction().UpdateVisuals();
	}
	
	void UpdatePhysics()
	{
		//update attachments physics
		ref array<string> attachment_slots = new ref array<string>;
		GetAttachmentSlots( this, attachment_slots );
		for ( int i = 0; i < attachment_slots.Count(); i++ )
		{
			string slot_name = attachment_slots.Get( i );
			EntityAI attachment = FindAttachmentBySlotName( slot_name );
			string slot_name_mounted = slot_name + "_Mounted";
			
			if ( attachment )
			{
				if ( attachment.IsInherited( BarbedWire ) )
				{
					BarbedWire barbed_wire = BarbedWire.Cast( attachment );
					if ( barbed_wire.IsMounted() )
					{
						RemoveProxyPhysics( slot_name );
						AddProxyPhysics( slot_name_mounted );
					}
					else
					{
						RemoveProxyPhysics( slot_name_mounted );
						AddProxyPhysics( slot_name );
					}
				}
				else
				{
					if ( IsAttachmentSlotLocked( attachment ) )
					{
						RemoveProxyPhysics( slot_name_mounted );
						RemoveProxyPhysics( slot_name );
					}
					else
					{
						AddProxyPhysics( slot_name );
					}
				}
			}
			else
			{
				RemoveProxyPhysics( slot_name_mounted );
				RemoveProxyPhysics( slot_name );
			}
		}
		
		//check base
		if ( !HasBase() )
		{
			AddProxyPhysics( ANIMATION_DEPLOYED );
		}
		else
		{
			RemoveProxyPhysics( ANIMATION_DEPLOYED );
		}
		
		GetConstruction().UpdatePhysics();
		
		//regenerate navmesh
		UpdateNavmesh();
	}	
	
	protected void UpdateNavmesh()
	{
		SetAffectPathgraph( true, false );
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, this );
	}
	
	override bool CanUseConstruction()
	{
		return true;
	}

	protected bool IsAttachmentSlotLocked( EntityAI attachment )
	{
		if ( attachment )
		{
			InventoryLocation inventory_location = new InventoryLocation;
			attachment.GetInventory().GetCurrentInventoryLocation( inventory_location );
			
			return GetInventory().GetSlotLock( inventory_location.GetSlot() );
		}
			
		return false;
	}
	
	//--- ATTACHMENT SLOTS
	void GetAttachmentSlots( EntityAI entity, out array<string> attachment_slots )
	{
		string config_path = "CfgVehicles" + " " + entity.GetType() + " " + "attachments";
		if ( GetGame().ConfigIsExisting( config_path ) )
		{
			GetGame().ConfigGetTextArray( config_path, attachment_slots );
		}
	}
	
	// --- INIT
	void ConstructionInit()
	{
		if ( !m_Construction )
		{
			m_Construction = new Construction( this );
		}
		
		GetConstruction().Init();
	}
	
	Construction GetConstruction()
	{
		return m_Construction;
	}
	
	//--- INVENTORY/ATTACHMENTS CONDITIONS
	//attachments
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		return true;
	}
	
	bool HasAttachmentsBesidesBase()
	{
		int attachment_count = GetInventory().AttachmentCount();
		if ( attachment_count > 0 )
		{
			if ( HasBase() && attachment_count == 1 )
			{
				return false;
			}
			
			return true;
		}
		
		return false;
	}
	
	//this into/outo parent.Cargo
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	override bool CanRemoveFromCargo( EntityAI parent )
	{
		return false;
	}

	//hands
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}
	
	//--- ACTION CONDITIONS
	//direction
	bool IsFacingFront( PlayerBase player, string selection )
	{
		return true;
	}
	
	bool IsFacingBack( PlayerBase player, string selection )
	{
		return !IsFacingFront( player, selection );
	}
	
	//folding
	bool CanFoldBaseBuildingObject()
	{
		if ( HasBase() || GetInventory().AttachmentCount() > 0 )
		{
			return false;
		}
		
		return true;
	}
	
	void FoldBaseBuildingObject()
	{
		CreateConstructionKit();
		DestroyConstruction();
	}
	
	//Damage triggers (barbed wire)
	void CreateAreaDamage( string slot_name )
	{
		if ( GetGame() && GetGame().IsServer() )
		{
			//destroy area damage if some already exists
			DestroyAreaDamage( slot_name );
			
			//create new area damage
			AreaDamageRegularDeferred area_damage = new AreaDamageRegularDeferred( this );
			
			vector min_max[2];
			if ( MemoryPointExists( slot_name + "_min" ) )
			{
				min_max[0] = GetMemoryPointPos( slot_name + "_min" );
			}
			if ( MemoryPointExists( slot_name + "_max" ) )
			{
				min_max[1] = GetMemoryPointPos( slot_name + "_max" );
			}
			
			//get proper trigger extents (min<max)
			vector extents[2];
			GetConstruction().GetTriggerExtents( min_max, extents );
			
			//get box center
			vector center;
			center = GetConstruction().GetBoxCenter( min_max );
			center = ModelToWorld( center );
			
			area_damage.SetExtents( extents[0], extents[1] );
			area_damage.SetAreaPosition( center );
			area_damage.SetLoopInterval( 0.5 );
			area_damage.SetDeferInterval( 0.5 );
			area_damage.SetHitZones( { "Head","Torso","LeftHand","LeftLeg","LeftFoot","RightHand","RightLeg","RightFoot" } );
			area_damage.SetAmmoName( "MeleeDamage" );
			area_damage.Spawn();
			
			m_DamageTriggers.Insert( slot_name, area_damage );
		}
	}
	
	void DestroyAreaDamage( string slot_name )
	{
		if ( GetGame() && GetGame().IsServer() )
		{
			AreaDamageRegularDeferred area_damage;
			if ( m_DamageTriggers.Find( slot_name, area_damage ) ) 
			{
				if ( area_damage )
				{
					area_damage.DestroyDamageTrigger();
				}
				
				m_DamageTriggers.Remove( slot_name );
			}
		}
	}
	
	//================================================================
	// SOUNDS
	//================================================================
	protected void SoundBuildStart( string part_name )
	{
		PlaySoundSet( m_Sound, GetBuildSoundByMaterial( part_name ), 0.1, 0.1 );
	}

	protected void SoundDismantleStart( string part_name )
	{
		PlaySoundSet( m_Sound, GetDismantleSoundByMaterial( part_name ), 0.1, 0.1 );
	}
	
	protected void SoundDestroyStart( string part_name )
	{
		PlaySoundSet( m_Sound, GetDismantleSoundByMaterial( part_name ), 0.1, 0.1 );
	}
	
	protected string GetBuildSoundByMaterial( string part_name )
	{
		ConstructionMaterialType material_type = GetConstruction().GetMaterialType( part_name );
		
		switch( material_type )
		{
			case ConstructionMaterialType.MATERIAL_LOG: 	return SOUND_BUILD_WOOD_LOG;
			case ConstructionMaterialType.MATERIAL_WOOD: 	return SOUND_BUILD_WOOD_PLANK;
			case ConstructionMaterialType.MATERIAL_STAIRS: 	return SOUND_BUILD_WOOD_STAIRS;
			case ConstructionMaterialType.MATERIAL_METAL: 	return SOUND_BUILD_METAL;
			case ConstructionMaterialType.MATERIAL_WIRE:	return SOUND_BUILD_WIRE;
		}
		
		return "";
	}
	
	protected string GetDismantleSoundByMaterial( string part_name )
	{
		ConstructionMaterialType material_type = GetConstruction().GetMaterialType( part_name );
		
		switch( material_type )
		{
			case ConstructionMaterialType.MATERIAL_LOG: 	return SOUND_DISMANTLE_WOOD_LOG;
			case ConstructionMaterialType.MATERIAL_WOOD: 	return SOUND_DISMANTLE_WOOD_PLANK;
			case ConstructionMaterialType.MATERIAL_STAIRS: 	return SOUND_DISMANTLE_WOOD_STAIRS;
			case ConstructionMaterialType.MATERIAL_METAL: 	return SOUND_DISMANTLE_METAL;
			case ConstructionMaterialType.MATERIAL_WIRE:	return SOUND_DISMANTLE_WIRE;
		}
		
		return "";
	}	
}