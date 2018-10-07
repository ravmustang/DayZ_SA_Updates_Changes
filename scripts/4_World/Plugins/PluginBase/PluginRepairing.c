class PluginRepairing extends PluginBase
{	
	static protected const int HEALTH_UP_ONE_LVL = 25;
	static protected const int PRISTINE_STATE = 0;
	static protected const int WORN_STATE = 1;
	static protected const int DAMAGED_STATE = 2;
	static protected const int BADLY_DAMAGED_STATE = 3;
	static protected const int RUINED_STATE = 4;

	bool Repair(PlayerBase player, ItemBase repair_kit, ItemBase item, float specialty_weight)
	{	
		switch ( item.GetHealthLabel() ) 
		{
			case PRISTINE_STATE:
				break;
				
			case WORN_STATE:
				if( CanRepairToPristine( player ) )
				{
					CalculateHealth( player, repair_kit, item, specialty_weight );
				}
				break;
				
			case DAMAGED_STATE:
				CalculateHealth( player, repair_kit, item, specialty_weight );
				break;
				
			case BADLY_DAMAGED_STATE:
				CalculateHealth( player, repair_kit, item, specialty_weight );
				break;
			
			case RUINED_STATE:
				break;
		}

		return true;
	}

	void CalculateHealth( PlayerBase player, ItemBase kit, ItemBase item, float specialty_weight )
	{
		float cur_kit_quantity = kit.GetQuantity();
		float cur_item_health = item.GetHealth("","");
		float kit_repair_cost = GetKitRepairCost( kit, item );
		float new_quantity;
		float new_quantity_adjusted;
		
		if ( cur_kit_quantity > HEALTH_UP_ONE_LVL )
		{
			new_quantity = kit.GetQuantity() - kit_repair_cost;
			//new_quantity_adjusted = player.GetSoftSkillManager().AddSpecialtyBonus( new_quantity, specialty_weight );
			//new_quantity_adjusted = Math.Clamp( new_quantity_adjusted, 0, 100 );
			kit.SetHealth( "", "", new_quantity );
			kit.SetQuantity( new_quantity, false );
			
			item.AddHealth( "", "", HEALTH_UP_ONE_LVL );
		}
		else
		{
			new_quantity = kit.GetQuantity() - kit_repair_cost;
			//new_quantity_adjusted = player.GetSoftSkillManager().AddSpecialtyBonus( new_quantity, specialty_weight );
			//new_quantity_adjusted = Math.Clamp( new_quantity_adjusted, 0, 100 );
			kit.SetHealth( "", "", new_quantity );
			kit.SetQuantity( new_quantity, false );
			
			item.AddHealth( "", "", cur_kit_quantity );
		}
				
		if ( !CanRepairToPristine( player ) )
		{
			float item_max_health = item.GetMaxHealth( "", "" );
			float item_health = item.GetHealth( "", "" );
			float clamp_health = Math.Clamp( item_health, 0, ( item_max_health * 0.7 ) );
			item.SetHealth( "", "", clamp_health );
		}		
	}

	bool CanRepair( ItemBase repair_kit, ItemBase item )
	{
		if ( item.GetHealthLabel() <= WORN_STATE)
		{
			return false;
		}
				
		int repair_kit_type = repair_kit.ConfigGetInt( "repairKitType" );
		
		ref array<int> repairable_with_types = new array<int>;
		item.ConfigGetIntArray( "repairableWithKits", repairable_with_types );	
					
		for ( int i = 0; i < repairable_with_types.Count(); i++ )
		{
			int repairable_with_type = repairable_with_types.Get(i);
			
			if ( IsRepairValid( repair_kit_type, repairable_with_type ) )
			{
				return true;
			}
		}

		return false;
		
	}
	
	private bool IsRepairValid(int repair_kit_type, int repairable_with_type)
	{
		if ( repair_kit_type > 0 && repairable_with_type > 0 )
		{
			if ( repair_kit_type == repairable_with_type )
			{
				return true;
			}
		}
		
		return false;
	}

	private bool CanRepairToPristine( PlayerBase player )
	{
// temporary disabled
/*
		float specialty = player.GetStatSpecialty().Get();
		
		if ( specialty <= -0.5 || specialty >= 0.5 )
		{
			return true;
		}
		else
		{
			return false; 
		}
*/
		return false; 
	}
	
	private float GetKitRepairCost( ItemBase repair_kit, ItemBase item )
	{
		ref array<int> repair_kit_types = new array<int>;	
		ref array<float> repair_kit_costs = new array<float>;	
		
		item.ConfigGetIntArray( "repairableWithKits", repair_kit_types );
		item.ConfigGetFloatArray( "repairCosts", repair_kit_costs );
		
		int eligible_kit;
		int kit_in_hands = repair_kit.ConfigGetInt( "repairKitType" );
		
		for ( int i = 0; i < repair_kit_types.Count(); i++ )
		{
			eligible_kit = repair_kit_types.Get(i);
			
			if ( eligible_kit == kit_in_hands )
			{
				float repair_cost = repair_kit_costs.Get(i);
			}
		} 
					
		return repair_cost;
	}
}
