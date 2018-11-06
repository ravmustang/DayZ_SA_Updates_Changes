class FenceKit extends ItemBase
{	
	override void EEInit()
	{
		super.EEInit();
		
		//set visual on init
		UpdateVisuals();
		UpdatePhysics();
	}
	
	override void OnPlacementComplete( Man player )
	{
		//Create fence
		PlayerBase player_base = PlayerBase.Cast( player );
		vector position = player_base.GetLocalProjectionPosition();
		vector orientation = player_base.GetLocalProjectionOrientation();
		
		Fence fence = Fence.Cast( GetGame().CreateObject( "Fence", GetPosition() ) );
		fence.SetPosition( position );
		fence.SetOrientation( orientation );
		
		//destroy construction kit
		fence.DestroyConstructionKit( this );
	}
	
	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.OnItemLocationChanged( old_owner, new_owner );
		
		//update visuals after location change
		UpdatePhysics();
	}
	
	//Update visuals and physics
	void UpdateVisuals()
	{
		SetAnimationPhase( "Inventory", 0 );
		SetAnimationPhase( "Placing", 1 );
	}
	
	void UpdatePhysics()
	{
		AddProxyPhysics( "Inventory" );
		RemoveProxyPhysics( "Placing" );		
	}	
	
	override bool IsOneHandedBehaviour()
	{
		return true;
	}
	
	override bool IsDeployable()
	{
		return true;
	}	
}
