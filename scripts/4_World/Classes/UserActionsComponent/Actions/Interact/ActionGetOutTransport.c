class ActionGetOutTransport: ActionInteractBase
{
	private Transport m_transport;
	private int       m_crewIdx;


	void ActionGetOutTransport()
	{
		m_MessageSuccess = "";
		//m_CommandUID = DayZPlayerConstants.;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = "GetInDriver";
	}


	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_GETOUT_TRANSPORT;
	}

	override string GetText()
	{
		return "Leave vehicle";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
 		m_transport = null;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			m_transport = vehCommand.GetTransport();
			if ( m_transport )
			{
				m_crewIdx = m_transport.CrewMemberIndex( player );
				if ( m_crewIdx >= 0 && m_transport.CrewCanGetThrough( m_crewIdx ) )
				return true;
			}
		}

		return false;
	}

	override void Start( ActionData action_data )
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			
			if ( trans )
			{
				Car car;
				if ( Class.CastTo(car, trans) )
				{
					float speed = car.GetSpeedometer();
					if ( speed <= 5 )
					{
						vehCommand.GetOutVehicle();
					}
					else
					{
						vehCommand.JumpOutVehicle();
					}
				}
			}
		}
	}
	
	override bool IsInstant()
	{
		return true;
	}
};
