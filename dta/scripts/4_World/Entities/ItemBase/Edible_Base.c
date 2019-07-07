class Edible_Base extends ItemBase
{
	ref FoodStage m_FoodStage;
	
	void Edible_Base()
	{
		if ( HasFoodStage() )
		{
			m_FoodStage = new FoodStage( this );
			
			//synchronized variables
			RegisterNetSyncVariableInt( "m_FoodStage.m_FoodStageType", 	FoodStageType.NONE, FoodStageType.COUNT );
			RegisterNetSyncVariableInt( "m_FoodStage.m_SelectionIndex", 0, 6 );
			RegisterNetSyncVariableInt( "m_FoodStage.m_TextureIndex", 	0, 6 );
			RegisterNetSyncVariableInt( "m_FoodStage.m_MaterialIndex", 	0, 6 );
			RegisterNetSyncVariableFloat( "m_FoodStage.m_CookingTime", 	0, 600, 0 );						//min = 0; max = 0; precision = 0;
		}
	}
	
	override void EEInit()
	{
		super.EEInit();
		
		//update visual
		UpdateVisuals();
	}
	
	void UpdateVisuals()
	{
		if ( GetFoodStage() )
		{
			GetFoodStage().UpdateVisuals();
		}
	}
	
	bool Consume(float amount, PlayerBase consumer)
	{
		AddQuantity(-amount, false, false);
		OnConsume(amount, consumer);
		return true;
	}
	
	void OnConsume(float amount, PlayerBase consumer);	
	
	//food staging
	override bool CanBeCooked()
	{
		return false;
	}
	
	override bool CanBeCookedOnStick()
	{
		return false;
	}
	
	//================================================================
	// SYNCHRONIZATION
	//================================================================	
	void Synchronize()
	{
		if ( GetGame().IsServer() )
		{
			SetSynchDirty();
			
			if ( GetGame().IsMultiplayer() )
			{
				UpdateVisuals();
			}
		}
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		//update visuals
		UpdateVisuals();
	}	
	
	//================================================================
	// SERIALIZATION
	//================================================================	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );

		if ( GetFoodStage() )
		{
			GetFoodStage().OnStoreSave( ctx );
		}
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( GetFoodStage() )
		{
			if ( !GetFoodStage().OnStoreLoad( ctx, version ) )
			return false;
		}
		
		return true;
	}
	
	override void AfterStoreLoad()
	{	
		super.AfterStoreLoad();
		
		//synchronize
		Synchronize();
	}	

	//get food stage
	FoodStage GetFoodStage()
	{
		return m_FoodStage;
	}
	
	//food types
	override bool IsMeat()
	{
		return false;
	}
	
	override bool IsFruit()
	{
		return false;
	}
	
	override bool IsMushroom()
	{
		return false;
	}	
	
	//================================================================
	// NUTRITIONAL VALUES
	//================================================================	
	//food properties
	static float GetFoodTotalVolume(ItemBase item, string classname = "", int food_stage = 0)
	{
		Edible_Base food_item = Edible_Base.Cast(item);
		if(food_item && food_item.GetFoodStage())
		{
			 return FoodStage.GetFullnessIndex(food_item.GetFoodStage());
		}
		else if(classname != "" && food_stage)
		{
			return FoodStage.GetFullnessIndex(null, food_stage, classname);
		}
		string class_path = "cfgVehicles " + classname + " Nutrition";
		return GetGame().ConfigGetFloat( class_path + " fullnessIndex" );

	}
	
	static float GetFoodEnergy(ItemBase item, string classname = "", int food_stage = 0)
	{
		Edible_Base food_item = Edible_Base.Cast(item);
		if(food_item && food_item.GetFoodStage())
		{
			 return FoodStage.GetEnergy(food_item.GetFoodStage());
		}
		else if(classname != "" && food_stage)
		{
			return FoodStage.GetEnergy(null, food_stage, classname);
		}
		string class_path = "cfgVehicles " + classname + " Nutrition";
		return GetGame().ConfigGetFloat( class_path + " energy" );			
	}
	
	static float GetFoodWater(ItemBase item, string classname = "", int food_stage = 0)
	{
		Edible_Base food_item = Edible_Base.Cast(item);
		if(food_item && food_item.GetFoodStage())
		{
			return FoodStage.GetWater(food_item.GetFoodStage());
		}
		else if(classname != "" && food_stage)
		{
			return FoodStage.GetWater(null, food_stage, classname);
		}
		string class_path = "cfgVehicles " + classname + " Nutrition";
		return GetGame().ConfigGetFloat( class_path + " water" );			
	}
	
	static float GetFoodNutritionalIndex(ItemBase item, string classname = "", int food_stage = 0)
	{
		Edible_Base food_item = Edible_Base.Cast(item);
		if(food_item && food_item.GetFoodStage())
		{
			return FoodStage.GetNutritionalIndex(food_item.GetFoodStage());	
		}
		else if(classname != "" && food_stage)
		{
			return FoodStage.GetNutritionalIndex(null, food_stage, classname);
		}
		string class_path = "cfgVehicles " + classname + " Nutrition";
		return GetGame().ConfigGetFloat( class_path + " nutritionalIndex" );		
		
	}
	
	static float GetFoodToxicity(ItemBase item, string classname = "", int food_stage = 0)
	{
		Edible_Base food_item = Edible_Base.Cast(item);
		if(food_item && food_item.GetFoodStage())
		{
			return FoodStage.GetToxicity(food_item.GetFoodStage());
		}
		else if(classname != "" && food_stage)
		{
			return FoodStage.GetToxicity(null, food_stage, classname);
		}
		string class_path = "cfgVehicles " + classname + " Nutrition";
		return GetGame().ConfigGetFloat( class_path + " toxicity" );			
	}
	
	static int GetFoodAgents(ItemBase item, string classname = "", int food_stage = 0)
	{
		Edible_Base food_item = Edible_Base.Cast(item);
		if(food_item && food_item.GetFoodStage())
		{
			return FoodStage.GetAgents(food_item.GetFoodStage());
		}
		else if(classname != "" && food_stage)
		{
			return FoodStage.GetAgents(null, food_stage, classname);
		}
		string class_path = "cfgVehicles " + classname + " Nutrition";
		return GetGame().ConfigGetInt( class_path + " agents" );
	}
	
	static float GetFoodDigestibility(ItemBase item, string classname = "", int food_stage = 0)
	{
		Edible_Base food_item = Edible_Base.Cast(item);
		if(food_item && food_item.GetFoodStage())
		{
			return FoodStage.GetDigestibility(food_item.GetFoodStage());
		}
		else if(classname != "" && food_stage)
		{
			return FoodStage.GetDigestibility(null, food_stage, classname);
		}
		string class_path = "cfgVehicles " + classname + " Nutrition";
		return GetGame().ConfigGetInt( class_path + " digestibility" );
	}
	
	static NutritionalProfile GetNutritionalProfile(ItemBase item, string classname = "", int food_stage = 0)
	{
		return new NutritionalProfile(GetFoodEnergy(item, classname, food_stage),GetFoodWater(item, classname, food_stage),GetFoodNutritionalIndex(item, classname, food_stage),GetFoodTotalVolume(item, classname, food_stage), GetFoodToxicity(item, classname, food_stage),  GetFoodAgents(item, classname,food_stage), GetFoodDigestibility(item, classname,food_stage));
	}
	
	//================================================================
	// FOOD STAGING
	//================================================================
	FoodStageType GetFoodStageType()
	{
		return GetFoodStage().GetFoodStageType();
	}
	
	//food stage states
	bool IsFoodRaw()
	{
		if ( GetFoodStage() ) 
		{
			return GetFoodStage().IsFoodRaw();
		}
		
		return false;
	}

	bool IsFoodBaked()
	{
		if ( GetFoodStage() ) 
		{
			return GetFoodStage().IsFoodBaked();
		}
		
		return false;
	}
	
	bool IsFoodBoiled()
	{
		if ( GetFoodStage() ) 
		{
			return GetFoodStage().IsFoodBoiled();
		}
		
		return false;
	}
	
	bool IsFoodDried()
	{
		if ( GetFoodStage() ) 
		{
			return GetFoodStage().IsFoodDried();
		}
		
		return false;
	}
	
	bool IsFoodBurned()
	{
		if ( GetFoodStage() ) 
		{
			return GetFoodStage().IsFoodBurned();
		}
		
		return false;
	}
	
	bool IsFoodRotten()
	{
		if ( GetFoodStage() ) 
		{
			return GetFoodStage().IsFoodRotten();
		}
		
		return false;
	}				
	
	//food stage change
	void ChangeFoodStage( FoodStageType new_food_stage_type )
	{
		GetFoodStage().ChangeFoodStage( new_food_stage_type );
	}
	
	FoodStageType GetNextFoodStageType( CookingMethodType cooking_method )
	{
		return GetFoodStage().GetNextFoodStageType( cooking_method );
	}
	
	string GetFoodStageName( FoodStageType food_stage_type )
	{
		return GetFoodStage().GetFoodStageName( food_stage_type );
	}
	
	bool CanChangeToNewStage( CookingMethodType cooking_method )
	{
		return GetFoodStage().CanChangeToNewStage( cooking_method );
	}
	
	//================================================================
	// COOKING
	//================================================================
	//cooking time
	float GetCookingTime()
	{
		return GetFoodStage().GetCookingTime();
	}
	
	void SetCookingTime( float time )
	{
		GetFoodStage().SetCookingTime( time );
		
		//synchronize when calling on server
		Synchronize();
	}
	
	//replace edible with new item (opening cans)
	void ReplaceEdibleWithNew (string typeName)
	{
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (player)
		{
			ReplaceEdibleWithNewLambda lambda = new ReplaceEdibleWithNewLambda(this, typeName, player);
			player.ServerReplaceItemInHandsWithNew(lambda);
		}
		else
			Error("ReplaceEdibleWithNew - cannot use edible without player");
	}
}

class ReplaceEdibleWithNewLambda : TurnItemIntoItemLambda
{
	void ReplaceEdibleWithNewLambda (EntityAI old_item, string new_item_type, PlayerBase player) { }
};
