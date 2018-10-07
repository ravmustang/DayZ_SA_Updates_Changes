class ActionGetInTransport: ActionInteractBase
{
	private Transport m_transport;
	private int       m_crewIdx;


	void ActionGetInTransport()
	{
		m_MessageSuccess = "";
		//m_CommandUID = DayZPlayerConstants.;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = "GetInDriver";
	}


	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_GETIN_TRANSPORT;
	}

	override string GetText()
	{
		return "Get in vehicle";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_transport = null;
		m_crewIdx   = -1;

		if ( !target )
			return false;

		if ( !Class.CastTo(m_transport, target.GetObject()) )
			return false;

		if ( player.GetCommand_Vehicle() )
			return false;
		
		//if ( !IsInReach(player, target, UAMaxDistances.DEFAULT) )
		//	return false;

		int componentIndex = target.GetComponentIndex();

		m_crewIdx = m_transport.CrewPositionIndex(componentIndex);
		if ( m_crewIdx < 0 )
			return false;

		Human crew = m_transport.CrewMember( m_crewIdx );
		if ( crew )
			return false;
		
		if ( !m_transport.CrewCanGetThrough( m_crewIdx ) )
			return false;
		
		return true;
	}

	override void Start( ActionData action_data )
	{
		int seat = m_transport.GetSeatAnimationType(m_crewIdx);
		HumanCommandVehicle vehCommand = action_data.m_Player.StartCommand_Vehicle(m_transport, m_crewIdx, seat);
		if( vehCommand )
		{
			vehCommand.SetVehicleType(m_transport.GetAnimInstance());
		}
	}

	override bool IsInstant()
	{
		return true;
	}
};
