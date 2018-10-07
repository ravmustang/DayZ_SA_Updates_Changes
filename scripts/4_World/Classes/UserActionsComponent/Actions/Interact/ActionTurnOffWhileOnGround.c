class ActionTurnOffWhileOnGround: ActionInteractBase
{	
	void ActionTurnOffWhileOnGround()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess 	= "I've switched it off.";
		m_MessageFail 		= "It's already turned off.";
	}
	
	override int GetType()
	{
		return AT_TURN_OFF_WHILE_ON_GROUND;
	}

	override string GetText()
	{
		return "Switch off";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		
		if ( player.IsAlive() && target_entity.HasEnergyManager()  &&  target_entity.GetCompEM().CanSwitchOff() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnCompleteServer( ActionData action_data )
	{
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetObject() );
		
		if ( target_entity.GetCompEM().CanSwitchOff() )
		{
			target_entity.GetCompEM().SwitchOff();
			
			InformPlayers( action_data.m_Player, action_data.m_Target, UA_FINISHED );	//Success
		}
		else
		{
			InformPlayers( action_data.m_Player, action_data.m_Target, UA_FAILED );		//Already turned off
		}	
	}
}