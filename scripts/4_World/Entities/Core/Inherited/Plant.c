//-----------------------------------------------------------------------------
/** \brief Specific tree class can be declared as: class TreeHard + _ + p3d filename (without extension) 
@code
//exapmle for tree DZ\plants2\tree\t_acer2s.p3d
class TreeHard_t_acer2s: TreeHard 
{
};
@endcode
*/

class TreeHard extends PlantSuper
{
	override bool IsTree()
	{
		return true;
	}
		
	override void PlayFallingPlantSound()
	{
		SoundParams soundParams = new SoundParams("hardTreeFall_SoundSet");
		SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = soundBuilder.BuildSoundObject();
		soundObject.SetPosition(GetPosition());
		GetGame().GetSoundScene().Play3D(soundObject, soundBuilder);
	}
	
	override int GetAmountOfDrops(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 10;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 4;
		}
		else
		{
			return 100; 
		}
	}
	
	override void GetMaterialAndQuantityMap(ItemBase item, out map<string,int> output_map)
	{
		if ( item && item.KindOf("Knife") )
		{
			output_map.Insert("Bark_Oak",1);
		}
		else if ( item && item.KindOf("Axe") )
		{
			output_map.Insert("FireWood",1);
			output_map.Insert("WoodenStick",1);
		}
	}
	
	override float GetDamageToMiningItemEachDrop(ItemBase item)
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
};

//-----------------------------------------------------------------------------
//! For specific tree declaration see description of \ref TreeHard
class TreeSoft extends PlantSuper
{
	override bool IsTree()
	{
		return true;
	}
	
	override void PlayFallingPlantSound()
	{
		SoundParams soundParams = new SoundParams("softTreeFall_SoundSet");
		SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = soundBuilder.BuildSoundObject();
		soundObject.SetPosition(GetPosition());
		GetGame().GetSoundScene().Play3D(soundObject, soundBuilder);
	}
	
	override int GetAmountOfDrops(ItemBase item)
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
	
	override void GetMaterialAndQuantityMap(ItemBase item, out map<string,int> output_map)
	{
		if ( item && item.KindOf("Knife") )
		{
			output_map.Insert("Bark_Oak",1);
		}
		else if ( item && item.KindOf("Axe") )
		{
			output_map.Insert("FireWood",1);
			output_map.Insert("WoodenStick",1);
		}
	}
	
	override float GetDamageToMiningItemEachDrop(ItemBase item)
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
};

//-----------------------------------------------------------------------------
//! For specific tree declaration see description of \ref TreeHard
//! default values for hard bushes
class BushHard extends PlantSuper
{
	override bool IsBush()
	{
		return true;
	}
	
	override void PlayFallingPlantSound()
	{
		SoundParams soundParams = new SoundParams("hardBushFall_SoundSet");
		SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = soundBuilder.BuildSoundObject();
		soundObject.SetPosition(GetPosition());
		GetGame().GetSoundScene().Play3D(soundObject, soundBuilder);
	}
	
	override int GetAmountOfDrops(ItemBase item)
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
	
	override void GetMaterialAndQuantityMap(ItemBase item, out map<string,int> output_map)
	{
		if ( item && item.KindOf("Knife") )
		{
			output_map.Insert("LongWoodenStick",1);
		}
		else if ( item && item.KindOf("Axe") )
		{
			output_map.Insert("LongWoodenStick",1);
			output_map.Insert("WoodenStick",1);
		}
	}
	
	override float GetDamageToMiningItemEachDrop(ItemBase item)
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
};

//-----------------------------------------------------------------------------
//! For specific tree declaration see description of \ref TreeHard
class BushSoft extends PlantSuper
{
	override bool IsBush()
	{
		return true;
	}
	
	override void PlayFallingPlantSound()
	{
		SoundParams soundParams = new SoundParams("softBushFall_SoundSet");
		SoundObjectBuilder soundBuilder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = soundBuilder.BuildSoundObject();
		soundObject.SetPosition(GetPosition());
		GetGame().GetSoundScene().Play3D(soundObject, soundBuilder);
	}
	
	override int GetAmountOfDrops(ItemBase item)
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
	
	override void GetMaterialAndQuantityMap(ItemBase item, out map<string,int> output_map)
	{
		if ( item && item.KindOf("Knife") )
		{
			output_map.Insert("LongWoodenStick",1);
		}
		else if ( item && item.KindOf("Axe") )
		{
			output_map.Insert("LongWoodenStick",1);
			//output_map.Insert("WoodenStick",1);
		}
	}
	
	override float GetDamageToMiningItemEachDrop(ItemBase item)
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
};
