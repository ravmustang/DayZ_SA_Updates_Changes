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
		
		/*
		SetAnimationPhase( TRIPOD_DEPLOYED, 1 );
		RemoveProxyPhysics( TRIPOD_DEPLOYED );	
		*/
	}
}