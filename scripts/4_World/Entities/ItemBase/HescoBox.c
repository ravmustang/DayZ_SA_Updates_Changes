class HescoBox extends Inventory_Base
{
	static const int FOLDED 		= 0;
	static const int UNFOLDED 		= 1;
	static const int FILLED 		= 2;
	
	static ref array<string> 		m_SurfaceForSetup;
	ref Timer 						m_Timer;

	protected int m_State;
	
	void HescoBox()
	{
		m_State = FOLDED;
		
		if (!m_SurfaceForSetup)
		{
			m_SurfaceForSetup = new array<string>;
			InsertMaterialForSetup();
		}
		
		//synchronized variables
		RegisterNetSyncVariableInt( "m_State", FOLDED, FILLED );
	}

	override string GetDeploySoundset()
	{
		return "hescobox_deploy_SoundSet";
	}
	
	override bool IsHeavyBehaviour()
	{
		return true;
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		return CanBeManipulated();
	}

	override void OnPlacementComplete( Man player )
	{
		Unfold();
	}

	void Synchronize()
	{
		SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		//refresh visuals
		RefreshVisuals();
	}
	
	void RefreshVisuals()
	{
		if( GetState() == UNFOLDED )
		{
			InsertMaterialForSetup();
		}
	}

	int GetState()
	{
		return m_State;
	}
	
	void SetState( int state )
	{
		m_State = state;
	}

	bool CanBeFilledAtPosition( vector position )
	{
		string surface_type;
		GetGame().SurfaceGetType( position[0], position[2], surface_type );

		if ( m_SurfaceForSetup.Find(surface_type) > -1 )
		{
			return true;
		}
		
		return false;
	}

	bool CanBeManipulated()
	{
		if ( GetState() == FOLDED )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void Fold()
	{
		this.ShowSelection( "inventory" );
		this.HideSelection( "placing" );
		this.HideSelection( "filled" );
		
		SetState( FOLDED );
		RefreshPhysics();
		
		if ( GetGame().IsServer() )
		{
			SetAllowDamage(true);
			Synchronize();
			DecreaseHealth( "", "", 5 ); //TODO Daniel implement soft skill bonus via useraction
		}
	}

	void Unfold()
	{
		this.HideSelection( "inventory" );
		this.ShowSelection( "placing" );
		this.HideSelection( "filled" );
		
		InsertMaterialForSetup();
		SetState( UNFOLDED );
		RefreshPhysics();
		
		if ( GetGame().IsServer() )
		{
			SetAllowDamage(true);
			Synchronize();
			DecreaseHealth( "", "", 5 ); //TODO Daniel implement soft skill bonus via useraction
		}
	}

	override void EEItemLocationChanged (notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged (oldLoc, newLoc);
		
		RefreshPhysics();
	}
	
	void RefreshPhysicsDelayed()
	{
		if ( this  &&  !ToDelete() )
		{
			RemoveProxyPhysics( "inventory" );
			RemoveProxyPhysics( "placing" );
			RemoveProxyPhysics( "filled" );
			
			int state = GetState();
			
			switch (state)
			{
				case UNFOLDED:
					//ShowSelection( "placing" );
					AddProxyPhysics( "placing" ); 
					
				return;
				
				case FOLDED:
					AddProxyPhysics( "inventory" ); 
				return;
				
				case FILLED:
					AddProxyPhysics( "filled" ); 
				return;
			}
		}
	}
	
	void RefreshPhysics()
	{
		if (!m_Timer)
			m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
		
		m_Timer.Run(0.1, this, "RefreshPhysicsDelayed");
	}
	
	void InsertMaterialForSetup()
	{	
		m_SurfaceForSetup.Insert("cp_dirt");
		m_SurfaceForSetup.Insert("cp_broadleaf_dense1");
		m_SurfaceForSetup.Insert("cp_broadleaf_dense2");
		m_SurfaceForSetup.Insert("cp_broadleaf_sparse1");
		m_SurfaceForSetup.Insert("cp_broadleaf_sparse2");
		m_SurfaceForSetup.Insert("cp_conifer_common1");
		m_SurfaceForSetup.Insert("cp_conifer_common2");
		m_SurfaceForSetup.Insert("cp_conifer_moss1");
		m_SurfaceForSetup.Insert("cp_conifer_moss2");
		m_SurfaceForSetup.Insert("cp_grass");
		m_SurfaceForSetup.Insert("cp_grass_tall");
		m_SurfaceForSetup.Insert("cp_gravel");
	}

	void Fill()
	{
		this.HideSelection( "inventory" );
		this.HideSelection( "placing" );
		this.ShowSelection( "filled" );
		
		SetState( FILLED );
		RefreshPhysics();
		
		if ( GetGame().IsServer() )
		{
			Synchronize();
			DecreaseHealth( "", "", 5 ); //TODO Daniel implement soft skill bonus via useraction
			SetAllowDamage(false);
		}
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{   
		super.OnStoreSave(ctx);
		
		// Save state
		ctx.Write( m_State );
	}

	override void OnStoreLoad(ParamsReadContext ctx)
	{   
		super.OnStoreLoad(ctx);
		
		// Load folded/unfolded state
		int state = FOLDED;
		ctx.Read(state);
		
		switch (state)
		{
			case FOLDED:
			{
				Fold();
				break;
			}
			case UNFOLDED:
			{
				Unfold();
				break;
			}
			case FILLED:
			{
				Fill();
				break;
			}
		}
	}
}
