class Tripod_Proxy_Test extends ItemBase
{
	//Visual states
	const string TRIPOD_COLLAPSED 	= "Collapsed";
	const string TRIPOD_DEPLOYED 	= "Deployed";
	
	override void EEInit()
	{
		super.EEInit();
		
		//refresh visuals
		SetAnimationPhase( TRIPOD_COLLAPSED, 0 );
		AddProxyPhysics( TRIPOD_COLLAPSED );
		
		SetAnimationPhase( TRIPOD_DEPLOYED, 1 );
		RemoveProxyPhysics( TRIPOD_DEPLOYED );
		
		//create damage area
		AreaDamageRegularDeferred area_damage = new AreaDamageRegularDeferred( this );
		area_damage.SetExtents( "-2 -1 -2", "2 1 2" );
		area_damage.SetLoopInterval( 0.5 );
		area_damage.SetDeferInterval( 0.5 );
		area_damage.SetHitZones( { "Head","Torso","LeftHand","LeftLeg","LeftFoot","RightHand","RightLeg","RightFoot" } );
		area_damage.SetAmmoName( "MeleeDamage" );
		area_damage.Spawn();
	}
}