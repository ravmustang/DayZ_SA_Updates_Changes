class ControlSchemeManager
{	
	static void SetControlScheme( EControlSchemeState state )
	{
		return;
		
		if ( GetUApi() )
		{
			GetUApi().DeactivateContext();
			
			switch ( state )
			{
				case EControlSchemeState.WeaponIronSight:
				{
					GetUApi().ActivateContext("ironsights");
					break;
				}
				case EControlSchemeState.WeaponMelee:
				{
					GetUApi().ActivateContext("melee");
					break;
				}
				case EControlSchemeState.WeaponGun:
				{
					GetUApi().ActivateContext("gun");
					break;
				}
				case EControlSchemeState.VehicleDriver:
				{
					GetUApi().ActivateContext("car");
					break;
				}
				case EControlSchemeState.UI:
				{
					GetUApi().ActivateContext("ui");
					break;
				}
			}
		}
		
	}
	
		
}