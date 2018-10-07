class WorldCraftActionData : ActionData
{
	int m_RecipeID;
}

class ActionWorldCraftCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousCraft(UATimeSpent.DEFAULT_CRAFT); //default value can be set in recipes
	}

	/*override void OnFinish(bool pCanceled)	
	{
		super.OnFinish(pCanceled);
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			PlayerBase player;
			if( Class.CastTo(player, GetGame().GetPlayer()) )
			{
				if( player.GetCraftingManager().IsInventoryCraft() )
					player.GetCraftingManager().CancelInventoryCraft();
			}
		}
	}*/
};

class ActionWorldCraft: ActionContinuousBase
{
	private string m_ActionPrompt;

	void ActionWorldCraft()
	{
		m_CallbackClass = ActionWorldCraftCB;		
		m_MessageStartFail = "Ooops";
		m_MessageStart = "I have started crafting.";
		m_MessageSuccess = "I crafted something!";
		m_MessageFail = "Oops again..";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_ActionPrompt = "Craft";
	}
	
	override ActionData CreateActionData()
	{
		ActionData action_data = new WorldCraftActionData;
		return action_data;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_WORLD_CRAFT;
	}
		
	override string GetText()
	{
		PlayerBase player;
		if( Class.CastTo(player, GetGame().GetPlayer()) )
		{
			PluginRecipesManager module_recipes_manager;
			Class.CastTo(module_recipes_manager,  GetPlugin(PluginRecipesManager) );
			return module_recipes_manager.GetRecipeName( player.GetCraftingManager().GetRecipeID() );
		}

		return "Default worldcraft text";
	}
	
	override bool Can( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//Client
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{					
			if ( /*player.GetCraftingManager().IsWorldCraft() || player.GetCraftingManager().IsInventoryCraft()*/ player.GetCraftingManager().GetRecipesCount() > 0 )
			{
				return true;
			}
			return false;
		}
		//Server
		return true;		
	}
	
	override void Start( ActionData action_data ) //Setup on start of action
	{
		super.Start(action_data);
		
		WorldCraftActionData wcActionData;
		Class.CastTo(wcActionData, action_data);
		
		if( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			wcActionData.m_RecipeID = action_data.m_Player.GetCraftingRecipeID();
		/*	ref ActionTarget newTarget;
			newTarget = new ActionTarget(action_data.m_Player.GetCraftingManager().m_item2, null, -1, vector.Zero, 0);		
			action_data.m_Callback.SetActionData( action_data ); */
		}
		if ( action_data.m_MainItem ) action_data.m_MainItem.SetInvisible(true);
	}
	
	override void OnCompleteServer( ActionData action_data )
	{
		if ( action_data.m_MainItem ) action_data.m_MainItem.SetInvisible(false);
	}
	
	override void OnCancelServer( ActionData action_data )
	{
		if ( action_data.m_MainItem ) action_data.m_MainItem.SetInvisible(false);
	}
	
	override void OnCompleteClient( ActionData action_data )
	{
		if ( action_data.m_MainItem ) action_data.m_MainItem.SetInvisible(false);
	}
	
	override void OnCancelClient( ActionData action_data )
	{
		if ( action_data.m_MainItem ) action_data.m_MainItem.SetInvisible(false);
	}	
	
	override void OnCompleteLoopServer( ActionData action_data )
	{
		if (!GetGame().IsMultiplayer())
		{
			ActionManagerClient am = ActionManagerClient.Cast(action_data.m_Player.GetActionManager());
			am.UnlockInventory(action_data);
		}
		
		WorldCraftActionData wcActionData;
		PluginRecipesManager module_recipes_manager;
		ItemBase item2;
		
		Class.CastTo(wcActionData, action_data);
		Class.CastTo(module_recipes_manager,  GetPlugin(PluginRecipesManager) );
		Class.CastTo(item2,  action_data.m_Target.GetObject() );
	
		module_recipes_manager.PerformRecipeServer( wcActionData.m_RecipeID, action_data.m_MainItem, item2, action_data.m_Player );
	}
	
	override void OnCompleteLoopClient( ActionData action_data )
	{
		ActionManagerClient am = ActionManagerClient.Cast(action_data.m_Player.GetActionManager());
		am.UnlockInventory(action_data);
	}
	
	override void WriteToContext (ParamsWriteContext ctx, ActionData action_data)
	{
		PlayerBase player;
		Class.CastTo(player, GetGame().GetPlayer());

		ctx.Write(player.GetCraftingManager().m_item1);
		ctx.Write(player.GetCraftingManager().m_item2);
		ctx.Write(player.GetCraftingRecipeID());
	}
	
	override bool ReadFromContext(ParamsReadContext ctx, ActionData action_data )
	{
		ItemBase item1 = null;
		ItemBase item2 = null;
		int recipeID = -1;
		if (!ctx.Read(item1))
				return false;
		if (!ctx.Read(item2))
				return false;
		if (!ctx.Read(recipeID))
				return false;
		
		
		WorldCraftActionData wcActionData;
		Class.CastTo(wcActionData, action_data);
		
		ref ActionTarget target;
		target = new ActionTarget(item2 , NULL, -1,vector.Zero, 0);
						
		wcActionData.m_MainItem = item1;
		wcActionData.m_Target = target;
		wcActionData.m_RecipeID = recipeID;
		wcActionData.m_Player.SetCraftingRecipeID(recipeID);//TODO remove this after refactor will work
		return true;
	}
};

