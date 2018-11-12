class Fence extends BaseBuildingBase
{	
	protected bool m_IsOpened 		= false;
	
	typename ATTACHMENT_BARBED_WIRE			= BarbedWire;
	typename ATTACHMENT_CAMONET 			= CamoNet;
	typename ATTACHMENT_COMBINATION_LOCK 	= CombinationLock;
	
	const string ATTACHMENT_SLOT_COMBINATION_LOCK = "Att_CombinationLock";
	
	void Fence()
	{
		CONSTRUCTION_KIT		= "FenceKit";
		
		//synchronized variables
		RegisterNetSyncVariableBool( "m_IsOpened" );
	}
	
	// --- EVENTS
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		//write
		ctx.Write( m_IsOpened );
	}
	
	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		//is opened
		bool is_opened;
		ctx.Read( is_opened );
		SetOpenedState( is_opened );
	}	
	
	// ---
	void SetOpenedState( bool state )
	{
		m_IsOpened = state;
		
		Synchronize();
	}
	
	bool IsOpened()
	{
		return m_IsOpened;
	}
	
	bool IsLocked()
	{
		CombinationLock combination_lock = GetCombinationLock();
		if ( combination_lock && combination_lock.IsLockedOnGate() )
		{
			return true;
		}
		
		return false;
	}
		
	CombinationLock GetCombinationLock()
	{
		CombinationLock combination_lock = CombinationLock.Cast( FindAttachmentBySlotName( ATTACHMENT_SLOT_COMBINATION_LOCK ) );
		return combination_lock;
	}
	
	//--- BUILD EVENTS
	override void OnPartDismantled( string part_name )
	{
		super.OnPartDismantled( part_name );
		
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		//check gate state
		if ( constrution_part.IsGate() )
		{
			if ( IsLocked() )
			{
				CombinationLock combination_lock = CombinationLock.Cast( FindAttachmentBySlotName( ATTACHMENT_SLOT_COMBINATION_LOCK ) );
				combination_lock.Unlock( this );
			}
		}
	}	
		
	//--- ATTACHMENT & CONDITIONS
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		if ( attachment.Type() == ATTACHMENT_BARBED_WIRE || attachment.Type() == ATTACHMENT_CAMONET )
		{
			if ( !HasBase() )
			{
				return false;
			}
		}
		
		if ( attachment.Type() == ATTACHMENT_COMBINATION_LOCK )
		{
			if ( !HasGate() && IsOpened() )
			{
				return false;
			}
		}
			
		return true;
	}
	
	override void EEItemAttached( EntityAI item, string slot_name )
	{
		super.EEItemAttached( item, slot_name );
		
		//Check combination lock
		if ( item.IsInherited( CombinationLock ) )
		{
			CombinationLock combination_lock = CombinationLock.Cast( item );
			combination_lock.Lock( this );
		}
	}
	
	//hands
	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		
		if ( HasBase() )
		{
			return false;
		}
		
		return true;
	}	
	
	//--- OPEN/CLOSE ACTIONS
	bool CanOpenFence()
	{
		if ( HasGate() && !IsOpened() && !IsLocked() )
		{
			return true;
		}
		
		return false;
	}
	
	bool CanCloseFence()
	{
		if ( HasGate() && IsOpened() )
		{
			return true;
		}
		
		return false;
	}
	
	void OpenFence()
	{
		SetAnimationPhase( "Wall_Interact_Rotate", 				100 );
		SetAnimationPhase( "Wall_Barbedwire_Mounted_Rotate", 	100 );
		SetAnimationPhase( "Wall_Camonet_Rotate", 				100 );
		SetAnimationPhase( "Wall_Gate_Rotate", 					100 );
		SetAnimationPhase( "Wall_Base_Down_Rotate", 			100 );
		SetAnimationPhase( "Wall_Base_Up_Rotate", 				100 );
		SetAnimationPhase( "Wall_Wood_Down_Rotate", 			100 );
		SetAnimationPhase( "Wall_Wood_Up_Rotate", 				100 );
		SetAnimationPhase( "Wall_Metal_Down_Rotate", 			100 );
		SetAnimationPhase( "Wall_Metal_Up_Rotate", 				100 );
		
		SetOpenedState( true );
	}
	
	void CloseFence()
	{
		SetAnimationPhase( "Wall_Interact_Rotate", 				0 );
		SetAnimationPhase( "Wall_Barbedwire_Mounted_Rotate", 	0 );
		SetAnimationPhase( "Wall_Camonet_Rotate", 				0 );
		SetAnimationPhase( "Wall_Gate_Rotate", 					0 );
		SetAnimationPhase( "Wall_Base_Down_Rotate", 			0 );
		SetAnimationPhase( "Wall_Base_Up_Rotate", 				0 );
		SetAnimationPhase( "Wall_Wood_Down_Rotate", 			0 );
		SetAnimationPhase( "Wall_Wood_Up_Rotate", 				0 );
		SetAnimationPhase( "Wall_Metal_Down_Rotate", 			0 );
		SetAnimationPhase( "Wall_Metal_Up_Rotate", 				0 );

		SetOpenedState( false );
	}
	
	//--- ACTION CONDITIONS
	override bool IsFacingFront( PlayerBase player )
	{
		vector fence_pos = GetPosition();
		vector player_pos = player.GetPosition();
		vector fence_dir = GetDirection();
		
		vector fence_player_dir = player_pos - fence_pos;
		fence_player_dir.Normalize();
		fence_dir.Normalize();
		
		if ( fence_dir.Length() != 0 )
		{
			float dot = vector.Dot( fence_player_dir, fence_dir );
			
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
		vector fence_dir = GetDirection();
		
		vector fence_player_dir = player_pos - fence_pos;
		fence_player_dir.Normalize();
		fence_dir.Normalize();
		
		if ( fence_dir.Length() != 0 )
		{
			float dot = vector.Dot( fence_player_dir, fence_dir );
			
			if ( dot < 0 )
			{
				return true;
			}
		}
		
		return false;
	}
}
