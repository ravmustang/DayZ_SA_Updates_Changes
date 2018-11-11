	//Advanced placement
	//vector entity_position;
	//vector entity_orientation;


class ActionManagerServer: ActionManagerBase
{		
	protected ActionBase 				m_PendingAction;
	ref ActionReciveData				m_PendingActionReciveData;
	
	void ActionManagerServer(PlayerBase player)
	{
		m_PendingAction = NULL;
		m_PendingActionReciveData = NULL;
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
	
	override void OnJumpStart()
	{
		if(m_CurrentActionData)
		{
			if( m_CurrentActionData.m_State == UA_AM_PENDING || m_CurrentActionData.m_State == UA_AM_REJECTED || m_CurrentActionData.m_State == UA_AM_ACCEPTED)
			{
				OnActionEnd();
				m_PendingActionAcknowledgmentID = -1;
			}
			else
			{
				m_CurrentActionData.m_Action.Interrupt(m_CurrentActionData);
			}
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
				
				if (!recvAction.ReadFromContext(ctx, m_PendingActionReciveData))
				{
					success = false;
				}
				
				m_PendingAction = recvAction;
					
				if (recvAction.UseAcknowledgment())
				{
					int AckID;
					if (!ctx.Read(AckID))
						success = false;
						
					m_PendingActionAcknowledgmentID = AckID;
				}
					
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
				DayZPlayerSyncJunctures.SendActionInterrupt(m_Player);
			}
			//m_CurrentActionData = NULL;
			return false;
		}
		//StartDeliveredAction();
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
		
		ActionBase picked_action;
		bool accepted = false;
		bool is_target_free = true;
		ref ActionTarget target;
		ItemBase item;
		
		picked_action = m_CurrentActionData.m_Action;
		target = m_CurrentActionData.m_Target;
		item = m_CurrentActionData.m_MainItem;
		
		if ( picked_action.CanBePerformedFromQuickbar() )
			m_CurrentActionData.m_Player.SetActionEndInput();

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
				m_CurrentActionData.m_State = UA_AM_ACCEPTED;
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
				DayZPlayerSyncJunctures.SendActionInterrupt(m_Player);
			}
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
	
	
	override void Update(int pCurrentCommandID)
	{
		super.Update(pCurrentCommandID);
		
		if (m_PendingAction)
		{
			if ( m_CurrentActionData )
			{
				DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_PendingActionAcknowledgmentID, false);
			}
			else
			{
				ref ActionTarget target = new ActionTarget(NULL, NULL, -1, vector.Zero, 0); 
				bool success = true;
						
				Debug.Log("[Action DEBUG] Start time stamp ++: " + m_Player.GetSimulationTimeStamp());
				if (!m_PendingAction.SetupAction(m_Player,target,m_Player.GetItemInHands(),m_CurrentActionData))
				{
					success = false;
				}
				Debug.Log("[AM] Action data synced (" + m_Player + ")");
			
				if (success)
				{
					StartDeliveredAction();
				}	
				else
				{
					if (m_PendingAction.UseAcknowledgment())
					{
						DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_PendingActionAcknowledgmentID, false);
					}
					else
					{
						DayZPlayerSyncJunctures.SendActionInterrupt(m_Player);
					}
				}
			}
			m_PendingAction = NULL;
			m_PendingActionReciveData = NULL;
		}
	
		if (m_CurrentActionData)
		{
			switch (m_CurrentActionData.m_State)
			{
				case UA_AM_PENDING:
					break;
			
				case UA_AM_ACCEPTED:
					// check pCurrentCommandID before start or reject 
					if( ActionPossibilityCheck(pCurrentCommandID) && pCurrentCommandID != DayZPlayerConstants.COMMANDID_SWIM && pCurrentCommandID != DayZPlayerConstants.COMMANDID_LADDER )
					{
						if(!m_Player.IsRestrained() || m_CurrentActionData.m_Action.CanBeUsedInRestrain() )
						{
							m_CurrentActionData.m_State = UA_START;
							m_CurrentActionData.m_Action.Start(m_CurrentActionData);
							if( m_CurrentActionData.m_Action && m_CurrentActionData.m_Action.IsInstant() )
								OnActionEnd();
						}
						else
						{
							OnActionEnd();
						}
					}
					else
					{
						OnActionEnd();
					}
					m_PendingActionAcknowledgmentID = -1;
					break;
				
				case UA_AM_REJECTED:
					OnActionEnd();
					m_PendingActionAcknowledgmentID = -1;
			
					//m_Player.GetDayZPlayerInventory().UnlockHands();
					break;
			
				default:
					break;
			}
		}
	}
	
	
	override void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		int AcknowledgmentID;
		pCtx.Read(AcknowledgmentID);
		
		if ( m_CurrentActionData && AcknowledgmentID == m_PendingActionAcknowledgmentID)
		{
			if (pJunctureID == DayZPlayerSyncJunctures.SJ_ACTION_ACK_ACCEPT)
				m_CurrentActionData.m_State = UA_AM_ACCEPTED;
			else if (pJunctureID == DayZPlayerSyncJunctures.SJ_ACTION_ACK_REJECT)
				m_CurrentActionData.m_State = UA_AM_REJECTED;
		}
	}
	
	/*private void Interrupt()
	{
		DayZPlayerSyncJunctures.SendActionInterrupt(m_Player);
	}*/
	
	override ActionReciveData GetReciveData()
	{
		return m_PendingActionReciveData;
	}
};