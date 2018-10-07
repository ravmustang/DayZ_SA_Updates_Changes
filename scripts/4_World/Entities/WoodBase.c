class WoodBase extends Plant
{
	override bool IsWoodBase()
	{
		return true;
	}

	int GetAmountOfDrops(ItemBase item)
	{
		if ( IsTree() )
		{
			if ( item && item.KindOf("Knife") )
			{
				return 10;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 3;
			}
			else
			{
				return 100; 
			}
		}
		else
		{
			if ( item && item.KindOf("Knife") )
			{
				return 1;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 1;
			}
			else
			{
				return 3;
			}
		}
	}
	
	void GetMaterialAndQuantityMap(ItemBase item, out map<string,int> output_map)
	{
		if ( IsTree() )
		{
			if ( item && item.KindOf("Knife") )
			{
				output_map.Insert("Bark_Oak",10);
			}
			else if ( item && item.KindOf("Axe") )
			{
				output_map.Insert("FireWood",1);
				output_map.Insert("WoodenStick",1);
				//output_map.Insert("TacticalBaconCan",1);
				//output_map.Insert("HumanSteakMeat",1);
			}
		}
		else
		{
			if ( item && item.KindOf("Knife") )
			{
				output_map.Insert("LongWoodenStick",1);
			}
			else if ( item && item.KindOf("Axe") )
			{
				output_map.Insert("LongWoodenStick",1);
			}
		}
	}
	
	float GetDamageToMiningItemEachDrop(ItemBase item)
	{
		if ( IsTree() )
		{
			if ( item && item.KindOf("Knife") )
			{
				return 10;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 5;
			}
			else
			{
				return 0; 
			}
		}
		else
		{
			if ( item && item.KindOf("Knife") )
			{
				return 10;
			}
			else if ( item && item.KindOf("Axe") )
			{
				return 5;
			}
			else
			{
				return 0;
			}
		}
	}
};