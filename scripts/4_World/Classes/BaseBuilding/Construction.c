class Construction
{
	protected ref map<string, ref ConstructionPart> m_ConstructionParts;	//string - part name; int - 0-not constructed, 1-constructed
	protected BaseBuildingBase 	m_Parent;
	
	//Debug
	protected Shape m_CollisionBox;
	//Collision detectection
	protected ConstructionBoxTrigger m_ConstructionBoxTrigger;
	
	//============================================
	// Construction
	//============================================	
	void Construction( BaseBuildingBase parent )
	{
		m_ConstructionParts = new ref map<string, ref ConstructionPart>;
		
		//set parent object
		SetParent( parent );
	}
	
	void Init()
	{
		UpdateConstructionParts();
	}
		
	//parent
	protected BaseBuildingBase GetParent()
	{
		return m_Parent;
	}
	protected void SetParent( BaseBuildingBase parent )
	{
		m_Parent = parent;
	}
	//============================================
	// Construction process
	//============================================
	//constructed parts
	void AddToConstructedParts( string part_name )
	{
		ConstructionPart constrution_part = GetConstructionPart( part_name );
		
		if ( constrution_part )
		{
			constrution_part.SetBuiltState( true );
		}
	}
	
	void RemoveFromConstructedParts( string part_name )
	{
		ConstructionPart constrution_part = GetConstructionPart( part_name );
		
		if ( constrution_part ) 
		{
			constrution_part.SetBuiltState( false );
		}
	}
	
	//BuildPart
	void BuildPart( string part_name, bool take_materials )
	{
		//remove materials
		if ( take_materials )
		{
			TakeMaterials( part_name );
		}

		//add part to constructed parts
		AddToConstructedParts( part_name );
		
		//destroy build collision check trigger
		DestroyCollisionTrigger();
		
		//call event
		GetParent().OnPartBuilt( part_name );
	}
	
	//DismantlePart
	void DismantlePart( string part_name, bool receive_materials )
	{
		//receive materials
		ReceiveMaterials( part_name, receive_materials );
		
		//add part to constructed parts
		RemoveFromConstructedParts( part_name );
			
		//call event
		GetParent().OnPartDismantled( part_name );
	}
	
	//DestroyPart
	void DestroyPart( string part_name )
	{
		DismantlePart( part_name, false );
	}	
	
	//============================================
	// Update construction
	//============================================
	//update visual
	void UpdateVisuals()
	{
		for ( int i = 0; i < m_ConstructionParts.Count(); ++i )
		{
			string key = m_ConstructionParts.GetKey( i );
			ConstructionPart value = m_ConstructionParts.Get( key );
		
			if ( value.IsBuilt() )
			{
				ShowConstructionPart( value.GetPartName() );
		
			}
			else
			{
				HideConstructionPart( value.GetPartName() );
			}
		}
	}
	
	//update physics (only)
	void UpdatePhysics()
	{
		for ( int i = 0; i < m_ConstructionParts.Count(); ++i )
		{
			string key = m_ConstructionParts.GetKey( i );
			ConstructionPart value = m_ConstructionParts.Get( key );
		
			if ( value.IsBuilt() )
			{
				ShowConstructionPartPhysics( value.GetPartName() );
			}
			else
			{
				HideConstructionPartPhysics( value.GetPartName() );
			}
		}
	}
	
	//update construction parts
	protected void UpdateConstructionParts()
	{
		string construction_path = "cfgVehicles" + " " + GetParent().GetType() + " " + "Construction";
		
		if ( GetGame().ConfigIsExisting( construction_path ) )
		{
			int part_id;
			
			//main parts
			for ( int i = 0; i < GetGame().ConfigGetChildrenCount( construction_path ); ++i )
			{
				string main_part_name;
				GetGame().ConfigGetChildName( construction_path, i, main_part_name );
				string part_path = construction_path + " " + main_part_name;
				
				//parts
				for ( int j = 0; j < GetGame().ConfigGetChildrenCount( part_path ); ++j )
				{
					part_id++;		//inc part_id
					
					string part_name;
					GetGame().ConfigGetChildName( part_path, j, part_name );
					
					string name;
					GetGame().ConfigGetText( part_path + " " + part_name + " " + "name", name );							//name
					bool show_on_init = GetGame().ConfigGetInt( part_path + " " + part_name + " " + "show_on_init" );		//show on init
					bool is_base = GetGame().ConfigGetInt( part_path + " " + part_name + " " + "is_base" );					//is base (part)
					bool is_gate = GetGame().ConfigGetInt( part_path + " " + part_name + " " + "is_gate" );					//is gate (part)
					
					m_ConstructionParts.Insert( part_name, new ConstructionPart( name, part_name, main_part_name, part_id, show_on_init, is_base, is_gate ) );
				}
			}
		}
	}
	
	//============================================
	// Parts
	//============================================	
	map<string, ref ConstructionPart> GetConstructionParts()
	{
		return m_ConstructionParts;
	}
	
	ConstructionPart GetConstructionPart( string part_name )
	{
		return m_ConstructionParts.Get( part_name );
	}

	//CONSTRUCTION
	ConstructionPart GetConstructionPartToBuild( string part_name, ItemBase tool )
	{
		if ( CanBuildPart( part_name, tool ) )
		{
			return GetConstructionPart( part_name );
		}
		
		return NULL;
	}
	
	bool CanBuildPart( string part_name, ItemBase tool )
	{
		if ( !IsPartConstructed( part_name ) && HasRequiredPart( part_name ) && !HasConflictPart( part_name ) && HasMaterials( part_name ) && CanUseToolToBuildPart( part_name, tool ) )
		{
			return true;
		}
		
		return false;
	}
	
	//Get all construction parts that can be build (at that current time)
	void GetConstructionPartsToBuild( string main_part_name, out array<ConstructionPart> construction_parts, ItemBase tool )
	{
		construction_parts.Clear();
		
		for ( int i = 0; i < m_ConstructionParts.Count(); ++i )
		{
			string key = m_ConstructionParts.GetKey( i );
			ConstructionPart value = m_ConstructionParts.Get( key );
		
			if ( main_part_name == value.GetMainPartName() && CanBuildPart( value.GetPartName(), tool ) )
			{
				construction_parts.Insert( value );
			}
		}
	}
	
	//checks if construction part has required part already built
	protected bool HasRequiredPart( string part_name )
	{
		string main_part_name = GetConstructionPart( part_name ).GetMainPartName();
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + main_part_name + " " + part_name + " " + "required_parts";
		
		ref array<string> required_parts = new array<string>;
		GetGame().ConfigGetTextArray( cfg_path, required_parts );
		
		//check if parts are already built
		for ( int i = 0; i < required_parts.Count(); ++i )
		{
			if ( !IsPartConstructed( required_parts.Get( i ) ) )
			{
				return false;
			}
		}
		
		return true;
	}
	
	//checks if there are conflict parts already built
	protected bool HasConflictPart( string part_name )
	{
		string main_part_name = GetConstructionPart( part_name ).GetMainPartName();
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + main_part_name + " " + part_name + " " + "conflicted_parts";
		ref array<string> conflict_parts = new array<string>;
		GetGame().ConfigGetTextArray( cfg_path, conflict_parts );

		//check if parts are already built
		for ( int i = 0; i < conflict_parts.Count(); i++ )
		{
			if ( IsPartConstructed( conflict_parts.Get( i ) ) )
			{
				return true;
			}
		}
		
		return false;
	}	
	
	//DECONSTRUCTION
	ConstructionPart GetConstructionPartToDismantle( string part_name, ItemBase tool )
	{
		if ( CanDismantlePart( part_name, tool ) )
		{
			return GetConstructionPart( part_name );
		}
		
		return NULL;
	}	

	bool CanDismantlePart( string part_name, ItemBase tool )
	{
		if ( IsPartConstructed( part_name ) && !HasDependentPart( part_name ) && CanUseToolToDismantlePart( part_name, tool ) )
		{
			return true;
		}
		
		return false;
	}
	
	//checks if construction part has dependent part (that is already built) because of which it cannot be deconstruct
	bool HasDependentPart( string part_name )
	{
		for ( int i = 0; i < m_ConstructionParts.Count(); ++i )
		{
			string key = m_ConstructionParts.GetKey( i );
			ConstructionPart construction_part = m_ConstructionParts.Get( key );
			
			if ( construction_part.IsBuilt() )
			{
				string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " " + "Construction" + " " + construction_part.GetMainPartName() + " " + construction_part.GetPartName() + " " + "required_parts";
				ref array<string> required_parts = new array<string>;
				GetGame().ConfigGetTextArray( cfg_path, required_parts );
				
				if ( required_parts.Find( part_name ) > -1 )
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	//DESTROY
	ConstructionPart GetConstructionPartToDestroy( string part_name )
	{
		if ( CanDestroyPart( part_name ) )
		{
			return GetConstructionPart( part_name );
		}
		
		return NULL;
	}	

	bool CanDestroyPart( string part_name )
	{
		if ( IsPartConstructed( part_name ) && !HasDependentPart( part_name ) )
		{
			return true;
		}
		
		return false;
	}	
	
	//CONSTRUCTION PART STATE
	//show/hide construction part
	protected void ShowConstructionPart( string part_name )
	{
		GetParent().SetAnimationPhase( part_name, 0 );
	}
	
	protected void HideConstructionPart( string part_name )
	{
		GetParent().SetAnimationPhase( part_name, 1 );
	}
	
	//show/hide physics
	protected void ShowConstructionPartPhysics( string part_name )
	{
		GetParent().AddProxyPhysics( part_name );
	}
	
	protected void HideConstructionPartPhysics( string part_name )
	{
		GetParent().RemoveProxyPhysics( part_name );
	}	
	
	//is part constructed
	bool IsPartConstructed( string part_name )
	{
		ConstructionPart construction_part = GetConstructionPart( part_name );
		if ( construction_part && construction_part.IsBuilt() )
		{
			return true;
		}
		
		return false;
	}
	
	//============================================
	// Materials for construction
	//============================================
	//has materials
	bool HasMaterials( string part_name )
	{
		string main_part_name = GetConstructionPart( part_name ).GetMainPartName();
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + main_part_name + " " + part_name + " " + "Materials";
		
		if ( GetGame().ConfigIsExisting( cfg_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string child_name;
				GetGame().ConfigGetChildName( cfg_path, i, child_name );
				
				//get type, quantity from material
				string material_path;
				string slot_name;
				float quantity;
				material_path = cfg_path + " " + child_name + " " + "slot_name";
				GetGame().ConfigGetText( material_path, slot_name );
				material_path = cfg_path + " " + child_name + " " + "quantity";
				quantity = GetGame().ConfigGetFloat( material_path );
				
				//if the selected material (or its quantity) is not available
				if ( !HasMaterialWithQuantityAttached( slot_name, quantity ) )
				{
					return false;
				}
			}
		}
		
		return true;	//return true even if no material required
	}
	
	//check if parent object has attachment of required quantity attached to it
	protected bool HasMaterialWithQuantityAttached( string slot_name, float quantity )
	{
		ItemBase attachment = ItemBase.Cast( GetParent().FindAttachmentBySlotName( slot_name ) );
		
		if ( attachment && attachment.GetQuantity() >= quantity )
		{
			return true;
		}
		
		return false;
	}
	
	//take materials when building
	protected void TakeMaterials( string part_name )
	{
		string main_part_name = GetConstructionPart( part_name ).GetMainPartName();
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + main_part_name + " " + part_name + " " + "Materials";
		
		if ( GetGame().ConfigIsExisting( cfg_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string child_name;
				GetGame().ConfigGetChildName( cfg_path, i, child_name );
				
				//get type, quantity from material
				string config_path;
				string slot_name;
				config_path = cfg_path + " " + child_name + " " + "slot_name";
				GetGame().ConfigGetText( config_path, slot_name );
				config_path = cfg_path + " " + child_name + " " + "quantity";
				float quantity = GetGame().ConfigGetFloat( config_path );
				config_path = cfg_path + " " + child_name + " " + "lockable";
				bool lockable = GetGame().ConfigGetInt( config_path );
				
				ItemBase attachment = ItemBase.Cast( GetParent().FindAttachmentBySlotName( slot_name ) );
				if ( lockable )
				{
					//lock attachment
					InventoryLocation inventory_location = new InventoryLocation;
					attachment.GetInventory().GetCurrentInventoryLocation( inventory_location );
			
					GetParent().GetInventory().SetSlotLock( inventory_location.GetSlot(), true );
				}
				else
				{
					if ( quantity > -1 )						//0 - ignores quantity
					{
						//subtract quantity
						attachment.AddQuantity( -quantity );
					}
					else										//-1 - deletes the object
					{
						GetGame().ObjectDelete( attachment );
					}	
				}
			}
		}
	}
	
	//receive materials when dismantling
	protected void ReceiveMaterials( string part_name, bool receive_materials )
	{
		string main_part_name = GetConstructionPart( part_name ).GetMainPartName();
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + main_part_name + " " + part_name + " " + "Materials";
		
		if ( GetGame().ConfigIsExisting( cfg_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string child_name;
				GetGame().ConfigGetChildName( cfg_path, i, child_name );
				
				//get type, quantity from material
				string config_path;
				string type;
				string slot_name;
				config_path = cfg_path + " " + child_name + " " + "type";
				GetGame().ConfigGetText( config_path, type );
				config_path = cfg_path + " " + child_name + " " + "slot_name";
				GetGame().ConfigGetText( config_path, slot_name );
				config_path = cfg_path + " " + child_name + " " + "quantity";
				float quantity = GetGame().ConfigGetFloat( config_path );
				config_path = cfg_path + " " + child_name + " " + "lockable";
				bool lockable = GetGame().ConfigGetInt( config_path );
				
				//receive material quantity
				ItemBase attachment = ItemBase.Cast( GetParent().FindAttachmentBySlotName( slot_name ) );
				int slot_id;
				
				//material still attached
				if ( lockable )			//if lockable 
				{
					if ( attachment )
					{
						InventoryLocation inventory_location = new InventoryLocation;
						attachment.GetInventory().GetCurrentInventoryLocation( inventory_location );
						GetParent().GetInventory().SetSlotLock( inventory_location.GetSlot() , false );
						
						if ( receive_materials )		//drop attachment if true
						{
							//detach
							if ( GetGame().IsMultiplayer() )
							{
								GetGame().RemoteObjectTreeDelete(attachment);
								GetParent().GetInventory().DropEntity( InventoryMode.LOCAL, GetParent(), attachment );
								GetGame().RemoteObjectTreeCreate(attachment);	
							}
							else
							{
								GetParent().GetInventory().DropEntity( InventoryMode.LOCAL, GetParent(), attachment );
							}
						}
						else
						{
							GetGame().ObjectDelete( attachment );		//delete object if not
						}
					}
				}
				else
				{
					if ( receive_materials )
					{
						if ( attachment )
						{
							float att_quantity = attachment.GetQuantity();
							float att_max_quantity = attachment.GetQuantityMax();
							float att_quantity_diff = att_max_quantity - att_quantity; 
							if ( quantity > att_quantity_diff )
							{
								while ( quantity > 0 )
								{
									//create material on ground if quantity exceeds max quantity
									ItemBase received_material = ItemBase.Cast( GetGame().CreateObject( attachment.GetType(), GetParent().GetPosition() ) );
									if ( quantity > att_max_quantity )
									{
										received_material.SetQuantity( att_max_quantity );
									}
									else
									{
										received_material.SetQuantity( quantity );
									}
									
									quantity -= att_max_quantity;
								}
							}
							else
							{
								attachment.AddQuantity( quantity );
							}
						}
						//material slot is empty, create a new material
						else
						{
							attachment = ItemBase.Cast( GetGame().CreateObject( type, GetParent().GetPosition() ) );
							if ( quantity > 0 ) 					//object was deleted or the quantity is ignored
							{
								attachment.SetQuantity( quantity );
							}
							
							//attach item
							slot_id = InventorySlots.GetSlotIdFromString( slot_name );
							GetParent().GetInventory().TakeEntityAsAttachmentEx( InventoryMode.LOCAL, attachment, slot_id );
						}
					}
				}
			}
		}
	}	
	
	//============================================
	// Construction tools
	//============================================	
	bool CanUseToolToBuildPart( string part_name, ItemBase tool )
	{
		ConstructionPart construction_part = GetConstructionPart( part_name );
		string part_cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + construction_part.GetMainPartName() + " " + construction_part.GetPartName() + " " + "build_action_type";
		if ( GetGame().ConfigIsExisting( part_cfg_path ) )
		{
			int part_build_action_type = GetGame().ConfigGetInt( part_cfg_path );
			string tool_cfg_path = "cfgVehicles" + " " + tool.GetType() + " " + "build_action_type";
			
			if ( GetGame().ConfigIsExisting( tool_cfg_path ) )
			{
				int tool_build_action_type = GetGame().ConfigGetInt( tool_cfg_path );
  
				if ( ( part_build_action_type & tool_build_action_type ) > 0 )
				{
					return true;
				}				
			}
		}
		
		return false;
	}
	
	bool CanUseToolToDismantlePart( string part_name, ItemBase tool )
	{
		ConstructionPart construction_part = GetConstructionPart( part_name );
		string part_cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + construction_part.GetMainPartName() + " " + construction_part.GetPartName() + " " + "dismantle_action_type";
		if ( GetGame().ConfigIsExisting( part_cfg_path ) )
		{
			int part_dismantle_action_type = GetGame().ConfigGetInt( part_cfg_path );
			string tool_cfg_path = "cfgVehicles" + " " + tool.GetType() + " " + "dismantle_action_type";
			
			if ( GetGame().ConfigIsExisting( tool_cfg_path ) )
			{
				int tool_dismantle_action_type = GetGame().ConfigGetInt( tool_cfg_path );
  
				if ( ( part_dismantle_action_type & tool_dismantle_action_type ) > 0 )
				{
					return true;
				}				
			}
		}
		
		return false;
	}
	
	//============================================
	// Collision check
	//============================================
	//Collisions (BBox and Trigger)
	bool IsColliding( string part_name )
	{
		ConstructionPart construction_part = GetConstructionPart( part_name );
		
		if ( construction_part )
		{
			vector center;
			vector absolute_ofset 	= "0 0.01 0"; 	//we need to lift BBox even more, because it colliddes with house floors due to various reasons (probably geometry or float imperfections)
			vector orientation 		= GetParent().GetOrientation();
			vector edge_length;
			vector min_max_local[2];				//data used for creating trigger
			vector min_max_world[2];				//data used for creating BBox
			ref array<Object> excluded_objects = new array<Object>;
			ref array<Object> collided_objects = new array<Object>;
			
			//get min_max and center from config and memory points
			GetCollisionBoxData( part_name, center, min_max_local );
			
			//set BBox data
			min_max_world[0] = GetParent().ModelToWorld( min_max_local[0] );
			min_max_world[1] = GetParent().ModelToWorld( min_max_local[1] );
						
			center = center + absolute_ofset;
			edge_length = GetCollisionBoxSize( min_max_world );
			
			excluded_objects.Insert( GetParent() );
			
			//Create trigger
			CreateCollisionTrigger( part_name, min_max_local );
			
			//check collision on box trigger and collision box
			if ( IsTriggerColliding() || GetGame().IsBoxColliding( center, orientation, edge_length, excluded_objects, collided_objects ) )
			{	
				//Debug
				//DrawDebugCollisionBox( min_max_world, ARGB( 255, 255, 0, 0 ) );
				//
				
				return true;	
			}
			
			//Debug
			//DrawDebugCollisionBox( min_max_world, ARGB( 255, 255, 255, 255 ) );
			//
		}
		return false;
	}

	protected vector GetCollisionBoxSize( vector min_max[2] )
	{
		vector box_size = Vector( 0, 0, 0 );
		
		box_size[0] = Math.AbsFloat( min_max[1][0] - min_max[0][0] );
		box_size[1] = Math.AbsFloat( min_max[1][1] - min_max[0][1] );
		box_size[2] = Math.AbsFloat( min_max[1][2] - min_max[0][2] );
		
		return box_size;
	}
	
	protected void GetCollisionBoxData( string part_name, out vector center, out vector min_max[2] )
	{
		string main_part_name = GetConstructionPart( part_name ).GetMainPartName();
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + main_part_name + " " + part_name + " " + "collision_data";
		ref array<string> collision_data = new array<string>;
		GetGame().ConfigGetTextArray( cfg_path, collision_data );
		
		if ( collision_data.Count() > 0 )
		{
			if ( GetParent().MemoryPointExists( collision_data[0] ) )
			{
				min_max[0] = GetParent().GetMemoryPointPos( collision_data[0] );
			}
			if ( GetParent().MemoryPointExists( collision_data[1] ) )
			{
				min_max[1] = GetParent().GetMemoryPointPos( collision_data[1] );
			}
			
			center = GetBoxCenter( min_max );
		}
	}
	
	protected vector GetBoxCenter( vector min_max[2] )
	{
		vector center;
		
		center[0] = ( min_max[1][0] - min_max[0][0] ) / 2;
		center[1] = ( min_max[1][1] - min_max[0][1] ) / 2;
		center[2] = ( min_max[1][2] - min_max[0][2] ) / 2;
		
		return center;
	}
	
	//Debug
	protected void DrawDebugCollisionBox( vector min_max[2], int color )
	{
		DestroyDebugCollisionBox();
		
		m_CollisionBox = Debug.DrawBox( min_max[0], min_max[1], color );
	}
	
	protected void DestroyDebugCollisionBox()
	{
		if ( m_CollisionBox )
		{
			m_CollisionBox.Destroy();
			m_CollisionBox = NULL;
		}		
	}
	
	void CreateCollisionTrigger( string part_name, vector min_max[2] )
	{
		if ( m_ConstructionBoxTrigger )
		{
			if ( m_ConstructionBoxTrigger.GetPartName() == part_name )
			{
				return;
			}
			else
			{
				DestroyCollisionTrigger();
			}
		}	
		
		//calculate proper collision trigger position
		float center_offset_x = ( min_max[1][0] - min_max[0][0] ) / 2;
		float center_offset_y = ( min_max[1][1] - min_max[0][1] ) / 2;
		float center_offset_z = ( min_max[1][2] - min_max[0][2] ) / 2;
		vector trigger_center = GetBoxCenter( min_max );
		vector trigger_center_offset = Vector( trigger_center[0] - center_offset_x, trigger_center[1] - center_offset_y, trigger_center[2] - center_offset_z );
		vector trigger_center_world = GetParent().ModelToWorld( trigger_center_offset );
		
		//create trigger
		m_ConstructionBoxTrigger = ConstructionBoxTrigger.Cast( GetGame().CreateObject( "ConstructionBoxTrigger", trigger_center_world, true ) );
		m_ConstructionBoxTrigger.SetPosition( trigger_center_world );
		m_ConstructionBoxTrigger.SetOrientation( GetParent().GetOrientation() );	
		m_ConstructionBoxTrigger.SetExtents( min_max[0], min_max[1] );
		
		m_ConstructionBoxTrigger.SetPartName( part_name );
	}
	
	void DestroyCollisionTrigger()
	{
		GetGame().ObjectDelete( m_ConstructionBoxTrigger );
		m_ConstructionBoxTrigger = NULL;
	}
	
	bool IsTriggerColliding()
	{
		return m_ConstructionBoxTrigger.IsColliding();
	}
}

class ConstructionBoxTrigger : ManTrigger
{
	string m_PartName;
	
	void SetPartName( string part_name )
	{
		m_PartName = part_name;
	}
	
	string GetPartName()
	{
		return m_PartName;
	}
	
	override protected void UpdateInsiders( int timeout )
	{
		super.UpdateInsiders( 20 );
	}
	
	bool IsColliding()
	{
		if ( GetInsiders().Count() > 0 )
		{
			return true;
		}
		
		return false;
	}
}
