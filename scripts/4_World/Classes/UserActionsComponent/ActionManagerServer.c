	//Advanced placement
	//vector entity_position;
	//vector entity_orientation;


class ActionManagerServer: ActionManagerBase
{		
	void ActionManagerServer(PlayerBase player)
	{
		//ActionManagerBase(player);
	}
	
	//------------------------------------------
	//EVENTS 
	//------------------------------------------
	// Continuous---------------------------------------------
	override void OnContinuousStart()
	{
//			StartDeliveredAction();
	}
	
	override void OnContinuousCancel()
	{
		if( !m_CurrentActionData || m_CurrentActionData.m_Action.GetActionCategory() != AC_CONTINUOUS )
			return;
			
		if( m_CurrentActionData.m_State == UA_AM_PENDING || m_CurrentActionData.m_State == UA_AM_ACCEPTED || m_CurrentActionData.m_State == UA_AM_REJECTED )
		{
			OnActionEnd();
			m_PendingActionAcknowledgmentID = -1;
		}
		else
		{
			m_CurrentActionData.m_Action.OnContinuousCancel(m_CurrentActionData);
		}
	}
	
	// Single----------------------------------------------------
	override void OnSingleUse()
	{
//			StartDeliveredAction();
	}
	
	// Interact----------------------------------------------------
	override void OnInteractAction() //Interact
	{
//		if ( m_SelectedActionIndex == 0 ) // tertiary action is always on index 0 now
//		{
//				StartDeliveredAction();
//		}
//		else
//		{
		//	rozbal debug akci
//		}

	}

	override bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{		
		switch ( userDataType )
		{
			case INPUT_UDT_STANDARD_ACTION:
			{
				bool success = true;
				
				int actionID = 0;
				if (!ctx.Read(actionID) )
					return false;
				
				ActionBase recvAction = GetAction(actionID);
				
				if (!recvAction)
					return false;
				
				switch (actionID)
				{
					/*case AT_DEBUG:
					{
						ItemBase targetItem = null;
						if ( !ctx.Read(targetItem) ) //jtomasik - proc ho nenacte?
							return false;

						int debugActionID = 0;
						if ( !ctx.Read(debugActionID) ) //jtomasik - proc ho nenacte?
							return false;
						
						targetItem.OnAction(debugActionID, m_Player, NULL);
						break;
					}*/
					
					default:
						ref ActionTarget target = new ActionTarget(NULL, NULL, -1, vector.Zero, 0);
						if (!recvAction.SetupAction(m_Player,target,m_Player.GetItemInHands(),m_CurrentActionData))
						{
							success = false;
						}
						if (!recvAction.ReadFromContext(ctx, m_CurrentActionData))
						{
							success = false;
						}
				}
					
				if (recvAction.UseAcknowledgment())
				{
					int AckID;
					if (!ctx.Read(AckID))
						success = false;
						
					m_PendingActionAcknowledgmentID = AckID;
				}
					
				m_CurrentActionData.m_State = UA_AM_PENDING;
				break;
			}
			default:
				return false;
		}
		
		if (!success)
		{
			if (recvAction.UseAcknowledgment())
			{
				DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_PendingActionAcknowledgmentID, false);
			}
			else
			{
				Interrupt();
			}
			m_CurrentActionData = NULL;
			return false;
		}
		StartDeliveredAction();
		return true;
		
	}
	
	override void StartDeliveredAction()
	{
		if( !m_CurrentActionData )
		{
			//! error - expected action data
			//Interrupt();
			return;
		}
		
		m_Player.SetActionEndInput();
		
		ActionBase picked_action;
		bool accepted = false;
		bool is_target_free = true;
		ref ActionTarget target;
		ItemBase item;
		
		picked_action = m_CurrentActionData.m_Action;
		target = m_CurrentActionData.m_Target;
		item = m_CurrentActionData.m_MainItem;

		if( is_target_free && !m_Player.GetCommandModifier_Action() && !m_Player.GetCommand_Action() && !m_Player.IsSprinting() && picked_action && picked_action.Can(m_Player,target,item)) 
		{
			accepted = true;
			if( picked_action.HasTarget())
			{
				EntityAI targetEntity;
				if ( EntityAI.CastTo(targetEntity,target.GetObject()) )
				{
					//Man target_owner;
					//target_owner = targetEntity.GetHierarchyRootPlayer();
					//if( target_owner && target_owner != m_Player && target_owner.IsAlive() )
					//{
					//	accepted = false;
					//}
					/*else*/ if( !Building.Cast(targetEntity))
					{
						//Lock target
						if( !GetGame().AddActionJuncture(m_Player,targetEntity,10000) )
						{
							accepted = false;
						}
					}
				}
			}
		}
		
		if( accepted )
		{
			if(picked_action.UseAcknowledgment())
			{
						//Unlock target
						//GetGame().ClearJuncture(m_Player, oldTrgetItem);
				m_CurrentActionData.m_State = UA_AM_PENDING;
				DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_PendingActionAcknowledgmentID, true);
			}
			else
			{
				picked_action.Start(m_CurrentActionData);
				if( picked_action.IsInstant() )
				{
					OnActionEnd();
				}
			}
		}
		else
		{
			if (picked_action.UseAcknowledgment())
			{
				DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_PendingActionAcknowledgmentID, false);
			}
			else
			{
				Interrupt();
			}
			m_CurrentActionData = NULL;
		}
	}
	
	override void OnActionEnd()
	{
		if(m_CurrentActionData)
		{
			if( m_CurrentActionData.m_Target )
			{
				EntityAI targetEntity;
				if( targetEntity.CastTo(targetEntity,m_CurrentActionData.m_Target.GetObject()))
				{
					if( !Building.Cast(targetEntity))
					{
						GetGame().ClearJuncture(m_CurrentActionData.m_Player, targetEntity);
					}
				}
			}
			super.OnActionEnd();
		}
	}
	
	
	override void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		int AcknowledgmentID;
		pCtx.Read(AcknowledgmentID);
		if( AcknowledgmentID == m_PendingActionAcknowledgmentID)
		{
			
			if(pJunctureID == DayZPlayerSyncJunctures.SJ_ACTION_ACK_ACCEPT)
			{
				ref HumanMovementState		state 	= new HumanMovementState();
		
		
				m_Player.GetMovementState(state);
				int movementStateID = state.m_CommandTypeId;

				if( ActionPossibilityCheck(movementStateID) && movementStateID != DayZPlayerConstants.COMMANDID_SWIM && movementStateID != DayZPlayerConstants.COMMANDID_LADDER )
				{
					m_CurrentActionData.m_Action.Start(m_CurrentActionData);
					if( m_CurrentActionData.m_Action.IsInstant() )
						OnActionEnd();
				}
				m_PendingActionAcknowledgmentID = -1;
			}
				
			if(pJunctureID == DayZPlayerSyncJunctures.SJ_ACTION_ACK_REJECT)
			{
				m_PendingActionAcknowledgmentID = -1;
			}
		}
	
	}
	
	private void Interrupt()
	{
		DayZPlayerSyncJunctures.SendActionInterrupt(m_Player);
	}
};