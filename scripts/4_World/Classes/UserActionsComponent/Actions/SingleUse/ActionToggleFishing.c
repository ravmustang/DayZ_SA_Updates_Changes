class ActionToggleFishing: ActionSingleUseBase
{
	void ActionToggleFishing()
	{
		m_MessageStart = " ";
		m_MessageSuccess = " ";
		m_MessageFail = "Fishing rod is ruined.";
		m_Sound = "CastingRod";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_TOGGLE_FISHING;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#start_or_end_fishing";
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		vector pos_cursor = target.GetCursorHitPos();
		float distance = Math.AbsInt(vector.Distance(pos_cursor,player.GetPosition()));
		if ( distance <= 2 /*//m_MaximalActionDistance*/ && g_Game.SurfaceIsPond(pos_cursor[0], pos_cursor[2]) /*action_data.m_Player.IsWaterContact()*/ )
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
		FishingRod_Base nitem = FishingRod_Base.Cast( action_data.m_MainItem );
		if( nitem.IsFishingActive() )
		{
			nitem.DeactivateFishing();
			action_data.m_MainItem.SetAnimationPhase("CloseRod",1);
			action_data.m_MainItem.SetAnimationPhase("OpenRod",0);
		}
		else
		{
			nitem.ActivateFishing();
			action_data.m_MainItem.SetAnimationPhase("CloseRod",0);
			action_data.m_MainItem.SetAnimationPhase("OpenRod",1);
		}
	}
};