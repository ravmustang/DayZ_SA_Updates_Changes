class Chemlight_ColorBase : ItemBase
{
	string m_DefaultMaterial;
	string m_GlowMaterial;
	
	void Chemlight_ColorBase()
	{
		//materials
		ref array<string> config_materials	= new array<string>;
		
		string config_path = "CfgVehicles" + " " + GetType() + " " + "hiddenSelectionsMaterials";
		GetGame().ConfigGetTextArray( config_path, config_materials );

		if (config_materials.Count() == 2)
		{
			m_DefaultMaterial = config_materials[0];
			m_GlowMaterial = config_materials[1];
		}
		else
		{
			string error = "Error! Item " + GetType() + " must have 2 entries in config array hiddenSelectionsMaterials[]. One for the default state, the other one for the glowing state. Currently it has " + config_materials.Count() + ".";
			Error(error);
		}
	}
	
	override void OnWorkStart()
	{
		SetPilotLight(true);
		SetObjectMaterial( 0, m_GlowMaterial );
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
		SetObjectMaterial( 0, m_DefaultMaterial );
		
		if ( GetGame().IsServer() )
		{
			SetHealth(0);
		}
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
