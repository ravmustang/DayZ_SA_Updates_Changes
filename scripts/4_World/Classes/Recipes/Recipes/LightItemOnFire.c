// This is a generic crafting recipe which sets target item on fire by using some source item.
// Documentation can be found on Confluence as "Item-to-item fire distribution"

class LightItemOnFire extends RecipeBase	
{	
	override void Init()
	{
		m_Name = "#STR_LightItemOnFire0";
		m_IsInstaRecipe = false;//should this recipe be performed instantly without animation
		m_AnimationLength = 1;//animation length in relative time units
		m_Specialty = -0.01;// value > 0 for roughness, value < 0 for precision
		
		
		//conditions
		m_MinDamageIngredient[0] = -1;//-1 = disable check
		m_MaxDamageIngredient[0] = -1;//-1 = disable check
		
		m_MinQuantityIngredient[0] = -1;//-1 = disable check
		m_MaxQuantityIngredient[0] = -1;//-1 = disable check
		
		m_MinDamageIngredient[1] = -1;//-1 = disable check
		m_MaxDamageIngredient[1] = -1;//-1 = disable check
		
		m_MinQuantityIngredient[1] = -1;//-1 = disable check
		m_MaxQuantityIngredient[1] = -1;//-1 = disable check
		//----------------------------------------------------------------------------------------------------------------------
		
		//INGREDIENTS
		//ingredient 1
		InsertIngredient(0,"Torch");//you can insert multiple ingredients this way
		InsertIngredient(0,"Matchbox");
		InsertIngredient(0,"Fireplace");
		InsertIngredient(0,"Roadflare");
		InsertIngredient(0,"PortableGasStove");
		InsertIngredient(0,"HandDrillKit");
		InsertIngredient(1,"PortableGasStove");

		m_IngredientAddHealth[0] = 0;// 0 = do nothing
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = 0;// 0 = do nothing
		m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
		m_IngredientUseSoftSkills[0] = false;// set 'true' to allow modification of the values by softskills on this ingredient
		
		//ingredient 2
		InsertIngredient(1,"Torch");//you can insert multiple ingredients this way
		InsertIngredient(1,"Matchbox");
		InsertIngredient(1,"Fireplace");
		InsertIngredient(1,"Roadflare");
		InsertIngredient(1,"PortableGasStove");
		InsertIngredient(1,"HandDrillKit");
		InsertIngredient(1,"PortableGasStove");
		
		m_IngredientAddHealth[1] = 0;// 0 = do nothing
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = -1;// 0 = do nothing
		m_IngredientDestroy[1] = false;// false = do nothing
		m_IngredientUseSoftSkills[1] = false;// set 'true' to allow modification of the values by softskills on this ingredient
		//----------------------------------------------------------------------------------------------------------------------
		
		//result1
		//AddResult("");//add results here

		m_ResultSetFullQuantity[0] = false;//true = set full quantity, false = do nothing
		m_ResultSetQuantity[0] = -1;//-1 = do nothing
		m_ResultSetHealth[0] = -1;//-1 = do nothing
		m_ResultInheritsHealth[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value "color" of ingredient (value)
		m_ResultToInventory[0] = -2;//(value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
		m_ResultUseSoftSkills[0] = false;// set 'true' to allow modification of the values by softskills on this result
		m_ResultReplacesIngredient[0] = -1;// value == -1 means do nothing; a value >= 0 means this result will transfer item propertiesvariables, attachments etc.. from an ingredient value
	}

	override bool CanDo(ItemBase ingredients[], PlayerBase player)//final check for recipe's validity
	{
		ItemBase item_source, item_target;
		
		// Due to an edge case with Torch, it is sometimes necesarry to swap item_source with item_target.
		ItemBase potential_item_source = ItemBase.Cast(ingredients[0]);
		ItemBase potential_item_target = ItemBase.Cast(ingredients[1]);
		
		if (potential_item_source.CanIgniteItem(potential_item_target))
		{
			item_source = ItemBase.Cast(ingredients[0]);
			item_target = ItemBase.Cast(ingredients[1]);
		}
		else if (potential_item_target.CanIgniteItem(potential_item_source))
		{
			item_source = ItemBase.Cast(ingredients[1]);
			item_target = ItemBase.Cast(ingredients[0]);
		}
		
		// item_source & item_target are now correctly defined.
		
		if (!item_source  ||  !item_target)
		{
			return false;
		}
		
		if (!item_target.HasFlammableMaterial())
			return false;
		
		if (item_target.IsIgnited())
			return false; // item is already on fire
		
		if (!item_source.CanIgniteItem(item_target))
			return false;
		
		if (!item_target.CanBeIgnitedBy(item_source))
			return false;
		
		if ( item_target.GetInventory() )
		{
			InventoryLocation loc = new InventoryLocation;
			item_target.GetInventory().GetCurrentInventoryLocation(loc);
			
			if ( loc.GetIdx() > -1 )
			{
				return false;
			}
		}
		
		return true;
	}

	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight)//gets called upon recipe's completion
	{
		ItemBase item_source, item_target;
		
		// Due to an edge case with Torch, it is sometimes necesarry to swap item_source with item_target.
		ItemBase potential_item_source = ItemBase.Cast(ingredients[0]);
		ItemBase potential_item_target = ItemBase.Cast(ingredients[1]);
		
		if (potential_item_source.CanIgniteItem(potential_item_target))
		{
			item_source = ItemBase.Cast(ingredients[0]);
			item_target = ItemBase.Cast(ingredients[1]);
		}
		else if (potential_item_target.CanIgniteItem(potential_item_source))
		{
			item_source = ItemBase.Cast(ingredients[1]);
			item_target = ItemBase.Cast(ingredients[0]);
		}
		
		// item_source & item_target are now correctly defined.
		
		if (item_source  &&  item_target  &&  item_source.IsTargetIgnitionSuccessful(item_target)  &&  item_target.IsThisIgnitionSuccessful(item_source) )
		{
			item_source.OnIgnitedTarget(item_target);
			item_target.OnIgnitedThis(item_source);
		}
		else
		{
			item_source.OnIgnitedTargetFailed(item_target);
			item_target.OnIgnitedThisFailed(item_source);
		}
		
		player.GetSoftSkillManager().AddSpecialty( UASoftSkillsWeight.ROUGH_LOW );
	}
};
