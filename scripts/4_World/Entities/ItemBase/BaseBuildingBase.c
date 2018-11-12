//BASE BUILDING BASE
class BaseBuildingBase extends ItemBase
{
	//TODO - remove player messages???
	static const string MESSAGE_CANNOT_BE_CONSTRUCTED		= "This part cannot be constructed.";
	static const string MESSAGE_CANNOT_BE_DECONSTRUCTED		= "This part cannot be dismantled because of some other part.";
	
	const string ANIMATION_DEPLOYED			= "Deployed";
			string CONSTRUCTION_KIT			= "";
	
	float 	m_ConstructionKitHealth;			//stored health value for used construction kit
	
	ref Construction 			m_Construction;
	
	bool 	m_HasBase = false;
	bool 	m_HasGate = false;
	//variables for synchronization of base building parts (2x32 is the current limit)
	int 	m_SyncParts01;									//synchronization for already built parts (32)
	int 	m_SyncParts02;									//synchronization for already built parts (64)
	
	ref map<string, ref AreaDamageRegularDeferred> m_DamageTriggers;
	
	// Constructor
	void BaseBuildingBase() 
	{
		//TODO - damage triggers
		/*
		SetFlags(EntityFlags.TRIGGER, false);
		
		m_DmgTriggers = new array<BarbedWireTrigger>;
		m_BarbedWires = new map<string,BarbedWire>;
		*/
		
		m_DamageTriggers = new ref map<string, ref AreaDamageRegularDeferred>;
		
		//synchronized variables
		RegisterNetSyncVariableInt( "m_SyncParts01" );
		RegisterNetSyncVariableInt( "m_SyncParts02" );
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
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( UpdateVisuals );
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdatePhysics, 100, false );
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		//update parts
		SetPartsFromSyncData();
		
		//update visuals (client)
		Refresh();
	}
	
	//parts synchronization
	void RegisterPartForSync( int part_id )
	{
		if ( part_id >= 1 )		//part_id must starts from index = 1
		{
			int offset;
			int mask;
			
			m_SyncParts01 = m_SyncParts01 | mask;
			
			if ( part_id > 32 )
			{
				offset = ( part_id % 32 ) - 1;
				mask = 1 << offset;
				
				m_SyncParts02 = m_SyncParts02 | mask;
			}
			else
			{
				offset = part_id - 1;
				mask = 1 << offset;
				
				m_SyncParts01 = m_SyncParts01 | mask;
			}
			
			Synchronize();
		}
	}
	
	void UnregisterPartForSync( int part_id )
	{
		if ( part_id >= 1 )		//part_id must starts from index = 1
		{
			int offset;
			int mask;
			
			m_SyncParts01 = m_SyncParts01 | mask;
			
			if ( part_id > 32 )
			{
				offset = ( part_id % 32 ) - 1;
				mask = 1 << offset;
				
				m_SyncParts02 = m_SyncParts02 & ~mask;
			}
			else
			{
				offset = part_id - 1;
				mask = 1 << offset;
				
				m_SyncParts01 = m_SyncParts01 & ~mask;
			}
			
			Synchronize();
		}		
	}	
	
	bool IsPartBuildInSyncData( int part_id )
	{
		if ( part_id >= 1 )		//part_id must starts from index = 1
		{
			int offset;
			int mask;
			
			m_SyncParts01 = m_SyncParts01 | mask;
			
			if ( part_id > 32 )
			{
				offset = ( part_id % 32 ) - 1;
				mask = 1 << offset;
				
				if ( ( m_SyncParts02 & mask ) > 0 )
				{
					return true;
				}
			}
			else
			{
				offset = part_id - 1;
				mask = 1 << offset;
				
				if ( ( m_SyncParts01 & mask ) > 0 )
				{
					return true;
				}
			}
		}			
		
		return false;
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
					GetConstruction().BuildPart( key, false );
				}
			}
			else
			{
				if ( value.IsBuilt() )
				{
					GetConstruction().DismantlePart( key, false );
				}
			}
		}
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
	
	//Gate
	bool HasGate()
	{
		return m_HasGate;
	}
	
	void SetGateState( bool has_gate )
	{
		m_HasGate = has_gate;
	}
	
	// --- PLACING
	override bool IsHeavyBehaviour()
	{
		return true;
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	
	//--- CONSTRUCTION KIT
	ItemBase CreateConstructionKit()
	{
		ItemBase construction_kit = ItemBase.Cast( GetGame().CreateObject( CONSTRUCTION_KIT, GetPosition() ) );
		construction_kit.SetHealth( m_ConstructionKitHealth );
		
		return construction_kit;
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
	}
	
	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		//Restore synced parts data
		//sync parts 01
		ctx.Read( m_SyncParts01 );
		ctx.Read( m_SyncParts02 );
		
		//restore parts from restored sync data
		SetPartsFromSyncData();
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
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdatePhysics, 100, false );
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
	void OnPartBuilt( string part_name )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		
		//check base state
		if ( constrution_part.IsBase() )
		{
			SetBaseState( true );
			
			//spawn kit
			if ( GetGame().IsServer() )
			{
				CreateConstructionKit();
			}
		}
	
		//check gate state
		if ( constrution_part.IsGate() )
		{
			SetGateState( true );
		}
	
		//register constructed parts for synchronization
		RegisterPartForSync( constrution_part.GetId() );		
	}
	
	void OnPartDismantled( string part_name )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		
		//check base state
		if ( constrution_part.IsBase() )
		{
			//Destroy construction
			DestroyConstruction();
		}
		
		//check gate state
		if ( constrution_part.IsGate() )
		{
			SetGateState( false );
		}		
		
		//register constructed parts for synchronization
		UnregisterPartForSync( constrution_part.GetId() );
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
						
						//TODO - add damage trigger once triggers are fixed
						//CreateAreaDamage( slot_name_mounted );			//create damage trigger if barbed wire is mounted
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
		SetAffectPathgraph( true, false );
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
		if ( GetInventory().AttachmentCount() > 1 )
		{
			if ( !HasBase() )
			{
				return true;
			}
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
	bool IsFacingFront( PlayerBase player )
	{
		return true;
	}
	
	bool IsFacingBack( PlayerBase player )
	{
		return true;
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
				min_max[0] = GetMemoryPointPos( slot_name + "_max" );
			}
			
			/*
			vector egde_length = GetConstruction().GetCollisionBoxSize( min_max );
			vector min;
			min[0] = -egde_length[0] / 2;
			min[1] = -egde_length[1] / 2;
			min[2] = -egde_length[2] / 2;
			vector max;
			max[0] = egde_length[0] / 2;
			max[1] = egde_length[1] / 2;
			max[2] = egde_length[2] / 2;
			area_damage.SetExtents( min_max[0], min_max[1] );
			*/
			area_damage.SetExtents( "-2 -1 -2", "2 1 2" );
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
}