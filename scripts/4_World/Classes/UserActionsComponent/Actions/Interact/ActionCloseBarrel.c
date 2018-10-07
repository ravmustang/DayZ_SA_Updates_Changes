class ActionCloseBarrel: ActionInteractBase
{
	void ActionCloseBarrel()
	{
		m_MessageSuccess = "I have closed it.";
		//m_Animation = "close";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override int GetType()
	{
		return AT_CLOSE_BARREL;
	}

	override string GetText()
	{
		return "Close";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		if ( target_object.IsItemBase() )
		{
			Barrel_ColorBase ntarget = Barrel_ColorBase.Cast( target_object );
			if( ntarget )
			{
				if ( !ntarget.IsLocked() && ntarget.IsOpen() )
				{
					return true;
				}
			}
		}
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		Object target_object = action_data.m_Target.GetObject();
		Barrel_ColorBase ntarget = Barrel_ColorBase.Cast( target_object );
		
		if( ntarget )
		{
			ntarget.DetermineAction(action_data.m_Player);
		}
	}
}