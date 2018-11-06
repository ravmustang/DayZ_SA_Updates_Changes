class ActionOpenFence: ActionInteractBase
{
	void ActionOpenFence()
	{
		m_MessageSuccess = "";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_FENCE_OPEN;
	}

	override string GetText()
	{
		return "#open_fence";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			Fence fence = Fence.Cast( targetObject );
			
			if ( fence && fence.CanOpenFence() )
			{
				string selection = fence.GetActionComponentName( target.GetComponentIndex() );
				
				if ( selection == "wall_interact" )
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		Fence fence = Fence.Cast( action_data.m_Target.GetObject() );
		fence.OpenFence();
	}
}