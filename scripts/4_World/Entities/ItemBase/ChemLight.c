class Chemlight_ColorBase : ItemBase
{
	override void OnWorkStart()
	{
		SetPilotLight(true);
	}
	
	// Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		StandUp();
	}
	
	void StandUp()
	{
		if ( GetGame().IsServer()  &&  GetCompEM().IsWorking() )
		{
			vector ori_rotate = "0 0 0";
			SetOrientation(ori_rotate);
		}
	}
	
	override void OnWorkStop()
	{
		SetPilotLight(false);
	}
	
	override void OnWork (float consumed_energy)
	{
		// Handle case when chemlight is put into cargo
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // client side
		{
			EntityAI container_EAI = GetHierarchyParent();
			
			if ( container_EAI  &&  container_EAI.IsInherited(ItemBase)  &&  !container_EAI.IsInherited(TripwireTrap) )
			{
				SetPilotLight(false);
			}
			else
			{
				SetPilotLight(true);
			}
		}
	}
};
