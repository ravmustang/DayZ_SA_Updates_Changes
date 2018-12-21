class ActionLightItemOnFireCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.FIREPLACE_IGNITE );
	}
}

class ActionLightItemOnFire: ActionContinuousBase
{
	void ActionLightItemOnFire()
	{
		m_CallbackClass = ActionLightItemOnFireCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIRE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_LIGHT_ITEM_ON_FIRE;
	}
		
	override string GetText()
	{
		return "#ignite";
	}

	override bool CanBePerformedFromQuickbar()
	{
		return true;
	}
	
	override bool CanBePerformedFromInventory()
	{
		return true;
	}
	// Check if ignite candidate is in cargo of something or not.
	bool IsItemInCargoOfSomething(ItemBase item)
	{
		if ( item.GetInventory() )
		{
			InventoryLocation loc = new InventoryLocation;
			item.GetInventory().GetCurrentInventoryLocation(loc);
			
			if ( loc.GetIdx() > -1 )
			{
				return true;
			}
		}
		
		return false;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		ItemBase target_item = ItemBase.Cast( target.GetObject() );
		
		if ( target_item && item )
		{
			if ( !target_item.IsIgnited()  &&  !IsItemInCargoOfSomething(target_item)  &&  item.CanIgniteItem( target_item )  &&  target_item.CanBeIgnitedBy( item ) )
			{
				return true;
			}
			
			else if ( !item.IsIgnited()  &&  !IsItemInCargoOfSomething(item)  &&  target_item.CanIgniteItem( item )  &&  item.CanBeIgnitedBy( target_item ) )
			{
				return true;		
			}
		}
		
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		ItemBase target_item = ItemBase.Cast( action_data.m_Target.GetObject() );
		ItemBase item = action_data.m_MainItem;
		
		bool is_ignition_successful;
		ItemBase ignited_item;				//item that was lit on fire
		ItemBase fire_source_item;			//item that was the source of fire
		
		if ( item.CanIgniteItem( target_item ) )
		{
			is_ignition_successful = target_item.IsTargetIgnitionSuccessful( item );
			ignited_item = target_item;
			fire_source_item = item;
		}
		else if ( item.CanBeIgnitedBy( target_item ) )
		{
			is_ignition_successful = item.IsThisIgnitionSuccessful( target_item );
			ignited_item = item;
			fire_source_item = target_item;			
		}
		
		if ( is_ignition_successful )
		{
			fire_source_item.OnIgnitedTarget( ignited_item );
			ignited_item.OnIgnitedThis( fire_source_item );			
		}
		else
		{
			fire_source_item.OnIgnitedTargetFailed( ignited_item );
			ignited_item.OnIgnitedThisFailed( fire_source_item );			
		}
	}
	
	//setup
	override bool SetupAction( PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL )
	{	
		if( super.SetupAction( player, target, item, action_data, extra_data ) )
		{
			ItemBase target_item = ItemBase.Cast( target.GetObject() );
			if ( target_item )
			{
				SetIgnitingAnimation( target_item );
			}
			
			return true;
		}
		
		return false;
	}
	
	void SetIgnitingAnimation( ItemBase target_item )
	{
		if( target_item.HasFlammableMaterial() )
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIRE;
		}
		else
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		}
	}
}