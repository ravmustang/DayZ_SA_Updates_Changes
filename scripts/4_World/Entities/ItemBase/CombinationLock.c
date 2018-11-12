class CombinationLock extends ItemBase
{
	static const int COMBINATION_LENGTH = 3;			//how many digits will the combination contain
	int m_Combination 		= 111;									//actual combination that is dialed on lock
	int m_CombinationLocked = 999;							//combination that was dialed on lock before the shuffle
	
	bool m_IsLockAttached;								//for storing to db
	
	void CombinationLock()
	{
		int combination_length = Math.Pow( 10, COMBINATION_LENGTH );
		
		//synchronized variables
		RegisterNetSyncVariableInt( "m_Combination", 		0, combination_length - 1 );
		RegisterNetSyncVariableInt( "m_CombinationLocked", 	0, combination_length - 1 );
		RegisterNetSyncVariableBool( "m_IsLockAttached" );
	}
	
	override void EEInit()
	{
		super.EEInit();
		
		//set visual on init
		UpdateVisuals();
	}	
	
	void SetLockAttachedState( bool state )
	{
		m_IsLockAttached = state;
		
		Synchronize();
	}
	
	bool IsLockAttached()
	{
		return m_IsLockAttached;
	}	
	
	// --- VISUALS
	void UpdateVisuals()
	{
		if ( IsLockedOnGate() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( HideItem, 		0, false );
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( ShowAttached, 	0, false );
		}
		else
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( ShowItem, 		0, false );
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( HideAttached, 	0, false );
		}
	}
	
	//TODO
	protected void ShowItem()
	{
		SetAnimationPhase( "Combination_Lock_Item", 0 );
	}
	
	protected void HideItem()
	{
		SetAnimationPhase( "Combination_Lock_Item", 1 );
	}
	
	protected void ShowAttached()
	{
		SetAnimationPhase( "Combination_Lock_Attached", 0 );
	}
	
	protected void HideAttached()
	{
		SetAnimationPhase( "Combination_Lock_Attached", 1 );
	}	
	// ---
	
	// --- EVENTS
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		//write data
		ctx.Write( m_Combination );
		ctx.Write( m_CombinationLocked );
		ctx.Write( m_IsLockAttached );
	}
	
	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		//combination
		int combination;
		ctx.Read( combination );
		SetCombination( combination );
		
		//combination locked
		int combination_locked;
		ctx.Read( combination_locked );
		SetCombinationLocked( combination_locked );
		
		//lock attached
		int is_lock_attached;
		ctx.Read( is_lock_attached );
		SetLockAttachedState( is_lock_attached );		
	}
	
	// --- SYNCHRONIZATION
	void Synchronize()
	{
		if ( GetGame().IsServer() )
		{
			SetSynchDirty();
			
			UpdateVisuals();
		}
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		//update visuals (client)
		UpdateVisuals();
	}	
	
	void SetCombination( int combination )
	{
		m_Combination = combination;
		
		Synchronize();
	}
	
	void SetCombinationLocked( int combination )
	{
		m_CombinationLocked = combination;
		
		Synchronize();
	}
	
	// --- ACTIONS
	int GetCombination()
	{
		return m_Combination;
	}
	
	void DialNextNumber( int dial_index )
	{
		string combination_text = m_Combination.ToString();
		string dialed_text;
		
		//insert zeros to dials with 0 value
		int length_diff = COMBINATION_LENGTH - combination_text.Length();
		for ( int i = 0; i < length_diff; ++i )
		{
			combination_text = "0" + combination_text;
		}
		
		//assemble the whole combination with increased part
		for ( int j = 0; j < combination_text.Length(); ++j )
		{
			if ( j == dial_index )
			{
				int next_dialed_number = combination_text.Get( j ).ToInt() + 1;
				if ( next_dialed_number > 9 )
				{
					next_dialed_number = 0;
				}
				
				dialed_text += next_dialed_number.ToString();
			}
			else
			{
				dialed_text += combination_text.Get( j );
			}
		}
		
		SetCombination( dialed_text.ToInt() );
	}
	
	//Lock lock
	void Lock( EntityAI parent )
	{
		if ( !IsLockAttached() )
		{
			SetCombinationLocked( m_Combination );
			ShuffleLock();
			
			//set slot lock
			InventoryLocation inventory_location = new InventoryLocation;
			GetInventory().GetCurrentInventoryLocation( inventory_location );		
			parent.GetInventory().SetSlotLock( inventory_location.GetSlot(), true );
			
			//set lock attached
			SetLockAttachedState( true );
		}
	}
	
	void Unlock( EntityAI parent )
	{
		//set slot unlock
		InventoryLocation inventory_location = new InventoryLocation;
		GetInventory().GetCurrentInventoryLocation( inventory_location );			
		parent.GetInventory().SetSlotLock( inventory_location.GetSlot(), false );			
		
		//drop entity from attachment slot
		if ( GetGame().IsMultiplayer() )
		{
			parent.GetInventory().DropEntity( InventoryMode.PREDICTIVE, parent, this );
		}
		else
		{
			parent.GetInventory().DropEntity( InventoryMode.LOCAL, parent, this );
		}
		
		//set lock attached
		SetLockAttachedState( false );
	}
	
	//Shuffle lock
	void ShuffleLock()
	{
		string combination_text = m_Combination.ToString();
		string shuffled_text;
		
		//insert zeros to dials with 0 value
		int length_diff = COMBINATION_LENGTH - combination_text.Length();
		for ( int i = 0; i < length_diff; ++i )
		{
			combination_text = "0" + combination_text;
		}
		
		//assemble the whole combination with increased part
		for ( int j = 0; j < combination_text.Length(); ++j )
		{
			int dial_number = combination_text.Get( j ).ToInt();
			dial_number = ( dial_number + Math.RandomInt( 1, 9 ) ) % 10;
			shuffled_text = shuffled_text + dial_number.ToString();
		}
		
		SetCombination( shuffled_text.ToInt() );
	}
	
	bool IsLockedOnGate()
	{
		Fence fence = Fence.Cast( GetHierarchyParent() );
		if ( fence )
		{
			if ( m_Combination != m_CombinationLocked )
			{
				return true;
			}
		}
		
		return false;
	}
}
