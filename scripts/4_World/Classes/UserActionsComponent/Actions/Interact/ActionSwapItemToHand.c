class ActionSwapItemToHands: ActionInteractBase
{
	string m_ItemName = "";

	void ActionSwapItemToHands()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
		m_CommandUIDProne   = DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HANDS;

		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTDummy();
	}	
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_SWAP_ITEM_TO_HANDS;
	}

	override string GetText()
	{
		return "Swap to hands";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI entity;
		ItemBase ih = player.GetItemInHands();
		if ( ih && item && Class.CastTo(entity, target.GetObject()) )
		{
			bool canAdd = player.GetInventory().CanAddEntityIntoHands(entity);
			
			if ( entity && !player.GetInventory().CanAddEntityIntoInventory(entity) && entity.CanPutIntoHands(player) && !canAdd && entity.GetHierarchyRootPlayer() != player )
			{
				return true;
			}
		}
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		EntityAI eih = EntityAI.Cast(action_data.m_Player.GetItemInHands());
		EntityAI ntarget = EntityAI.Cast(action_data.m_Target.GetObject());
		action_data.m_Player.LocalSwapEntities(ntarget, eih);
	}
	override void OnCompleteClient( ActionData action_data )
	{
		EntityAI eih = EntityAI.Cast(action_data.m_Player.GetItemInHands());
		EntityAI ntarget = EntityAI.Cast(action_data.m_Target.GetObject());
		action_data.m_Player.LocalSwapEntities(ntarget, eih);
	}
};