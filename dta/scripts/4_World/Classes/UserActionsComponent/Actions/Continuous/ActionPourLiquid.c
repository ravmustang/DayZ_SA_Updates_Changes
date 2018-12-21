class ActionPourLiquidCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.25;

	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityLiquidTransfer(UAQuantityConsumed.POUR_LIQUID, TIME_TO_REPEAT);
	}
	
/*	override void OnAnimationEvent(int pEventID)	
	{
		super.OnAnimationEvent( pEventID );
				
		Bottle_Base target_vessel = Bottle_Base.Cast( m_ActionData.m_Target.GetObject());
		
		switch (pEventID)
		{
			case UA_ANIM_EVENT:			
				if ( !GetGame().IsMultiplayer() && GetGame().IsServer() )
				{		
					//local singleplayer
					target_vessel.PlayPouringSound();
				}
			
				if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
				{			
					target_vessel.PlayPouringSound();					
				}
			
				if ( GetGame().IsMultiplayer() && GetGame().IsClient() )
				{			
					
				}

			break;
		}
	}
	
	override void EndActionComponent()
	{
		super.EndActionComponent();
		
		Bottle_Base target_vessel = Bottle_Base.Cast( m_ActionData.m_Target.GetObject());

		if ( !GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			//local singleplayer
			target_vessel.StopPouringSound();
		}
		
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{			
			target_vessel.StopPouringSound();					
		}
		
		if ( GetGame().IsMultiplayer() && GetGame().IsClient() )
		{		
		
		}
	}*/
};

class ActionPourLiquid: ActionContinuousBase
{
	void ActionPourLiquid()
	{
		m_CallbackClass = ActionPourLiquidCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTY_VESSEL;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		
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
	
	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		ItemBase item = player.GetItemInHands();
		
		return "#pour_liquid";
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
	
	override void OnExecuteServer( ActionData action_data ) 
	{
		Bottle_Base target_vessel = Bottle_Base.Cast( action_data.m_Target.GetObject());
		if (target_vessel) target_vessel.PlayPouringSound();
	}
	
	override void OnStartServer( ActionData action_data )
	{
		action_data.m_Player.SetLiquidTendencyDrain(false);
	}
	
	override void OnEndServer( ActionData action_data )
	{
		Bottle_Base target_vessel = Bottle_Base.Cast( action_data.m_Target.GetObject());
		if (target_vessel) target_vessel.StopPouringSound();
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
	}
};