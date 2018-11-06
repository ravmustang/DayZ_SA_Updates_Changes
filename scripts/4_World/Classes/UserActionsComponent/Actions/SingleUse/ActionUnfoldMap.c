class ActionUnfoldMapCB : HumanCommandActionCallback //ActionSingleUseBaseCB
{
	PlayerBase m_player;
	
	string closed_map = "ChernarusMap";
	string opened_map = "ChernarusMap_Open";
	
	void ActionUnfoldMapCB()
	{
		RegisterAnimationEvent("ActionExec", UA_ANIM_EVENT);
	}
	
	void ~ActionUnfoldMapCB()
	{
		if (!m_player)
			return;
		
		if (GetGame().IsClient())
		{
			GetGame().GetMission().PlayerControlEnable();
		}
		
		m_player.m_MapOpen = false;
		
		if (!GetGame().IsServer())
			return;
		
		ItemBase chernomap = m_player.GetItemInHands();
		if (m_player.IsAlive() && chernomap && chernomap.GetType() == opened_map)
		{
			MiscGameplayFunctions.TurnItemIntoItem(chernomap, closed_map, m_player); //fallback, if animation event does not fire for any reason
		}
	}
	
	/*override void OnFinish(bool pCanceled)
	{
		if (!m_player)
			return;
		
		m_player.m_MapOpen = false;
		
		if (!GetGame().IsServer())
			return;
		
		ItemBase chernomap = m_player.GetItemInHands();
		if (m_player.IsAlive() && chernomap && chernomap.GetType() == opened_map)
		{
			MiscGameplayFunctions.TurnItemIntoItem(chernomap, closed_map, m_player); //fallback, if animation event does not fire for any reason
		}
	}*/
	
	override void OnAnimationEvent(int pEventID)
	{
		if (!m_player)
			return;
			
		ItemBase chernomap = m_player.GetItemInHands();
		
		switch (pEventID)
		{			
			case UA_ANIM_EVENT: 
				if (m_player.IsSwimming() || m_player.IsFalling() || m_player.IsClimbingLadder() || m_player.IsUnconscious() || m_player.IsRestrained())
					return;
			
				if (chernomap && chernomap.GetType() == closed_map)
				{
					if (GetGame().IsServer())
						MiscGameplayFunctions.TurnItemIntoItem(chernomap, opened_map, m_player);
			
					if (!GetGame().IsMultiplayer() || GetGame().IsClient())
					{
						UIManager 	m_UIManager;
						m_UIManager = GetGame().GetUIManager();
						m_UIManager.CloseAll();
						m_UIManager.EnterScriptedMenu(MENU_MAP, NULL);
					}
					m_player.m_MapOpen = true;
				}
				else if (chernomap && chernomap.GetType() == opened_map)
				{
					if (GetGame().IsServer())
						MiscGameplayFunctions.TurnItemIntoItem(chernomap, closed_map, m_player);
				}
			break;
		}
	}
}

class ActionUnfoldMap: ActionSingleUseBase
{
	void ActionUnfoldMap()
	{
	}
	
	override bool IsInstant()
	{
		return true;
	}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_UNFOLD_MAP;
	}

	override string GetText()
	{
		return "#unfold_map";
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (player.m_hac || player.m_MapOpen)
		{
			return false;
		}
		return true;
	}
	
	override void OnStartClient( ActionData action_data )
	{
		OpenMap( action_data );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		OpenMap( action_data );
	}
	
	void OpenMap( ActionData action_data )
	{
		if (!action_data.m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
		{
			action_data.m_Player.m_hac = ActionUnfoldMapCB.Cast(action_data.m_Player.AddCommandModifier_Action(DayZPlayerConstants.CMD_ACTIONMOD_VIEWMAP,ActionUnfoldMapCB));
		}
		else
		{
			action_data.m_Player.m_hac = ActionUnfoldMapCB.Cast(action_data.m_Player.StartCommand_Action(DayZPlayerConstants.CMD_ACTIONFB_VIEWMAP,ActionUnfoldMapCB,GetStanceMask(action_data.m_Player)));
		}
		
		// sets player for associated callback to use
		if (action_data.m_Player.m_hac)
			action_data.m_Player.m_hac.m_player = action_data.m_Player;
		
		/*action_data.m_Player.m_MapOpen = true;
		
		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			UIManager 	m_UIManager;
			m_UIManager = GetGame().GetUIManager();
			m_UIManager.EnterScriptedMenu(MENU_MAP, NULL);
		}*/
	}
};