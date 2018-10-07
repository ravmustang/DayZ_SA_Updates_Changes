class ActionTurnOnTransmitterOnGround: ActionInteractBase
{
	void ActionTurnOnTransmitterOnGround()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess = "I have turned it on.";
		m_MessageFail = "Cannot turn on the device without power source.";
	}

	override int GetType()
	{
		return AT_TURN_ON_TRANSMITTER_ON_GROUND;
	}

	override string GetText()
	{
		return "Turn on";
	}
		
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		if ( player && target_object && target_object.IsStaticTransmitter() )
		{
			EntityAI target_entity = EntityAI.Cast( target_object );
			string selection = target_object.GetActionComponentName(target.GetComponentIndex());
			
			if ( target_entity.HasEnergyManager()  &&  target_entity.GetCompEM().CanSwitchOn() && selection == "power_panel" )
			{
				AdvancedCommunication transmitter = AdvancedCommunication.Cast( target_object );
				transmitter.DisplayRadioInfo( "OFF", player );
				
				return true;
			}
		}
		
		return false;
	}
		
	override void OnCompleteServer( ActionData action_data )
	{
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetObject() );
		
		if ( target_entity.GetCompEM().CanWork() )
		{
			target_entity.GetCompEM().SwitchOn();
			
			InformPlayers(action_data.m_Player,action_data.m_Target,UA_FINISHED);	//Success
		}
		else
		{
			
			InformPlayers(action_data.m_Player,action_data.m_Target,UA_FAILED);		//No power
		}	
	}
}