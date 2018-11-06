class ActionTakeItemToHands: ActionInteractBase
{
	void ActionTakeItemToHands()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HANDS;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}	
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_TAKE_ITEM_TO_HANDS;
	}

	override string GetText()
	{
		return "#take_to_hands";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase tgt_item = ItemBase.Cast( target.GetObject() );
		if ( tgt_item && !tgt_item.IsTakeable() ) return false;
		if ( tgt_item && tgt_item.IsBeingPlaced() ) return false;
		
		if ( player.GetCommand_Vehicle() )
			return false;
		
		EntityAI tgt_parent = EntityAI.Cast( target.GetParent() );
		EntityAI tgt_entity = EntityAI.Cast( target.GetObject() );

		if ( tgt_entity && !tgt_parent )
		{
			if ( tgt_entity && tgt_entity.IsItemBase() && !player.GetInventory().CanAddEntityIntoInventory(tgt_entity) && player.GetInventory().CanAddEntityIntoHands(tgt_entity) && tgt_entity.GetHierarchyRootPlayer() != player )
			{
				return true;
			}
		}
		return false;
	}
	
	override bool CanContinue( ActionData action_data )
	{
		return true;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		vector rotation = vector.Zero;
		vector target_dir = action_data.m_Target.GetObject().GetPosition() - action_data.m_Player.GetPosition();
		
		rotation[0] = Math.Atan2(target_dir[0], target_dir[2]) * Math.RAD2DEG;
		action_data.m_Player.SetOrientation(rotation);
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		EntityAI ntarget = EntityAI.Cast(action_data.m_Target.GetObject());
		action_data.m_Player.PredictiveTakeEntityToHands(ntarget);
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		EntityAI ntarget = EntityAI.Cast(action_data.m_Target.GetObject());
		action_data.m_Player.PredictiveTakeEntityToHands(ntarget);
	}
	
	override void CreateAndSetupActionCallback( ActionData action_data )
	{
		Object target = action_data.m_Target.GetObject();
		bool heavy_item = false;
		ActionBaseCB callback;
		if (target && target.ConfigIsExisting("heavyItem") && target.ConfigGetBool("heavyItem"))
		{
			heavy_item = true;
		}
			
		if(heavy_item)
		{
			Class.CastTo(callback, action_data.m_Player.StartCommand_Action(DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HEAVY,GetCallbackClassTypename(),DayZPlayerConstants.STANCEMASK_ERECT));
		}
		else
		{
			if( action_data.m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT) )
			{
				Class.CastTo(callback, action_data.m_Player.AddCommandModifier_Action(DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS,GetCallbackClassTypename()));
			}
			else
			{
				Class.CastTo(callback, action_data.m_Player.StartCommand_Action(DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HANDS,GetCallbackClassTypename(),DayZPlayerConstants.STANCEMASK_PRONE));
			}
		}
		callback.SetActionData(action_data); 
		callback.InitActionComponent();
		action_data.m_Callback = callback;
	}
};