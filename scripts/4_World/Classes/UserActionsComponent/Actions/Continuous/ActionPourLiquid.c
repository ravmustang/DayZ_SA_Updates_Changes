class ActionPourLiquidCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.25;

	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityLiquidTransfer(UAQuantityConsumed.POUR_LIQUID, TIME_TO_REPEAT);
	}
};

class ActionPourLiquid: ActionContinuousBase
{
	void ActionPourLiquid()
	{
		m_CallbackClass = ActionPourLiquidCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYBOTTLE;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_POURBOTTLE;
		
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started filling the bottle.";
		m_MessageSuccess = "I have finished filling the bottle..";
		m_MessageFail = "Player moved and filling the bottle was canceled.";
		m_MessageCancel = "I stopped filling the bottle.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_POUR_LIQUID;
	}
	
	//?
	override bool Consumable()
	{
		return true;
	}
	
	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		ItemBase item = player.GetItemInHands();
		
		//return "fill "+item.GetDisplayName().Substring(0,(item.GetDisplayName().Length() )); //crops the '' bit from the displayname
		return "Pour liquid";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		ItemBase target_item = ItemBase.Cast(target.GetObject());
		if ( target_item && item && GetGame().IsServer() && GetGame().IsMultiplayer() )
			return true;
		
		if ( GetGame().IsInventoryOpen() && !player.GetLiquidTendencyDrain() )
		{
			player.SetLiquidTendencyDrain(true);
			return false;
		}
		
		if ( target_item && item && item.GetQuantity() > item.GetQuantityMin() && target_item.GetQuantity() < target_item.GetQuantityMax() && !player.GetLiquidTendencyDrain() && Liquid.CanFillContainer( target_item, item.GetLiquidType() ) )
		{
			return true;
		}
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		action_data.m_Player.SetLiquidTendencyDrain(false);
	}
};